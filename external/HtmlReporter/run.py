#!/usr/bin/python3
import argparse
import base64
import pathlib
import zlib
import json
import codecs
import os
import hashlib
from pathlib import Path
from jsmin import jsmin
from rcssmin import cssmin

scriptPath = pathlib.Path(__file__).parent.resolve()
reportTemplatePath = os.path.join(scriptPath, "HtmlReportTemplate/ReportTemplate.html")
jsPath = os.path.join(scriptPath, "HtmlReportTemplate")
headStr = "<script id='cooddyResults' type='text/json'>"



def makeResultPath(root, file, resultDir):
    if os.path.isfile(root):
        root = os.path.dirname(root)
    resultPath = os.path.join(resultDir, os.path.relpath(file, root))
    Path(os.path.dirname(resultPath)).mkdir(parents=True, exist_ok=True)
    return resultPath


def makeResultPathWithExt(root, file, resultDir, extension, addExtension=False):
    if addExtension:
        file = file + extension
    else:
        file = os.path.splitext(file)[0] + extension
    return makeResultPath(root, file, resultDir)


def getFileText(file, relto=""):
    r = os.path.join(relto, file)
    if os.path.exists(r):
        file = r
    if not os.path.exists(file):
        return ""
    with codecs.open(file, encoding='utf-8', errors='ignore') as file:
        return file.read()


def getFiles(rootdir, exts, cwd=''):
    if os.path.isfile(rootdir):
        return [rootdir]
    ret = []
    for root, dir, files in os.walk(rootdir):
        for file in files:
            if os.path.splitext(file)[1] in exts:
                ret.append(os.path.join(cwd, root, file))
    return ret


def deflate(data):
    compress = zlib.compressobj(9, zlib.DEFLATED, -zlib.MAX_WBITS, zlib.DEF_MEM_LEVEL, 0)
    deflated = compress.compress(data)
    deflated += compress.flush()
    return deflated


def pack(text):
    deflated = deflate(bytes(text, 'utf-8'))
    header = bytearray(b'x\x01')
    header.extend(deflated)
    return base64.b64encode(header).decode('utf-8')


def inflate(data):
    decompress = zlib.decompressobj(-zlib.MAX_WBITS)
    inflated = decompress.decompress(data)
    inflated += decompress.flush()
    return inflated


def constructArgParser():
    parser = argparse.ArgumentParser(
        description='Create HTML report from problems list in json or html format and source code')
    parser.add_argument('--report', type=str, required=True, help='Json with problems list')
    parser.add_argument('--sources-path', type=str, required=False, help='Path to source code root folder', default="")
    parser.add_argument('--result', type=str, required=False, help='Output report file or directory',
                        default='./cooddy_result.html')
    return parser


def convertSarif(text, file):
    sarif = json.loads(text)
    res = {'problems': [], 'code_snippets': {}, 'configurations': {file.replace('\\','/'): text}}
    if sarif.get('runs', []) is None:
        return None
    for r in sarif.get('runs', []):
        for rr in r.get('results', []):
            p = {'file': rr.get('locations', [{}])[0].get('physicalLocation', {}).get('artifactLocation', {}).get('uri',
                                                                                                                  ''),
                 'line': int(
                     rr.get('locations', [{}])[0].get('physicalLocation', {}).get('region', {}).get('startLine', '0')),
                 'offset': int(
                     rr.get('locations', [{}])[0].get('physicalLocation', {}).get('region', {}).get('startColumn', '0')),
                 'length': 1,
                 'description': rr.get('message', {}).get('text', ''),
                 'problem_class': {'name': rr.get('ruleId', ''), 'inspection_name': rr.get('ruleId', ''), 'profile':'All'},
                 'trace': [],
                 'sarif': json.dumps(rr, indent=2)}
            p["unique_id"] = hashlib.md5(json.dumps(rr).encode()).hexdigest()
            for cf in rr.get('codeFlows', []):
                for tf in cf.get('threadFlows', []):
                    for l in tf.get('locations', []):
                        pl = l.get('location', {}).get('physicalLocation', {})
                        reg = pl.get('region', {})
                        sl = int(reg.get('startLine', '0'))
                        sc = int(reg.get('startColumn', '0'))
                        p['trace'].append(
                            {'file': pl.get('artifactLocation', {}).get('uri', ''),
                             'line': sl,
                             'col': sc,
                             'end_line': reg.get('endLine', sl),
                             'end_col': reg.get('endColumn', sc + 1),
                             'desc': l.get('metadata', '')
                             })
            res['problems'].append(p)
    return res


def getJson(file):
    text = getFileText(file)
    res = None
    if file.endswith(".sarif"):
        res = convertSarif(text, file)
    if file.endswith(".html"):
        pos = text.find(headStr) + len(headStr)
        endPos = text.find("</script>", pos)
        text = base64.b64decode(text[pos: endPos])
        text = inflate(text[2:])
        text = text.decode(encoding="utf-8", errors="ignore")
        res = json.loads(text)
    if file.endswith(".json"):
        res = json.loads(text)
    if res is None:
        return None
    res['report_file'] = file
    return res


def packJson(text, newJson):
    pos = text.find(headStr) + len(headStr)
    endPos = text.find("</script>", pos)
    return text[:pos] + pack(json.dumps(newJson, indent=1)) + text[endPos:]


def packCSS(text, files):
    for f in files:
        tempFind = '<link rel="stylesheet" href="' + os.path.relpath(f, jsPath).replace('\\', '/') + '">'
        tempStart = "<script type='text/json' class='packedStyle'>"
        tempEnd = "</script>"
        pos = text.find(tempFind)
        if pos >= 0:
            text = text[:pos] + tempStart + pack(cssmin(getFileText(f))) + tempEnd + text[pos + len(tempFind):]
    return text


def packJS(text, files):
    for f in files:
        tempFind = '<script src="' + os.path.relpath(f, jsPath).replace('\\', '/') + '"></script>'
        tempStart = "<script type='text/json' class='packed'>"
        tempEnd = "</script>"
        pos = text.find(tempFind)
        if pos >= 0:
            text = text[:pos] + tempStart + pack(jsmin(getFileText(f))) + tempEnd + text[pos + len(tempFind):]
    return text


def removeGrid(text):
    removableFiles = ['<script src="Deps/FileSaver.min.js"></script>',
                      '<script src="Deps/tabulator.min.js"></script>',
                      '<script src="Deps/jszip.min.js"></script>',
                      '<link rel="stylesheet" href="Deps/tabulator-simple.min.css">',
                      '<script src="tabulatorInit.js"></script>']
    for f in removableFiles:
        pos = text.find(f)
        text = text[:pos] + text[pos + len(removableFiles):]
    return text


def getTemplate(single):
    text = getFileText(reportTemplatePath)
    if single:
        text = removeGrid(text)
    text = packJS(text, getFiles(jsPath, [".js"], scriptPath))
    text = packCSS(text, getFiles(jsPath, [".css"], scriptPath))
    return text


def convertTrace(report, sources_path):
    if not 'problems' in report:
        return None
    for problem in report["problems"]:
        for event in problem["trace"]:
            if 'snippet_path' not in event or event['snippet_path'] not in report['code_snippets']:
                file = event['file']
                if file not in report['code_snippets']:
                    buff = getFileText(file, sources_path)
                    report['code_snippets'][file] = {
                        'code': buff,
                        'line': 1,
                        'column': 0,
                        'end_line': buff.count('\n') + 2,
                        'end_column': 0,
                        'file': file
                    }
                event['snippet_path'] = file
    return report


def getJsons(files, sources):
    res = []
    for f in getFiles(files, ['.sarif', '.json', '.html']):
        print('Start process: ', f)
        reportData = getJson(f)
        if not reportData is None:
            reportData = convertTrace(reportData, sources)
            if not reportData is None:
                res.append(reportData)
    return res


def joinJsons(jsons):
    res = jsons[0]
    for j in jsons[1:]:
        res['problems'].extend(j['problems'])
        res['code_snippets'].update(j['code_snippets'])
        res['configurations'].update(j['configurations'])
    return res


if __name__ == "__main__":
    args = constructArgParser().parse_args()
    reportData = getJsons(args.report, args.sources_path)
    if args.result.endswith(".html"):
        report = joinJsons(reportData)
        text = getTemplate(len(report["problems"]) < 2)
        text = packJson(text, report)
        with codecs.open(args.result, "w", encoding='utf-8', errors='ignore') as outfile:
            outfile.write(text)
    else:
        for r in reportData:
            text = getTemplate(len(r["problems"]) < 2)
            text = packJson(text, r)
            with codecs.open(makeResultPathWithExt(args.report, r['report_file'], args.result, '.html'), "w",
                             encoding='utf-8', errors='ignore') as outfile:
                outfile.write(text)

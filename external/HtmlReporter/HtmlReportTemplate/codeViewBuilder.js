/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
var collapseCode = getGlobalOption('collapseCode', "false") == "true";
var hasCollapseGroups = false;
var preventCollapseCode = false;
var viewSarif = false;

function getCodeForTracePoint(tp) {
	let codeSnippet = cooddyResults.code_snippets[tp.snippet_path];
	if(!codeSnippet || !codeSnippet.code)
		return;
	let row = codeSnippet.code.split('\n')[tp.line - codeSnippet.line];
	row = [row.slice(0, tp.end_col - 1), '</b>', row.slice(tp.end_col - 1)].join('');
	row = [row.slice(0, tp.col - 1), '<b>', row.slice(tp.col - 1)].join('');
	return "<b>" + tp.line + "&#x29D0;</b> " + row.trim();
}
function appendCode(text) {
	if(text[text.length - 1] != '\n')
		text += '\n';
	var preContainer = ge("preContainer");
	var codeContainer = ce('div');
	codeContainer.innerHTML = text;
	preContainer.appendChild(codeContainer);
}

function CreateCollapsableCodeSnippet(before) {
	if(before.length >= 5)
		hasCollapseGroups = true;
	if(preventCollapseCode || !collapseCode || before.length < 5)
		appendCode(before.join("\n"));
	else {
		appendCode(before[0]);
		appendCode('...\n');
		appendCode(before.last());
	}
}
function createPopup(st, visibleRowNumber, rowNumSize, col, cols) {
    if(st.length == 0) return;
    let cl = visibleRowNumber > 10 && rowNumSize - visibleRowNumber < 11 ? "popupUp" : "popupDown";
    cl += col > 65 ? " popupLeft" : " popupRight";
	let resultRow = '';
    for(let i = st.length - 1; i >=0; i--) {
        let highlight = st[i];
        if(highlight.branch)
            resultRow += highlight.branch.desc + "\n";

        if(highlight.codeLink)
            for(let j = 0; j < highlight.codeLink.length; j++) {
                const cs = cooddyResults.code_snippets[highlight.codeLink[j]];
                if(cs && cs.code.trim() != "#define")
                    resultRow += Prism.highlight(cs.code + "\n", Prism.languages.clike, 'clike');
            }
    }
    if(resultRow.trim())
        return '<table class="variable_popup '+cl+'"><tbody><tr><td valign="top"><div>' + resultRow + '</div></td><td></td></tr></tbody></table>';
	return '';
}

function HighlightRow(needHighlightCode, visibleRowNumber, visibleRowCount, rowNumSize, line, text, textOffset, highlightedLocations, tracePoints, branches) {
	function pad(num, size) {
		num = num.toString();
		while (num.length < size) num = " " + num;
		return num;
	}
	let resultRow = Prism.highlight(pad(line, rowNumSize).toString() + " ", Prism.languages.clike, 'clike');
	let highlights = [];
	highlightedLocations.forEach(([k, v]) => {
		let s = k.split(":").map(Number);
		highlights.push({start:true, col:s[1] != line ? text.search(/\S|$/) : (s[2] - textOffset), codeLink:v });
		highlights.push({start:false, col:s[3] != line ? text.length : (s[4] - textOffset), codeLink:v });

	});
	tracePoints.forEach(t => {
		highlights.push({start:true, tracePoint: true, col:t.line != line ? text.search(/\S|$/) : (t.col - textOffset)});
		highlights.push({start:false, tracePoint: true, col:t.end_line != line ? text.length : (t.end_col - textOffset)});
	});
    branches.forEach(t => {
		highlights.push({start:true, branch:t, col:t.line != line ? text.search(/\S|$/) : (t.col - textOffset)});
		highlights.push({start:false, branch:t, col:t.end_line != line ? text.length : (t.end_col - textOffset)});
	});
	highlights.sort((a, b) => {
		if(a.col == b.col && a.start == b.start) {
            return 0;
        }
		if(a.col == b.col)
			return a.start ? -1 : 1;
		return a.col < b.col ? -1 : 1;
	});
	let prevCol = 0;
	let st = [];
	for(let i = 0; i < highlights.length; i++) {
		let highlight = highlights[i];
		resultRow += Prism.highlight(text.slice(prevCol, highlight.col - 1), Prism.languages.clike, 'clike');
		if(highlight.start) {
            if(st.length > 0) {
                resultRow += createPopup(st, visibleRowNumber, visibleRowCount, highlight.col, text.length);
                resultRow += '</span>';
            }

            st.push(highlight);
            resultRow += '<span class="variable hoverHighlight' +
                            (st.some(s=>s.codeLink) ? ' highlightedCode' : '') +
                            (st.some(s=>s.tracePoint) ? ' highlightedTraceCode' : '') +
                            (st.some(s=>s.branch) ? ' highlightedBranch' : '') +
                            '">';
		} else {
            resultRow += createPopup(st, visibleRowNumber, visibleRowCount, highlight.col, text.length) + '</span>';
            st.pop();
			if(st.length > 0) {
                resultRow += '<span class="variable hoverHighlight' +
                                (st.some(s=>s.codeLink) ? ' highlightedCode' : '') +
                                (st.some(s=>s.tracePoint) ? ' highlightedTraceCode' : '') +
                                (st.some(s=>s.branch) ? ' highlightedBranch' : '') +
                                '">';
			}
		}
		prevCol = highlight.col - 1;
	}
    if(needHighlightCode || visibleRowCount < 1000 || highlights.length > 0)
        resultRow += Prism.highlight(text.slice(prevCol, text.length), Prism.languages.clike, 'clike');
    else
        resultRow += text.slice(prevCol, text.length);
    return resultRow;
}
function CreateCodeSnippet(visibleRowNumber, visibleRowsCount, tracePoints, problem) {
	var preContainer = ge("preContainer");
	var codeSnippet = cooddyResults.code_snippets[tracePoints[0].snippet_path];
    appendCode("<span style='opacity:0.3'>" + codeSnippet.file + "<span/>");
	var text = codeSnippet.code;

	let allRows = text.split('\n');
	let rowNumSize = (codeSnippet.line + allRows.length).toString().length;
	let before = [];
	for(let i = 0; i < allRows.length; i++) {
		let currentTracePoints = tracePoints.filter(t => t.line <= codeSnippet.line + i && t.end_line >= codeSnippet.line + i);
        let branches = problem.trace.filter(t => t.kind=="Branch" && t.line <= codeSnippet.line + i && t.end_line >= codeSnippet.line + i)
		let currentHighlightedLocations = [];
		if(codeSnippet.highlighted_locations)
			currentHighlightedLocations = Object.entries(codeSnippet.highlighted_locations).filter(([k, v]) => {
				let r = k.split(":").map(Number);
				return  r[1] <= codeSnippet.line + i && r[3] >= codeSnippet.line + i;
			});
		let needHighlightCode = tracePoints.some(t => Math.abs(t.end_line - codeSnippet.line - i) < 20);
		let row = HighlightRow(needHighlightCode, visibleRowNumber + i, visibleRowsCount, rowNumSize, codeSnippet.line + i, allRows[i], i == 0 ? codeSnippet.column - 1 : 0, currentHighlightedLocations, currentTracePoints, branches);
		let currentTracePoints2 = tracePoints.filter(t => t.end_line - codeSnippet.line == i);
		if(currentTracePoints2.length > 0) {
			CreateCollapsableCodeSnippet(before);
			before = [];
			let r = ce('div');
			r.innerHTML = row;
			preContainer.appendChild(r);
			for(let j = 0; j < currentTracePoints2.length; j++) {
				let tracePoint = currentTracePoints2[j];
				let desc = ce('div');
                let col = tracePoint.col > 120 ? 120 : tracePoint.col;
                let offset = col - tracePoint.desc.length;
                if (offset < 0) offset = 0;
                offset /= 2;
                let descContent = '<div id="tracePoint' + tracePoint.filteredIndex + '" style="margin-left:' + offset + 'em" class="msg"><table><tr>';
                descContent += '<td><div class="PathIndex">' + (tracePoint.filteredIndex + 1).toString() + '</div></td>';
                if (tracePoint.filteredIndex > 0)
                    descContent += '<td><div class="PathNav"><a href="#tracePoint' + (tracePoint.filteredIndex - 1).toString() + '">&#x2190;</a></div></td>';

                descContent += '<td>' + tracePoint.desc + '</td>';
                if(cooddyResults.taint_man_url && tracePoint.desc.indexOf("Untrusted data source") >= 0)
                   descContent += '<td><a target="_blank" href="' + cooddyResults.taint_man_url + problem.reason.replaceAll(" ", "").toLowerCase() + '"> more </a></td>';
                if (tracePoint.filteredIndex < tracePoint.filteredTraceLength - 1)
                    descContent += '<td><div class="PathNav"><a href="#tracePoint' + (tracePoint.filteredIndex + 1).toString() + '">&#x2192;</a></div></td>';
                descContent += '</tr></table></div>';
                desc.innerHTML = descContent;
                preContainer.appendChild(desc);
            }
		} else {
			before.push(row);
		}
	}
	CreateCollapsableCodeSnippet(before);
    return allRows.length;
}

function GetTraceHeader(problem) {
    let desc = ce('div');
    desc.className = "codeHeader";
    let descContent = "";
    if(problem.trace) {
        descContent += '<b>Bug Summary</b>\n';
        let pfn = problem.file;
        if(pfn.length > 40) pfn = "..."+pfn.slice(-40);
        if(problem.filteredTrace.length > 0)
            descContent += 'Sink:  <span class="PathNavNormalSize" style="text-overflow: ellipsis;direction: rtl;font-size:normal !important"><a id="sinkLink" href="#tracePoint' + (problem.filteredTrace.length - 1).toString() + '">' + pfn + ' line:' + problem.line + ' col:' + problem.offset + '</a></span>\n';
        if(problem.description)
            descContent += '<span>' + problem.description + '</span>\n';
        if(problem.source)
            descContent += 'Source:   <span class="PathNavNormalSize" style="text-overflow: ellipsis;direction: rtl;font-size:normal !important"><a href="#tracePoint0">' + problem.source + '</a></span>\n';
        if(problem.problem_class.name) {
            descContent += 'Inspection name: ' + problem.problem_class.name;
            if(problem.solver_duration) {
                descContent += ' Solver Duration: ' + problem.solver_duration + '\n';
            }
        }
        if(problem.kleeConclusion)
            descContent += 'Klee conclusion: ' + problem.kleeConclusion + '\n';
        if(collapseCode) {
            descContent += '<a href="true" class="expandCollapseCode">Expand all code groups</a>\n';
        } else {
            descContent += '<a href="false" class="expandCollapseCode">Collapse unnecessary code groups</a>\n';
        }
    }
    desc.innerHTML = descContent;
    return desc;
}

function RenderTrace(problem) {
    if(problem.htmlTraceFileContent) {
        let container = ge("codeSnippetContainer");
        container.textContent = '';
        classAdd("headerPane", "invisible");
        const iframe = ce("iframe");
        iframe.width = "100%";
        iframe.height = "100%";
        let t = decode(cooddyResults.traces[problem.htmlTraceFileContent])
        iframe.srcdoc = t;
        container.appendChild(iframe);
    } else {
        RenderTraceFromCodeSnippets(problem);
    }
}
function RenderTraceFromCodeSnippets(problem) {
    showHide("allEvents", problem.trace.some(t => t.kind=="Weakassumption"));
    showHide("hideBranches", problem.trace.some(t => t.kind=="Branch"));
    showHide("traceExpandCollapse", problem.trace.length > 1);
    showHide("rightSidebar", problem.trace.length > 0);
    showHide("resizer2", problem.trace.length > 0);
    showHide("sarifTab", problem.sarif);

    ge('preSarifContainer').textContent = problem.sarif
    ge("preContainer").textContent = '';
    let groups = {};
    let problemTrace = problem.filteredTrace;
    for(let i = 0; i < problemTrace.length; i++) {
        let trace = problemTrace[i];
        if(groups[trace.snippet_path])
            groups[trace.snippet_path].push(trace);
        else
            groups[trace.snippet_path] = [trace];
    }
    let i = 0;
    let lineCount = 0;
    for(let k in groups) {
        let s = cooddyResults.code_snippets[k];
        lineCount += s.end_line - s.line;
    }
    preventCollapseCode = lineCount < 25;
    let curRow = 0
    for(let k in groups) {
        if(i > 0)
            appendCode("...\n...\n");
        i++;
        curRow += CreateCodeSnippet(curRow, lineCount, groups[k], problem);
    }
    appendCode("\n\n\n\n\n\n\n\n");

    function navFunc(currentValue, currentIndex) {
        var hrefValue = currentValue.getAttribute("href");
        currentValue.onclick = function() {
            scrollTo(document.querySelector(hrefValue));
            return false;
        }
    };
    qsa(".PathNav > a", navFunc);
    qsa(".PathNavNormalSize > a", navFunc);
    qsa(".expandCollapseCode",
        function(currentValue, currentIndex) {
            if(preventCollapseCode || !hasCollapseGroups)
                currentValue.classList.add("invisible");
            var hrefValue = currentValue.getAttribute("href") == "true" ? true : false;
            currentValue.onclick = function() {
                collapseCode = !hrefValue;
                setGlobalOption("collapseCode", collapseCode);
                loadReport(lastLoadedProblem);
                return false;
            };
        });
}
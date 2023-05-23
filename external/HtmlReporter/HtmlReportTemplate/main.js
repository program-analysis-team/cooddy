/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
createBody();
var conclusionCB = ge("conclusionCB");
var rhsServer = undefined;
var binary_string = ge("cooddyResults").text;
var cooddyResults = {};
if(binary_string.startsWith('e')) 
    cooddyResults = JSON.parse(decode(binary_string));
else
    cooddyResults = JSON.parse(binary_string);
var allEvents = getGlobalOption('allEvents', "false") == "true";
if(allEvents) classAdd("allEvents", "menuItemActive");
var hideBranches = getGlobalOption('hideBranches', "true") == "true";
if(!hideBranches) classAdd("hideBranches", "menuItemActive");
var traceExpanded = getGlobalOption('traceExpanded', "true") == "true";


for(let p of cooddyResults.problems) {
    if(!p.source) p.source = "";
    if(!p.file) p.file = "nofilename";
    if(!p.conclusion) p.conclusion = "";
    if(!p.comment) p.comment = "";
    if(!p.problem_class) p.problem_class = {};
    if(!p.problem_class.inspection_name) p.problem_class.inspection_name = " ";
    if(!p.problem_class.profile) p.problem_class.profile = " ";
}

SetupResizer();

var fullPathFileName = document.location.href.split(".html")[0];
var fileName = fullPathFileName.replace('\\','/').split('/').last();
cooddyResults.problems.forEach(p => p.linkForCSVExport = '=HYPERLINK("' + fullPathFileName + '.html?currentId=' + p.unique_id + '")');

var lastLoadedProblem;
qsa(".menuItem", s => 
    s.onclick = e => {
        qsa(".menuItem", s => s.classList.remove('menuItemActive'));
        qsa(".tab", s => s.classList.remove('tabActive'));
        e.currentTarget.classList.add('menuItemActive');
        e.currentTarget.attributes["data-internalid"].value.split(" ").forEach(e=> classAdd(e, 'tabActive'));
        setUrlParam('view', e.currentTarget.id, "codeSnippetTab");
        loadReport(lastLoadedProblem);
    })
var fileNameId = fileName.slice(0, 32);
problem = cooddyResults.problems.find(p => p.unique_id == fileNameId);
if(problem)
    loadReport(problem);
else
    fileNameId = undefined;
const problemIdParam = getUrlParam('problemId');
if(problemIdParam != undefined) {
    qsa(".leftContainer", s => s.classList.add('invisible'));
    loadReport(cooddyResults.problems.filter(x=>x.unique_id == problemIdParam)[0]);
    ge("codeSnippetTab").click();
}

const viewParam = getUrlParam('view');
if(viewParam)
    ge(viewParam).click();
else
    ge("codeSnippetTab").click();
function InvalidateComments(problem) {
    if(!problem)
        return;
    if(problem.conclusions && problem.conclusions.length > 0) {
        ge("commentsPane").classList.remove("invisible");
        ge("comments").value = problem.conclusions[0].comment ? problem.conclusions[0].comment : "";
        conclusionCB.value = problem.conclusion;
    } else {
        ge("comments").value = "";
        classAdd("commentsPane", "invisible");
    }
}

function conclusionChanged(problem) {
    if(!problem.conclusions[0].comment) {
        problem.conclusions = [{comment:"", created:Date.now()}].concat(problem.conclusions);
    }
    problem.conclusions[0].created = Date.now();
    problem.conclusions[0].comment = problem.comment;
    let v = problem.conclusion;
    let newStatus = {user_id:'', created:Date.now(), status:v};
    if(problem.conclusions) {
        if(problem.conclusions.length > 0 && newStatus.created - problem.conclusions.last().created < 60000) {
            problem.conclusions.pop();
        }
        if(problem.conclusions.length > 8) {
            problem.conclusions.splice(1, problem.conclusions.length - 8);
        }
        problem.conclusions.push(newStatus);
    } else 
        problem.conclusions = [newStatus];
    SendChanges(problem);
    if(typeof Tabulator != "undefined")
        updateStats();
    InvalidateComments(problem);
}
ge("conclusionCB").onchange = function() {
    lastLoadedProblem.conclusion = conclusionCB.value;
    conclusionChanged(lastLoadedProblem);
}

if(cooddyResults.git_commit || cooddyResults.configurations) {
    classRemove("cooddySettingsTab", "invisible");
    let desc = ce('div');
    desc.className = "codeHeader";
    let descContent = "";
    if(cooddyResults.command_line) 
        descContent += '<span>Command line: ' + cooddyResults.command_line + '</span>\n';
    if(cooddyResults.prev_command_line)
        descContent += '<span style>Previous command line: ' + cooddyResults.prev_command_line + '</span>\n';
    if(cooddyResults.git_version) 
        descContent += '<span>Cooddy version: ' + cooddyResults.git_version + ' from commit ' + cooddyResults.git_commit + '</span>\n';
    if(cooddyResults.start_time) 
        descContent += '<span>Start time: ' + cooddyResults.start_time + '</span>\n';
    if(cooddyResults.end_time) 
        descContent += '<span>End time: ' + cooddyResults.end_time + '</span>\n';
    if(cooddyResults.project_git_summary)
        descContent += '<span>Project git head: ' + cooddyResults.project_git_summary + '</span>\n';
    if(cooddyResults.configurations) {
        descContent += '<div class="menu wrappedMenu">';
        let first = 0;
        for(const [fn, t] of Object.entries(cooddyResults.configurations)) {
            descContent += '<div class="submenuItem ' + (t=="Default" ? "submenuItemGreen " : "submenuItemRed ")
                         + (first == 0 ? "submenuItemActive" : "")
                         + '" data-internalid="' + fn + '" id="' + fn + '" title="' + fn + '">' + fn.split('/').last() + '</div>';
            if(first == 0)
                SetPreContent("preSettingsContainer", t);
            first++;
        }
        descContent += '</div>';
    }

    desc.innerHTML = descContent;
    SetElementContent("cooddySettingsHeader", desc);
}
qsa(".submenuItem", s => {
    s.onclick = function(e) {
        qsa(".submenuItem", s => s.classList.remove('submenuItemActive'));
        e.currentTarget.classList.add('submenuItemActive');
        SetPreContent("preSettingsContainer", cooddyResults.configurations[e.currentTarget.attributes["data-internalid"].value]);
}});
function filterTracePoints(problem) {
    let m = {}
    for(let i = 0; i < problem.trace.length; i++) {
        if(problem.trace[i].parentEventId == undefined) problem.trace[i].parentEventId = -1;
        problem.trace[i].index = i;
        problem.trace[i].filteredParentEvent = problem.trace[i].parentEventId;
        m[problem.trace[i].id] = problem.trace[i];
    }
    problem.filteredTrace = problem.trace.filter((t, i) => {
        if(i > 0 && t.desc == "Assume return here" && problem.trace[i-1].line == t.line)
            return false;
        if(i < problem.trace.length - 1) {
            let pt = problem.trace[i+1];
            if(t.kind == "Branch" && pt.kind == "Branch" && t.line == pt.line && t.col == pt.col)
                return false;
        }
        let res = (!hideBranches || t.kind!="Branch") &&
                  (allEvents || (t.kind!="Weakassumption" && (t.parentEventId < 0 || (!m[t.parentEventId] || m[t.parentEventId].kind != "Weakassumption"))));
        if(!res) {
            problem.trace.forEach(tt => {
                if(tt.filteredParentEvent == t.id) tt.filteredParentEvent = t.filteredParentEvent;
            });
        }
        return res;
    });
    for(let i = 0; i < problem.filteredTrace.length; i++) {
        if(!m[problem.filteredTrace[i].filteredParentEvent]) problem.filteredTrace[i].filteredParentEvent = -1;
        problem.filteredTrace[i].filteredIndex = i;
        problem.filteredTrace[i].filteredTraceLength = problem.filteredTrace.length;
    }
}
function loadReport(problem) {
    if(!problem || (!problem.trace && !problem.htmlTraceFileContent))
        return;
    hasCollapseGroups = false;
    lastLoadedProblem = problem;
    filterTracePoints(problem);
    setUrlParam("currentId", problem.unique_id);
    InvalidateComments(problem);
    SetElementContent("headerPane", GetTraceHeader(problem));
    RenderTrace(problem);
    if(problem.trace) {
        let desc = ce('div');
        desc.className = "";
        descContent = "";
        let createNode = (i, trace) => {
            let t = trace[i];
            let c = trace.filter(a=>a.filteredParentEvent == t.id);
            descContent += '<div><table><tr>';
            if(c.length == 0)
                descContent += '<td class="tpcollapseEmpty">&nbsp</td>';
            else
                descContent += '<td class="tpcollapse" data-id="' + i + '">' + (traceExpanded ? '-' : '+') + '</td>';
            switch(t.kind) {
                case "Branch":
                    descContent += '<td class="tpbranch">&#9670;</td>';
                    break;
                case "Assumption":
                    descContent += '<td class="tpcode">&#9656</td>';
                    break;
                case "Weakassumption":
                    descContent += '<td class="tpassume"> </td>';
                    break;
                default:
                    descContent += '<td class="tpcode">&#9632;</td>';
            }
            descContent += '<td class="graphTracePoint" id="gtracePoint' + i + '" data-id="' + i + '">' + t.file.split("/").last() + ':' + t.line + ' ' + t.desc + '</td></tr>';
            if(c.length != 0) {
                descContent += '<tr class="'+(traceExpanded ? '' : 'invisible')+'" id="tpgroup'+i+'"><td></td><td></td><td>';
                for(let v of c) {
                    createNode(v.filteredIndex, trace);
                }
                descContent += '</td></tr>';
            }
            descContent += '</table></div>';  
        }
        let trace = problem.filteredTrace;
        
        for(let i = 0; i < trace.length; i++) {
            let t = trace[i];
            if(t.filteredParentEvent == -1 || t.filteredParentEvent == undefined)
                createNode(i, trace);
        }
        desc.innerHTML = descContent;
        SetElementContent("preCodeGraphContainer", desc);
        qsa(".tpcollapse", s => {
            s.onclick = function(e) {
                let id = e.currentTarget.attributes["data-id"].value;
                if(e.currentTarget.textContent == '+') {
                    e.currentTarget.textContent = '-';
                    classRemove("tpgroup" + id, 'invisible');
                } else {
                    e.currentTarget.textContent = '+';
                    classAdd("tpgroup" + id, 'invisible');
                }
            }
        });
        qsa(".graphTracePoint", s => {
            s.onclick = function(e) {
                let id = e.currentTarget.attributes["data-id"].value;
                scrollTo(document.querySelector("#tracePoint" + id));
                return false;
        }});
    } else {
        classAdd("traceTab", "invisible");
    }
    ge("sinkLink")?.click();
}
if(typeof Tabulator != "undefined" && problemIdParam == undefined) {
    tabulatorInit();
} else {
    classAdd("download-csv", "invisible");
    classAdd("filterBtn", "invisible");
    classAdd("groupBtn", "invisible");
    classAdd("fSearch", "invisible");
    
    let grid = ge("dataGrid");
    grid.classList.add("gridStyle");
    grid.style.height = window.innerHeight - 60;
    let d = "";
    let first = true;
    let i = 0;
    for(let p of cooddyResults.problems) {
        d += "<div class='gridRow " + (first ? "gridRowSelected" : "" ) + "' data-id='" + (i++) + "'>" + p.file + ":" + p.line + ":" + p.offset + "</div>";
        first = false;
    }
    grid.innerHTML = d;
    qsa(".gridRow", r => {
        r.onclick = e => {
            qsa(".gridRowSelected", rr => rr.classList.remove("gridRowSelected"));
            e.currentTarget.classList.add('gridRowSelected');
            loadReport(cooddyResults.problems[e.currentTarget.attributes["data-id"].value]);
        }
    })
    if(cooddyResults.problems.length == 1) {
        classAdd("sidebar", "invisible");
        classAdd("resizer", "invisible");
    }
    loadReport(cooddyResults.problems[0]);
}
ge("traceExpandCollapse").innerHTML = String.fromCharCode(traceExpanded ? 8648 : 8650);
ge("traceExpandCollapse").onclick = e => {
    traceExpanded  = !traceExpanded;
    setGlobalOption("traceExpanded", traceExpanded ? "true" : "false");
    ge("traceExpandCollapse").innerHTML = String.fromCharCode(traceExpanded ? 8648 : 8650);
    qsa(".tpcollapse", s => {
        if((s.textContent == '+' && traceExpanded) || (s.textContent == '-' && !traceExpanded))
            s.click();
    });
    
};
function SendChanges(problem) {
    let p  =fetch(rhsServer+'?problem_id=' + problem.unique_id, {method:'POST',headers: new Headers({'Content-Type': 'application/json'}),body: JSON.stringify(problem.conclusions)})
    p.then((response) => {
        if (response.status === 200 || response.status === 0)
            qsa(".connectionProblem", s => s.classList.add("invisible"));
        else
            qsa(".connectionProblem", s => s.classList.remove("invisible"));
    }).catch(()=>qsa(".connectionProblem", s => s.classList.remove("invisible")))
    return p;
}
if(cooddyResults.reporter == "secb") {
    let i = 0;
    for(let p of cooddyResults.problems) {
        if(!p.comment) p.comment = " ";
        p.kleeConclusion = p.comment.startsWith("KLEE:") ? p.comment.slice(5) : p.comment;
        p.kleeConclusion = p.conclusion + " " + p.kleeConclusion;
        p.comment = "";
        p.conclusions = [];
        p.conclusions.push({comment:p.comment});
        p.conclusions.push({status:p.conclusion, created:new Date()});
    }
    var opt = ce('option');
    opt.value = "IV";
    opt.innerHTML = "Invalid trace";
    conclusionCB.appendChild(opt);
    opt = ce('option');
    opt.value = "NF";
    opt.innerHTML = "Not found";
    conclusionCB.appendChild(opt);
    
    classAdd("filterSourceRow", "invisible");
    classAdd("filterSourceRow2", "invisible");
    classAdd("filterSourceHeader", "invisible");
    classRemove("filterSourceSecb", "invisible");
    InvalidateComments(lastLoadedProblem);
}
ge("comments").onchange = function(){
    if(!lastLoadedProblem.conclusions[0].comment) {
        lastLoadedProblem.conclusions = [{comment:"", created:Date.now()}].concat(lastLoadedProblem.conclusions);
    }
    lastLoadedProblem.comment = lastLoadedProblem.conclusions[0].comment = ge("comments").value;
    lastLoadedProblem.conclusions[0].created = Date.now();
    SendChanges(lastLoadedProblem);
}
qsa(".allEvents",
    function(currentValue, _) {
        currentValue.onclick = function() {
            allEvents = !allEvents;
            if(!allEvents) classRemove("allEvents", "menuItemActive");
            else classAdd("allEvents", "menuItemActive");
            setGlobalOption("allEvents", allEvents);
            loadReport(lastLoadedProblem);
            return false;
        };
    });
qsa(".hideBranches",
    function(currentValue, _) {
        currentValue.onclick = function() {
            hideBranches = !hideBranches;
            if(hideBranches) classRemove("hideBranches", "menuItemActive");
            else classAdd("hideBranches", "menuItemActive")
            setGlobalOption("hideBranches", hideBranches);
            loadReport(lastLoadedProblem);
            return false;
        };
    });
let ids = cooddyResults.problems.map(p=>"'"+p.unique_id+"'").join(',');

async function fetchWithTimeout(resource, options = {}) {
  const { timeout = 2000 } = options;
  
  const controller = new AbortController();
  const id = setTimeout(() => controller.abort(), timeout);
  const response = await fetch(resource, {
    ...options,
    signal: controller.signal  
  });
  clearTimeout(id);
  return response;
}

let rhsServersParam = getUrlParam("rhs-servers");
if(rhsServersParam)
    rhsServersParam  = rhsServersParam.split(",");

rhsServers =  rhsServersParam ?? rhsServers;
if(rhsServers.length > 0) {
    var rhsPromises = []
    rhsServers.forEach(s => {
        let p = fetchWithTimeout(s, { method:'POST', headers: new Headers({'Content-Type': 'application/json'}), body: '[' + ids + ']'})
        p.then(() => rhsServer = s).catch(err => {});
        rhsPromises.push(p);
    });
    function ConnectionError(err) {
        if(typeof Tabulator != "undefined") {
            table.hideColumn("conclusion");
            table.hideColumn("comment");
        }
        qsa(".connectionProblem", s => s.classList.remove("invisible"));
        console.log(err);
    }

    Promise.any(rhsPromises).then(()=>{
        fetch(rhsServer, { method:'POST', headers: new Headers({'Content-Type': 'application/json'}), body: '[' + ids + ']'})
        .then(response => response.json())
        .then((response) => {
            qsa(".connectionProblem", s => s.classList.add("invisible"));
            let r = {};
            for(let resp of response) {
                r[resp.problem_id] = JSON.parse(resp.data);
            }
            for(let p of cooddyResults.problems) {
                p.conclusions = r[p.unique_id] ?? p.conclusions ?? [];
                if(p.conclusions.length > 0) {
                    p.conclusion = p.conclusions.last().status;
                    p.comment = p.conclusions[0].comment;
                } else 
                    p.conclusion = undefined;
                if(p.conclusions.length == 0 || !p.conclusions[0].comment) {
                    p.conclusions = [{comment:"", created:Date.now()}].concat(p.conclusions);
                    p.comment = "";
                }
            }
            if(typeof Tabulator != "undefined")
                updateStats();
            InvalidateComments(lastLoadedProblem);
        }).catch(err => ConnectionError(err))
    }).catch(err => ConnectionError(err));
}
function createBody() {
    var content = ge("mainContainer");
    var d = ce("div");
    d.classList.add("table");
    d.classList.add("stop-scrolling");
    d.innerHTML = `
	<div class="table stop-scrolling">
		<div class="leftContainer" id="sidebar"><div id="dataGrid"></div></div>
		<div class="resizer" id="resizer"></div>
		<div class="rightContainerParent">
            <div class="rightContainer" id="rightContainer">
                <div class="menu">
                    <div class="menuItem invisible" data-internalid="cooddySettings" id="cooddySettingsTab">Settings</div>
                    <div class="menuItem" data-internalid="codeSnippet resizer2 rightSidebar" id="codeSnippetTab">&#9656 Report</div>
                    <div class="menuItem" data-internalid="sarif" id="sarifTab">Sarif</div>
                    <div class="menuItem" data-internalid="about" id="aboutTab">?</div>
                    <div class="connectionProblem invisible">Cannot connect to RHS</div>
                    <div id="debugInfo"></div>
                </div>
                <div class="tab tabStyled" id="codeSnippet" >
                    <div class="reportHeader">
                        <div class="headerPane" id="headerPane">
                        </div>
                    </div>
                    <div class="table" id="codeSnippetContainer">
                        <div class="codeSnippet">
                            <pre class="language-clike" id="preContainer"></pre>
                        </div>

                    </div>
                </div>

                <div class="tab tabStyled" id="cooddySettings" >
                    <div class="reportHeader" id="cooddySettingsHeader">
                    </div>
                    <div class="codeSnippet" id="cooddySettingsContainer">
                        <pre class="language-clike" id="preSettingsContainer"><code id="settingsContainer"></code></pre>
                    </div>
                </div>
                <div class="tab tabStyled" id="sarif" >
                    <div class="reportHeader" id="sarifHeader">
                    </div>
                    <div class="codeSnippet" id="sarifContainer">
                        <pre class="language-clike" id="preSarifContainer"><code id="sarifCodeContainer"></code></pre>
                    </div>
                </div>
                <div class="tab tabStyled" id="about" >
                    <div class="reportHeader" id="aboutHeader">
                    </div>
                    <div class="codeSnippet" id="aboutContainer">
                        <pre class="language-text" id="preAboutContainer">
Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.<br>
<br>
This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.<br>
For full terms see&nbsp;<a href="https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt">license</a>.<br>
<br>
Includes the following libraries:<br>
<br>
FileSaver.js Copyright (C) 2016 Eli Grey. Released under the MIT&nbsp;<a href="https://github.com/eligrey/FileSaver.js/blob/master/LICENSE.md">license</a>.<br>
JSZip Copyright (C) 2009-2016 Stuart Knightley <stuart [at] stuartk.com> Dual licenced under the MIT license or GPLv3. See&nbsp;<a href="https://raw.github.com/Stuk/jszip/main/LICENSE.markdown">license</a>.<br>
JSZip uses the library pako released under the MIT&nbsp;<a href="https://github.com/nodeca/pako/blob/master/LICENSE">license</a>.<br>
Prism.js Copyright (C) 2012 Lea Verou released under the MIT&nbsp;<a href="https://github.com/PrismJS/prism/blob/master/LICENSE">license</a>.<br>
Tabulator.js Copyright (C) 2015-2023 Oli Folkerd. Released under the MIT&nbsp;<a href="https://tabulator.info/docs/5.4/license">license</a>.<br>
tiny-inflate Copyright (C) 2015-present Devon Govett. Released under the MIT&nbsp;<a href="https://github.com/foliojs/tiny-inflate/blob/master/LICENSE">license</a>.<br>
                        </pre>
                    </div>
                </div>
            </div>
            <div class="tab resizer" id="resizer2"></div>
            <div class="tab rightSidebar" id="rightSidebar">
                <div class="menu">
                    <div class="menuItem2" id="traceExpandCollapse" title="Expand/Collapse all items in trace tree">&#8650</div>
                    <div class="menuItem2 hideBranches" id="hideBranches">Branches</div>
                    <div class="menuItem2 allEvents" id="allEvents">Calls</div>
                </div>
                <div class="graphContainer" id="codeGraphContainer">
                    <div id="preCodeGraphContainer"></div>
                </div>
                <div class="commentsPane invisible" id="commentsPane">
                    <div class="headerCommentsPane">
                        <div class="commentsHeader">Conclusion:</div>
                        <select name="conclusion" id="conclusionCB" class="conclusionComboBox">
                            <option value="TP">True positive</option>
                            <option value="FP">False positive</option>
                            <option value="QS">Questionable</option>
                            <option value="UN">Undefined</option>
                        </select> 
                    </div>                        
                    <textarea id="comments" name="comments" maxlength="1024" placeholder="Leave a comment here."></textarea>
                </div>
            </div>
        </div>
	</div>`;
    content.insertBefore(d, content.firstChild);
}
/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
var table;
var filter = {s: true, u:true, p:true, c:true, UN:true, TP:true, FP:true, QS:true, NF:true, IV:true};
var filterCheckers = new Set();
var sorterType=getGlobalOption("sorterType", "sort_IT");
var currentGrouping = ["problem_class.profile", "problem_class.inspection_name"];
var projectId = JSON.stringify(document.location.href.split("?")[0]).replace(/\W/g, '');
let groupStats = {}
function getDescendantProp(obj, desc) {
   var arr = desc.split(".");
   while(arr.length && (obj = obj[arr.shift()]));
   return obj;
}
async function allProgress(proms, progress_cb) {
    let d = 0;
    progress_cb(0);
    for (const p of proms) {
        p.then(()=> {    
            d ++;
            progress_cb( d );
        });
    }
    try {
    while (proms.length) {
        await Promise.all(proms.splice(0, 10))
    }
    } catch {
    }
}

function tabulatorInit() {
    createGridMenu();
    var modal = ge("myModal");
    ge("closeModal").onclick = () => {
        if(!butchUpdateProgress)
            modal.style.display = "none";
    }
    window.onclick = (e) => {
        if (e.target == modal && !butchUpdateProgress)
            modal.style.display = "none";
    }
    var butchUpdateItems = []
    var butchUpdateProgress = false;
    var butchUpdateNewStatus = "QS";
    ge("startBtn").onclick = async () => {
        ge("startBtn").classList.add('invisible')
        ge("closeModal").classList.add('invisible')
        butchUpdateProgress = true;
        try {
            await allProgress(butchUpdateItems.map(i=> {
                if(i.comment && (i.comment.startsWith("Bulk status change to UN") || i.comment.startsWith("Bulk status change to QS")))
                    i.comment = i.comment.slice(26)
                i.comment = "Bulk status change to " + butchUpdateNewStatus + ".\n" + (i.comment??"")
                i.conclusions = [{comment:i.comment ?? "", created:Date.now()}, {user_id:'', created:Date.now(), status:butchUpdateNewStatus}]
                i.conclusion = butchUpdateNewStatus
                return SendChanges(i)
            }), (p)=> {
                ge("modalContent").innerHTML = `Updated ${p}/${butchUpdateItems.length} conclusions`
            })
        } catch {}
        InvalidateComments(lastLoadedProblem);
        modal.style.display = "none";
        butchUpdateProgress = false;
    }
    var butchUpdateConclusion = (oldStatus, newStatus, group) => {
        butchUpdateItems = []
        butchUpdateNewStatus = newStatus;
        modal.style.display = "block";
        ge("startBtn").classList.remove('invisible')
        ge("closeModal").classList.remove('invisible')
        butchUpdateProgress = false;
        let it = (group)=>{
            for(const g of group.getSubGroups())
                it(g);
            for(const r of group._group.rows) {
                if(!r.data) continue;
                if((r.data.conclusion == undefined && oldStatus == "UN") || r.data.conclusion == oldStatus) {
                    butchUpdateItems.push(r.data)
                }
            }
        }
        it(group)
        ge("modalContent").innerHTML = "You want to change conclusion for group " + group.getKey() + " from " + oldStatus + " to " + newStatus + " for " + butchUpdateItems.length + " defects?"
    }
    ge("mainContainer").height = () => document.height() - 60;

    //define column header menu as column visibility toggle
    var headerMenu = () => {
        var menu = [];
        for (let column of this.getColumns()) {
            let label = document.createElement("span");
            let title = document.createElement("span");
            title.textContent = " " + column.getDefinition().title;
            label.appendChild(title);
            label.setAttribute("style", column.isVisible() ? "color:black;" : "color:grey;");
            menu.push({
                label: label,
                action: function (e) {
                    e.stopPropagation();
                    column.toggle();
                    label.setAttribute("style", column.isVisible() ? "color:black;" : "color:grey;");
                }
            });
        }
        return menu;
    };
    function GetColorByType(label) {
        return color = label == "UN" ? "#000" : 
                        label == "TP" ? "#080" :
                        label == "FP" ? "#f00" :
                        label == "NF" ? "#880" :
                        label == "IV" ? "#088" :
                        "#888";
    }
    function sorterFunc(aRow, bRow, par){
        const a = getDescendantProp(aRow.getData(),par);
        const b = getDescendantProp(bRow.getData(),par);
        if(a == b) return 0;
        if(!a) return -1;
        if(!b) return 1;
        let curA = aRow.getData();
        let curGrA = groupStats;
        for(let gr of currentGrouping) {
            curGrA = curGrA[getDescendantProp(curA,gr)]
            if(gr == par) break;
        }
        let curB = bRow.getData();
        let curGrB = groupStats;
        for(let gr of currentGrouping) {
            curGrB = curGrB[getDescendantProp(curB,gr)]
            if(gr == par) break;
        }
        return curGrA.count - curGrB.count;
    }
    function escapeHtml(unsafe) {
        return unsafe
             .replace(/&/g, "&amp;")
             .replace(/</g, "&lt;")
             .replace(/>/g, "&gt;")
             .replace(/"/g, "&quot;")
             .replace(/'/g, "&#039;");
     }
    //create table
    table = new Tabulator("#dataGrid", {
        sortOrderReverse:true,
        reactiveData:true,
        data: cooddyResults.problems,
        autoResize: true, // prevent auto resizing of table
        //autoColumns:true,
        resizableColumnFit: true,
        resizableRows: false,
        height: window.innerHeight - 60,
        selectable: 1,
        selectablePersistence: false,
        layout: "fitColumns",
        responsiveLayout:true,
        movableRows: false,
        groupToggleElement: "header",
        groupStartOpen: true,
        // tooltipsHeader:true,
        index:"unique_id",
        // responsiveLayout:"hide",
        // virtualDom:false, //disable virtual DOM rendering
        columns: [
            {title: "Conclusion", field: "conclusion", visible: rhsServers.length > 0, editor:"list", responsive:0, minWidth:50, width:50, widthGrow:1, download:true,
                editorParams:{
                    values:cooddyResults.reporter == "secb" ? ["UN", "TP", "FP", "QS", "NF", "IV"] : ["UN", "TP", "FP", "QS"],
                    itemFormatter:function(label, value, item, element){
                        return "<strong style='color: " + GetColorByType(label) + "'>" + label + " </strong>";
                    }
                },
                formatter:function(cell, formatterParams, onRendered){
                    let d = cell.getData().conclusion ?? "UN";
                    cell.getRow().getElement().style['opacity'] = cell.getData().status == "Gone" ? "0.6" : "1";
                    if(cell.getData().kleeConclusion)
                        cell.getRow().getElement().style['background'] = cell.getData().conclusion != cell.getData().kleeConclusion.slice(0,2) ? "#F001" : "#0000";
                    return "<strong style='color: " + GetColorByType(d) + "'>" + d + " </strong>";
                },
                tooltip:function(e, cell, onRendered){
                    if(cell.getData().conclusions && cell.getData().conclusions.length > 0) {
                        let r = '';
                        for(let i = 0; i < cell.getData().conclusions.length; i++) {
                            let d = cell.getData().conclusions[i].status;
                            if(!d)
                                continue;
                            r = new Date(cell.getData().conclusions[i].created).toLocaleString() + " : <strong style='color: " + GetColorByType(d) + "'>" + d + " </strong><br/>" + r;
                        }
                        var el = document.createElement("div");
                        el.style.backgroundColor = "yellow";
                        el.style.margin = "5";
                        el.innerHTML = r;
                        return el; 
                    }
                    return "Unknown status";
                },
                sorter:function(a, b, aRow, bRow, column, dir, sorterParams){
                    if(a == b) return 0;
                    if(!a) return -1;
                    if(!b) return 1;
                    const so = {"UN":0,"QS":1,"FP":2,"TP":3,"NF":4,"IV":5}
                    return so[a] - so[b];
                }
            },
            {title: "Comment", field: "comment", visible: rhsServers.length > 0, editor: "textarea", formatter:"textarea", headerMenu: headerMenu, tooltip: true, download:true, responsive:cooddyResults.reporter != "secb" ? 4 : 0 , minWidth:100, widthGrow:2},
            {title: "KLEE", field: "kleeConclusion", visible: cooddyResults.reporter == "secb", headerMenu: headerMenu, tooltip: true, download:true, responsive:cooddyResults.reporter != "secb" ? 4 : 0 , minWidth:100, widthGrow:2},
            {title: "Path", field: "path", visible: false, headerMenu: headerMenu, tooltip: true, download:true},
            {title: "File", field: "file", visible: true, headerMenu: headerMenu, tooltip: true, responsive:0, minWidth:200, widthGrow:3, download:true,
                formatter:function(cell, formatterParams, onRendered){
                    let d = cell.getData();
                    cell.getElement().style['direction'] = "rtl";
                    if(cell.getValue()) {
                        if(d.line && d.offset)
                            return '<bdo dir="ltr">' + escapeHtml(cell.getValue()) + ":" + d.line + ":" + d.offset + '</bdo>';
                        return cell.getValue();
                    }
                    if(d.line && d.offset)
                        return "line: " + d.line + " col: " + d.offset;
                    return "";
                },
                tooltip:function(e, cell, onRendered){
                    if(cell.getData().status == "Blink") {
                        let r = '';
                        let b = [];
                        if(cell.getData().blinkDate)
                            cell.getData().blinkDate.forEach(d => b.push({date: d, blink: true}));
                        if(cell.getData().goneDate)
                            cell.getData().goneDate.forEach(d => b.push({date: d, blink: false}));
                        b.sort((a,b) => a.date == b.date ? 0 : (a.date < b.date ? -1 : 1));
                        for(let i = 0; i < b.length; i++) {
                            r = (b[i].blink ? "Blink: " : "Gone: ") + new Date(b[i].date).toLocaleString() + "<br/>" + r;
                        }
                        var el = document.createElement("div");
                        el.style.backgroundColor = "yellow";
                        el.style.margin = "5";
                        el.innerHTML = r;
                        return el; 
                    }
                    return cell.getData().file;
                },
                sorter:function(a, b, aRow, bRow, column, dir, sorterParams){
                    return sorterFunc(aRow, bRow, "file");
                }
            },
            {title: "Line", field: "line", visible: false, download: true},
            {title: "Offset", field: "offset", visible: false, download: true},
            {title: "Profile", field: "problem_class.profile", sorter:"string", visible: cooddyResults.reporter != "secb", headerMenu: headerMenu, tooltip: true, responsive:6, minWidth:70, widthGrow:1, download:true,
                sorter:function(a, b, aRow, bRow, column, dir, sorterParams){
                    return sorterFunc(aRow, bRow, "problem_class.profile");
                }
            },
            {title: "Checker", field: "problem_class.inspection_name", sorter:"string", visible: true, headerMenu: headerMenu, tooltip: true, responsive:2, minWidth:200, widthGrow:1, download:true,
                sorter:function(a, b, aRow, bRow, column, dir, sorterParams){
                    return sorterFunc(aRow, bRow, "problem_class.inspection_name");
                }
            },
            {title: "Rule", field: "problem_class.name", visible: cooddyResults.reporter != "secb", headerMenu: headerMenu, tooltip: true, responsive:4, minWidth:150, widthGrow:1, download:true},
            {title: "Source", field: "source", visible: cooddyResults.reporter != "secb", headerMenu: headerMenu, tooltip: true, responsive:1, minWidth:200, widthGrow:1, download:true,
                sorter:function(a, b, aRow, bRow, column, dir, sorterParams){
                    return sorterFunc(aRow, bRow, "source");
                }
            },
            {title: "Reason", field: "reason", visible: cooddyResults.reporter != "secb", headerMenu: headerMenu, tooltip: true, responsive:3, minWidth:150, widthGrow:1, download:true},
            {title: "UniqueId", field: "unique_id", visible: cooddyResults.reporter != "secb", headerMenu: headerMenu, tooltip: true, responsive:5, minWidth:250, widthGrow:1, download:true},
            {title: "Severity", field: "problem_class.severity", visible: cooddyResults.reporter != "secb", headerMenu: headerMenu, tooltip: true, responsive:4, minWidth:70, widthGrow:1, download:true},
            {title: "Trace", field: "linkForCSVExport", visible: false, download:true},
            {title: "Solver duration", field: "solver_duration", visible: cooddyResults.reporter != "secb", responsive:4, minWidth:40, widthGrow:1, download:true},
            {title:"Status", field:"status", visible:false,
                sorter:function(a, b, aRow, bRow, column, dir, sorterParams){
                    return sorterFunc(aRow, bRow, "status");
                }
            }
        ],
        headerSortElement: function(column, dir){
            switch(dir){
                case "asc":
                    return "&#11205;";
                break;
                case "desc":
                    return "&#11206;";
                break;
                default:
                    return "&#11201;";
            }
        },
        downloadConfig:{
            columnHeaders:true,
            columnCalcs:true,
        },
        groupContextMenu:[
            {
                label:"Mark all UN as QS in group",
                action:function(e, group) {butchUpdateConclusion("UN", "QS", group)}
            },
            {
                label:"Mark all QS as UN in group",
                action:function(e, group) {butchUpdateConclusion("QS", "UN", group)}
            },
        ],
        groupHeader:function(value, count, data, group){
            let tp=0, fp=0, un=0, qs=0, nf=0, iv=0, fo=false;
            let it = (group)=>{
                for(const g of group.getSubGroups())
                    it(g);
                for(const r of group._group.rows) {
                    if(!r.data) continue;
                    if(r.data.conclusion) fo = true;
                    switch(r.data.conclusion) {
                        case "TP":tp++; break;
                        case "FP":fp++; break;
                        case "QS":qs++; break;
                        case "NF":nf++; break;
                        case "IV":iv++; break;
                        default:un++; break;
                    }
                }
            }
            it(group);
            let r = escapeHtml(value) + "<span>(" + count + (sorterType=="sort_IT" ? "&#11206;" : "") + " items";
            if(fo) {
                if(un > 0) r += "<span style='color:black;'>un</span>:" + un + (sorterType=="sort_UN" ? "&#11206;" : "");
                if(tp > 0) r += "<span style='color:green;'>tp</span>:" + tp + (sorterType=="sort_TP" ? "&#11206;" : "");
                if(fp > 0) r += "<span style='color:red;'>fp</span>:" + fp + (sorterType=="sort_FP" ? "&#11206;" : "");
                if(qs > 0) r += "<span style='color:gray;'>qs</span>:" + qs + (sorterType=="sort_QS" ? "&#11206;" : "");
                if(nf > 0) r += "<span style='color:#880;'>nf</span>:" + nf;
                if(iv > 0) r += "<span style='color:#088;'>iv</span>:" + iv;
            }
            return r + ")</span>";
        }
    });
    window.addEventListener('resize',(event) => {
        table.redraw(true);
    });

    const input = document.getElementById("fSearch");
    input.addEventListener("keyup", function() {
        table.setFilter(matchAny, { value: input.value, filter: filter });
        setUrlParam("search", input.value);
    });
    let param = getUrlParam('search');
    if(param) {
        input.value = param;
        table.setFilter(matchAny, { value: param, filter: filter });
    }
    let allExpanded = false;
    document.getElementById("gridExpandCollapse").onclick = e => {
        allExpanded  = !allExpanded;
        setGlobalOption("gridExpanded", allExpanded ? "true" : "false");
        document.getElementById("gridExpandCollapse").innerHTML = String.fromCharCode(allExpanded ? 8648 : 8650);
        table.setGroupStartOpen(allExpanded);
        table.setGroupBy();
        GroupBy(getGlobalOption("groupBy"), allExpanded, false);
    };

    table.on("rowClick", function(e, row){
        loadReport(row.getData());
    });

    
    let lf = getGlobalOption(projectId + "_filter", "");
    if(lf) {
        filter = JSON.parse(lf);
    }


    for(let p of cooddyResults.problems) {
        filterCheckers.add(p.problem_class.inspection_name);
        if(filter[p.problem_class.inspection_name] === undefined) {
            filter[p.problem_class.inspection_name] = true;  
        }
    }
    document.getElementById("filter_u").addEventListener("click", function (e) { toggleFilter("u"); });
    document.getElementById("filter_p").addEventListener("click", function (e) { toggleFilter("p"); });
    document.getElementById("filter_s").addEventListener("click", function (e) { toggleFilter("s"); });
    document.getElementById("filter_c").addEventListener("click", function (e) { toggleFilter("c"); });
    document.getElementById("filter_UN").addEventListener("click", function (e) { toggleFilter("UN"); });
    document.getElementById("filter_TP").addEventListener("click", function (e) { toggleFilter("TP"); });
    document.getElementById("filter_FP").addEventListener("click", function (e) { toggleFilter("FP"); });
    document.getElementById("filter_QS").addEventListener("click", function (e) { toggleFilter("QS"); });
    document.getElementById("filter_NF").addEventListener("click", function (e) { toggleFilter("NF"); });
    document.getElementById("filter_IV").addEventListener("click", function (e) { toggleFilter("IV"); });

    document.getElementById("file_group").addEventListener("click", function () { GroupBy("file_group"); });
    document.getElementById("checker_source_group").addEventListener("click", function () { GroupBy("checker_source_group"); });
    document.getElementById("source_group").addEventListener("click", function () { GroupBy("source_group"); });
    document.getElementById("no_group").addEventListener("click", function () { GroupBy("no_group"); });
    document.getElementById("default").addEventListener("click", function () { GroupBy("default"); });

    document.getElementById("sort_IT").addEventListener("click", function (e) { sorterType="sort_IT"; GroupBy(getGlobalOption('groupBy')); });
    document.getElementById("sort_UN").addEventListener("click", function (e) { sorterType="sort_UN"; GroupBy(getGlobalOption('groupBy')); });
    document.getElementById("sort_TP").addEventListener("click", function (e) { sorterType="sort_TP"; GroupBy(getGlobalOption('groupBy')); });
    document.getElementById("sort_FP").addEventListener("click", function (e) { sorterType="sort_FP"; GroupBy(getGlobalOption('groupBy')); });
    document.getElementById("sort_QS").addEventListener("click", function (e) { sorterType="sort_QS"; GroupBy(getGlobalOption('groupBy')); });


    document.getElementById("download-csv").addEventListener("click", function () {
        table.download("csv", "cooddy_result.csv", {delimiter:","});
    });
    document.getElementById("download-json").addEventListener("click", function () {
        saveAs( new Blob([JSON.stringify(cooddyResults)], { type: 'text/json'}), "cooddy_result.json");
    });
    
    table.on("tableBuilt", function () {
        let filterCheckersMenuElement = document.getElementById("filterByChecker");
        for(const c of filterCheckers) {
            let newChild = document.createElement('div');
            newChild.innerHTML = '<a class="dropdown-row-item" id="filter_' + c + '" href="#" style="font-weight:bold;">' + c + '</a>';
            filterCheckersMenuElement.appendChild(newChild);
            newChild.addEventListener("click", function (e) { toggleFilter(c); })
        }
        updateFilterSelections();
        const rows = table.getRows();
        let currentId = getUrlParam('currentId') ?? fileNameId;
        if(!currentId) {
            let s = cooddyResults.problems.find(p=> p.status == "New") ?? cooddyResults.problems.find(p=> p.status == "Gone");
            if(s)
                currentId = s.unique_id;
            if(currentId)
                setUrlParam('currentId', currentId);
        }
        table.setGroupStartOpen(allExpanded);
        GroupBy(getGlobalOption('groupBy'));
        if(getGlobalOption('gridExpanded', "false") == "true")
            document.getElementById("gridExpandCollapse").click();
        const r = ShowCurrentRow()
        if(r)
            loadReport(r.getData());
    });
    table.on("cellEdited", function(cell) {
        conclusionChanged(cell.getData());
    });
}
function ShowCurrentRow() {
    let row = undefined;
    try {
        const currentId = getUrlParam('currentId') ?? fileNameId;
        const rows = table.getRows();
        if(rows.length == 0)
            return;
        row = rows.find(r => r.getData().unique_id == currentId) ?? rows[0];
        for(let i = currentGrouping.length - 1; i >= 0; i--) {
            let curGroup = row._row.getGroup();
            for(let j = 0; j < i; j++)
                curGroup = curGroup.parent;
            curGroup.show();
        }
        table.selectRow(row);
        table.scrollToRow(row, "center", false);
    } catch {}
    return row;
}
function focusCurrentRow() {
    try {
        const currentId = getUrlParam('currentId') ?? fileNameId;
        const rows = table.getRows();
        if(rows.length == 0)
            return;
        row = rows.find(r => r.getData().unique_id == currentId) ?? rows[0];
        table.selectRow(row);
    } catch {}
}
function updateGroupStats() {
   groupStats = {} 
   for(let p of cooddyResults.problems) {
       let cur = groupStats;
       for(let gr of currentGrouping) {
           if(!cur[getDescendantProp(p,gr)]) cur[getDescendantProp(p,gr)] = {};
           cur = cur[getDescendantProp(p,gr)]
           if(!cur.count) cur.count = 0;
           if(sorterType=="sort_IT" || 
             (sorterType=="sort_UN" && (!p.conclusion || p.conclusion == "UN")) ||
             (sorterType=="sort_FP" && p.conclusion == "FP") ||
             (sorterType=="sort_TP" && p.conclusion == "TP") ||
             (sorterType=="sort_QS" && p.conclusion == "QS"))
              cur.count++;
      }
   }
}
function GroupBy(par, showCurrentRow = true, updateFilter = true) {
    switch(par) {
        case "file_group":
            currentGrouping = ["file"];
            setGlobalOption("groupBy", "file_group");
            break;
        case "source_group":
            currentGrouping = ["source"];
            setGlobalOption("groupBy", "source_group");
            break;
        case "no_group":
            currentGrouping = [];
            setGlobalOption("groupBy", "no_group");
            break;
        case "checker_source_group":
            currentGrouping = ["problem_class.inspection_name", "source"];
            setGlobalOption("groupBy", "checker_source_group");
            break;
        default:
            currentGrouping = ["problem_class.profile", "problem_class.inspection_name"];
            setGlobalOption("groupBy", "default");
            break;
    }
    qsa(".groupPopupItem", s => s.classList.remove("dropdown-row-item-selected"));
    classAdd(par, "dropdown-row-item-selected");
    setGlobalOption("sorterType", sorterType);
    qsa(".groupSortItem", s => s.classList.remove("dropdown-row-item-selected"));
    classAdd(sorterType, "dropdown-row-item-selected");
    if(cooddyResults.problems.some(p => p.status)) {
        currentGrouping.unshift("status");
    }
    if(cooddyResults.problems.some(p => p.matchId)) {
        currentGrouping.unshift("matchId");
    }
    updateGroupStats();
    table.setGroupBy(currentGrouping);
    table.setSort(currentGrouping.map(g=>{return {column:g, dir:"desc"}}));
    if(showCurrentRow)
        ShowCurrentRow();
    if(updateFilter)
        UpdateFilterStats();
}
function updateStats() {
    updateGroupStats();    

    table.setSort(currentGrouping.map(g=>{return {column:g, dir:"desc"}}));
    focusCurrentRow();
    UpdateFilterStats();
}
function matchAny(data, filterParams) {
    var match = false;
    const regex = RegExp(filterParams.value, 'i');

    for (var key in data) {
        if (regex.test(data[key]) == true) {
            match = true;
        }
    }
    if(data["statusGeneration"] && data["statusGeneration"] != 1 && data["status"] == "Gone") return false;
    let c = data["conclusion"];
    if(c && c == "IV" && filterParams.filter.IV === false) return false;
    if(c && c == "NF" && filterParams.filter.NF === false) return false;
    if((!c || c == "UN") && filterParams.filter.UN === false) return false;
    if(c && c == "TP" && filterParams.filter.TP === false) return false;
    if(c && c == "FP" && filterParams.filter.FP === false) return false;
    if(c && c == "QS" && filterParams.filter.QS === false) return false;
    if(data.reason == "Undefined function" && !filterParams.filter.u) return false;
    if(data.reason == "Public function" && !filterParams.filter.p) return false;
    if(data.reason == "Annotated by macro" && !filterParams.filter.s) return false;
    if(data.reason == "Annotated by config" && !filterParams.filter.c) return false;
    
    let checker = data.problem_class.inspection_name;
    if(filter[checker] === false) return false;

    return match;
}
function updateFilterSelections() {
    setGlobalOption(projectId + "_filter", JSON.stringify(filter));
    
    Object.keys(filter).forEach(function(key, index) {
        updateFilterSelection(key, "filter_" + key);
    });
    document.getElementById("filter").classList.remove("filter-applied");
    Object.keys(filter).forEach(function(key, index) {
        if(!filter[key] && document.getElementById("filter_" + key))
            classAdd("filter", "filter-applied");
    });
        
    const input = document.getElementById("fSearch");
    table.setFilter(matchAny, { value: input.value, filter: filter });
    ShowCurrentRow();
    UpdateFilterStats();
}
function updateFilterSelection(option, item) {
    if(filter[option])
        classAdd(item, "dropdown-row-item-selected");
    else
        classRemove(item, "dropdown-row-item-selected");
}
function toggleFilter(option) {
    filter[option] = filter[option] ? false : true;
    updateFilterSelections();
}

function UpdateFilterStats() {

    function GetName(f) {
        if(f == "p") return "Public";
        if(f == "u") return "Undefined";
        if(f == "s") return "Annotated by macro";
        if(f == "c") return "Annotated by config";
        return f;
    }
    let filterStats = {s:0, u:0, p:0, c:0, UN:0, TP:0, FP:0, QS:0, NF:0, IV:0};
    for(const c of filterCheckers)
        filterStats[c] = 0;
        
    for(let p of cooddyResults.problems) {
        if(p.status == "Gone")
            continue;
        filterStats[p.problem_class.inspection_name]++;
        if(p.conclusion)
            filterStats[p.conclusion]++;
        else
            filterStats["UN"]++;
        if(p.reason == "Undefined function") filterStats.u++;
        if(p.reason == "Public function") filterStats.p++;
        if(p.reason == "Annotated by macro") filterStats.s++;
        if(p.reason == "Annotated by config") filterStats.c++;
    }
    Object.keys(filter).forEach(function(key, index) {
        if(document.getElementById("filter_" + key))
            document.getElementById("filter_" + key).innerHTML = GetName(key) + "<span style='color:gray;font-weight:normal;'>: " + filterStats[key] + "</span>"; 
    });
}

function createGridMenu() {
    var content = document.getElementById("sidebar");
    var d = document.createElement("div");
    d.innerHTML = `
    <div id="myModal" class="modal">
      <div class="modal-content">
        <span class="close" id="closeModal">&times;</span>
        <div>
            <div id="modalContent">
                
            </div>
            <div class="startBtn" id="startBtn">Start</div>
        </div>
      </div>
    </div>
    <div class="menu" id="gridMenu">
				<div class="menuItem2" id="gridExpandCollapse" title="Expand/Collapse all groups in grid view">&#8650</div>
				<div class="dropdown">
					<button class="dropbtn" id="downloadBtn">&#8595 Download</button>
					<div class="dropdown-content" id="downloadBtnContent">
						<a id="download-csv" href="#">Defects CSV</a>
						<a id="download-json" href="#">Defects JSON</a>
					</div>
				</div>
                <div class="dropdown" id="filterBtn">
					<button class="dropbtn" id="filter">&#9071 Filter</button>
					<div class="dropdown-content">
                        <div class="dropdown-header" id="filterSourceHeader">By source function</div>
                        <div class="dropdown-row" id="filterSourceRow">
                            <a class="dropdown-row-item" id="filter_u" href="#" style="font-weight:bold;">Undefined</a>
                            <a class="dropdown-row-item" id="filter_p" href="#" style="font-weight:bold;">Public</a>
                        </div>
                        <div class="dropdown-row" id="filterSourceRow2">
                            <a class="dropdown-row-item" id="filter_s" href="#" style="font-weight:bold;">Annotated by macro</a>
                        </div>
                        <div class="dropdown-row" id="filterSourceRow3">
                            <a class="dropdown-row-item" id="filter_c" href="#" style="font-weight:bold;">Annotated by config</a>
                        </div>
                        <div class="dropdown-header">By conclusion</div>
                        <div class="dropdown-row invisible" id="filterSourceSecb">
                            <a class="dropdown-row-item" id="filter_NF" href="#" style="color:#880;font-weight:bold;">NF</a>
                            <a class="dropdown-row-item" id="filter_IV" href="#" style="color:#088;font-weight:bold;">IV</a>
                        </div>
                        <div class="dropdown-row">
                            <a class="dropdown-row-item" id="filter_UN" href="#" style="color:black;font-weight:bold;">UN</a>
                            <a class="dropdown-row-item" id="filter_TP" href="#" style="color:green;font-weight:bold;">TP</a>
                            <a class="dropdown-row-item" id="filter_FP" href="#" style="color:red;font-weight:bold;">FP</a>
                            <a class="dropdown-row-item" id="filter_QS" href="#" style="color:gray;font-weight:bold;">QS</a>
                        </div>
                        <div class="dropdown-header">By checkers</div>
                        <div style="text-align: left !important;" id="filterByChecker">
                        </div>
                    </div>
				</div>
				<div class="dropdown" id="groupBtn">
					<button class="dropbtn" id="groupInnerBtn">&#10697 Group</button>
					<div class="dropdown-content">
						<div class="dropdown-header">Group by:</div>
                        <a class="dropdown-row-item groupPopupItem" id="no_group" href="#">None </a>
						<a class="dropdown-row-item groupPopupItem" id="default" href="#">Profile, Checker </a>
                        <a class="dropdown-row-item groupPopupItem" id="checker_source_group" href="#">Checker, Source </a>
						<a class="dropdown-row-item groupPopupItem" id="file_group" href="#">File</a>
						<a class="dropdown-row-item groupPopupItem" id="source_group" href="#">Source</a>
                        <div class="dropdown-header">Sort groups by count of:</div>
                        <div class="dropdown-row">
                            <a class="dropdown-row-item groupSortItem" id="sort_IT" href="#" style="color:black;font-weight:bold;">Items</a>
                            <a class="dropdown-row-item groupSortItem" id="sort_UN" href="#" style="color:black;font-weight:bold;">UN</a>
                            <a class="dropdown-row-item groupSortItem" id="sort_TP" href="#" style="color:green;font-weight:bold;">TP</a>
                            <a class="dropdown-row-item groupSortItem" id="sort_FP" href="#" style="color:red;font-weight:bold;">FP</a>
                            <a class="dropdown-row-item groupSortItem" id="sort_QS" href="#" style="color:gray;font-weight:bold;">QS</a>
                        </div>
					</div>
				</div>
				<input type="text" id="fSearch" placeholder="&#8981 Search" name="fSearch">
				<div class="menuItem2" id="gridFullSize">&#9655 </div>
			</div>`;
    content.insertBefore(d, content.firstChild);
    if(cooddyResults.reporter == "secb") {
        classAdd("source_group", "invisible");
        classAdd("checker_source_group", "invisible");
    }
    ge("sidebar").style["max-width"] = window.innerWidth - 30;
    
    if(cooddyResults.problems.length > 1) {
        var content = document.getElementById("downloadBtnContent");
        var d = document.createElement("div");
        d.innerHTML = '<a id="downloadTrace" href="#">Download single trace</a>';
        content.appendChild(d);
        document.getElementById("downloadTrace").addEventListener("click", function () {
            let text = sourceHtmlText;
            let heder = "<script id='cooddyResults' type='text/json'>";
            let startPos = text.indexOf(heder);
            if(startPos < 0) {
                const heder = '<script id="cooddyResults" type="text/json">';
                startPos = text.indexOf(heder);
            }
            startPos += heder.length;
            const endPos = text.indexOf('</script>', startPos);
            let json = {problems:[lastLoadedProblem], code_snippets: {}};
            for(const [k, v] of Object.entries(cooddyResults)) {
                if(k != "problems" && k != "code_snippets")
                    json[k] = v;
            }
            for(const t of lastLoadedProblem.trace) {
                json.code_snippets[t.snippet_path] = cooddyResults.code_snippets[t.snippet_path];
                for(const [h, d] of Object.entries(cooddyResults.code_snippets[t.snippet_path].highlighted_locations)) {
                    for(const dd of d) {
                        json.code_snippets[dd] = cooddyResults.code_snippets[dd];
                    }
                }
            }
            function concatTypedArrays(a, b) {
                var c = new (a.constructor)(a.length + b.length);
                c.set(a, 0);
                c.set(b, a.length);
                return c;
            }
            var data = JSON.stringify(json);
            text = text.slice(0, startPos) + data + text.slice(endPos);
            saveAs( new Blob([text], { type: 'text/html'}), lastLoadedProblem.unique_id + ".html");
            return false;
        });
    }
    if(typeof JSZip != "undefined") {
        var content = document.getElementById("downloadBtnContent");
        var d = document.createElement("div");
        d.innerHTML = '<a id="download-project" href="#">Defects source ZIP</a>';
        content.appendChild(d);
        document.getElementById("download-project").addEventListener("click", function () {
            function zipSourceCode() {
                var zip = new JSZip();
                let files = {};
                
                let chunks = {}
                for(let k in cooddyResults.code_snippets) {
                    let fn = k.split(":")[0];
                    if(fn) {
                        files[fn] = [];
                        if(chunks[fn])
                            chunks[fn].push(cooddyResults.code_snippets[k]);
                        else
                            chunks[fn] = [cooddyResults.code_snippets[k]];
                    }
                }
                for(let k in chunks) {
                    chunks[k].sort((a, b) => {
                        if(a.line < b.line) return -1;
                        if(a.line == b.line) {
                            if(a.end_line > b.end_line) return -1;
                            if(a.end_line == b.end_line) return 0;
                        }
                        return 1;
                    });
                    let cl = 0;
                    for(let i in chunks[k]) {
                        let s = chunks[k][i];
                        if(cl <= s.line) {
                            files[k] += "\n\n//Code snippet line_start: " + s.line + " line_end: " + s.end_line + "\n" + s.code;
                            cl = s.end_line + 1;
                        }
                    }
                }

                for(let k in files)
                    zip.file(k, files[k]);
                return zip.generateAsync({type:"blob"});
            }
            zipSourceCode().then(function(content) {saveAs(content, "project.zip");});
        });
    } else {
        classAdd("download-project", "invisible");
    }
    var prevGridSize = "";
    document.getElementById("gridFullSize").onclick = e => {
        if(sidebar.style.flexBasis == "100%") {
            sidebar.style.flexBasis = prevGridSize;
            document.getElementById("gridFullSize").innerHTML = String.fromCharCode(9655);
        } else {
            prevGridSize = sidebar.style.flexBasis;
            sidebar.style.flexBasis = "100%";
            document.getElementById("gridFullSize").innerHTML = String.fromCharCode(9665);
        }
    }
}

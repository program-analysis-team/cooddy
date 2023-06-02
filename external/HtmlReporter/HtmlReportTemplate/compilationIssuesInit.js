var compilationIssuesTable;
var currentGrouping = [];

function compilationIssuesTabulatorInit() {
    createCompilationIssuesMenu();
    compilationIssuesTable = new Tabulator("#compilationIssuesData", {
        data: cooddyResults.compilation_issues,
        layout: "fitColumns",
        height: window.innerHeight - 90,
        autoResize: true,
        resizableRows: false,
        selectable: false,
        movableRows: false,
        groupToggleElement: "header",
        placeholder: "No Data Available",
        initialSort: [{column: "tu", dir: "asc"}],
        groupHeader: function (value, count, data, group) {
            let item = "items";
            if (count === 1)
                item = "item";
            return "<span style='color:#d00; margin-left:10px;'>" + value + " (" + count + " " + item + ")</span>";
        },
        columns: [
            {
                title: "Translation unit", field: "tu", visible: true, minWidth: 200,
                formatter: function (cell, formatterParams, onRendered) {
                    cell.getElement().style['direction'] = "rtl";
                    return '<bdo dir="ltr">' + cell.getValue() + '</bdo>';
                 },
             },
            {title: "File", field: "file", visible: true, minWidth: 200,
                 formatter:function(cell, formatterParams, onRendered){
                    cell.getElement().style['direction'] = "rtl";
                    return '<bdo dir="ltr">' + cell.getValue() + '</bdo>';
                 }
             },
            {title: "Message", field: "message", visible: true, },
            {title: "Line", field: "line", visible: true, maxWidth: 100},
            {title: "Column", field: "column", visible: true, maxWidth: 100},
            {title: "Severity", field: "severity", visible: true, maxWidth: 150},
        ]
    });
    window.addEventListener('resize',(event) => {
        compilationIssuesTable.redraw(true);
    });
    document.getElementById("issuesByDefault").addEventListener("click", function () { groupCompilationIssuesBy("default"); });
    document.getElementById("issuesByFile").addEventListener("click", function () { groupCompilationIssuesBy("file"); });
    document.getElementById("issuesByTU").addEventListener("click", function () { groupCompilationIssuesBy("tu"); });
    document.getElementById("issuesBySeverity").addEventListener("click", function () { groupCompilationIssuesBy("severity"); });
}

function groupCompilationIssuesBy(par) {
    qsa(".groupPopupItem", s => s.classList.remove("dropdown-row-item-selected"));
    switch(par) {
        case "file":
            currentGrouping = ["file"];
            classAdd("issuesByFile", "dropdown-row-item-selected");
            break;
        case "tu":
            currentGrouping = ["tu"];
            classAdd("issuesByTU", "dropdown-row-item-selected");
            break;
        case "severity":
            currentGrouping = ["severity"];
            classAdd("issuesBySeverity", "dropdown-row-item-selected");
            break;
        default:
            currentGrouping = [];
            compilationIssuesTable.setGroupBy();
            compilationIssuesTable.setSort("tu", "asc");
            return;
    }
    compilationIssuesTable.setGroupBy(currentGrouping);
    compilationIssuesTable.setSort(currentGrouping.map(g=>{return {column:g, dir:"asc"}}));
}

function createCompilationIssuesMenu() {
    let content = document.getElementById("cooddyCompilationIssuesContainer");
    let d = document.createElement("div");
    d.innerHTML = `
    <div class="menu" id="compilationIssuesMenu">
        <div class="dropdown" id="compilationIssuesGroupBtn">
            <button class="dropbtn" id="groupCompilationIssuesInnerBtn">&#10697 Group</button>
            <div class="dropdown-content">
                <div class="dropdown-header">Group by:</div>
                <a class="dropdown-row-item groupPopupItem" id="issuesByDefault" href="#">Default</a>
                <a class="dropdown-row-item groupPopupItem" id="issuesByFile" href="#">File</a>
                <a class="dropdown-row-item groupPopupItem" id="issuesByTU" href="#">Translation unit</a>
                <a class="dropdown-row-item groupPopupItem" id="issuesBySeverity" href="#">Severity</a>
            </div>
        </div>
    </div>`;
    content.insertBefore(d, content.firstChild);
}
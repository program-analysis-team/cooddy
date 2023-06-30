var compilationIssuesTable;
var compilationIssuesCategories;
function compilationIssuesCategoriesInit() {
    compilationIssuesCategories = new Tabulator("#compilationIssuesCategories", {
        data: cooddyResults.compilation_issues,
        index:"id",
        layout: "fitColumns",
        height: window.innerHeight - 53,
        autoResize: true,
        resizableRows: false,
        selectable: 1,
        selectablePersistence: false,
        movableRows: false,
        headerVisible: false,
        groupBy: "reason",
        groupStartOpen: false,
        groupToggleElement: "header",
        placeholder: "No Data Available",
        initialSort: [{column: "unit", dir: "asc"}],
        groupHeader:[
            function(value, count, data){
                let item = "items";
                if (count === 1)
                    item = "item";
                return "<span style='color:#d00; margin-left:10px; direction:rtl'>"+value+" ("+count+" "+item+")</span>";
            },
        ],
        columns: [
            {title: "Translation unit", field: "unit", visible: true, minWidth: 200,
                formatter: function (cell, formatterParams, onRendered) {
                    cell.getElement().style['direction'] = "rtl";
                    return '<bdo dir="ltr">' + cell.getValue() + '</bdo>';
                 },
                 tooltip:function(e, cell, onRendered){
                     return cell.getData().unit;
                 },
             },
            {title: "Reason", field: "reason", visible: false}
        ]
    });
    compilationIssuesCategories.on("rowClick", function(e, row){
        loadCompileIssue(null);
        classRemove("cooddyCompilationIssuesContainer","invisible");
        compilationIssuesTable=new Tabulator("#compilationIssuesData",{
            data:row.getData().errors,
            layout:"fitColumns",
            height:window.innerHeight-390,
            autoResize:true,
            resizableRows:false,
            selectable:1,
            movableRows:false,
            groupToggleElement:"header",
            placeholder:"No Data Available",
            groupHeader: function (value, count, data, group) {
                let item = "items";
                if (count === 1)
                    item = "item";
                return "<span style='color:#d00; margin-left:10px;'>" + value + " (" + count + " " + item + ")</span>";
            },
            columns: [
                {title: "Id", field: "id", visible: false},
                {title: "Location", field: "file", visible: true, minWidth: 200, widthGrow: 1, sorter:"string",
                    sorterParams:{
                        locale:true,
                        alignEmptyValues:"top",
                    },
                    formatter:function(cell, formatterParams, onRendered){
                        if(cell.getValue().length === 0)
                            return '<bdo dir="ltr">No location</bdo>';
                        let d = cell.getData();
                        cell.getElement().style['direction'] = "rtl";
                        let innerCompileIssue = "";
                        if(d.line && d.column)
                            innerCompileIssue = ':' + d.line + ':' + d.column;
                        return '<bdo dir="ltr">' + cell.getValue() + innerCompileIssue + '</bdo>';
                    },
                    tooltip:function(e, cell, onRendered){
                        return cell.getData().file;
                    }
                 },
                {title: "Line", field: "line", visible: false},
                {title: "Column", field: "column", visible: false},
                {title: "Message", field: "message", minWidth:150, widthGrow:1, visible: true},
                {title: "CodeSnippet", field: "codeSnippet", visible: false}
            ]
        });
        compilationIssuesTable.on("rowClick", function(e, row){
            let data = row.getData();
            loadCompileIssue(data);
        });
    });
    window.addEventListener('resize',(event) => {
        compilationIssuesCategories.redraw(true);
    });
}
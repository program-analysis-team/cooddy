/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.txt.
var qsa = (selector, func) => document.querySelectorAll(selector).forEach(func)
var ge = el => document.getElementById(el);
var ce = el => document.createElement(el);
var classAdd = (el, theClass) => {
    const e = ge(el);
    if(e) e.classList.add(theClass);
}
var classRemove = (el, theClass) => {
    const e = ge(el);
    if(e) e.classList.remove(theClass);
}
var showHide = (el, show) =>{ if(show) classRemove(el, 'invisible'); else classAdd(el, 'invisible'); }
function getUrlParam(param) {
    const urlParams = new URLSearchParams(window.location.search);
    return urlParams.get(param);
}
function setUrlParam(param, value, defaultValue = "default") {
    if ('URLSearchParams' in window) {
        let prevSerch = window.location.search;
        var searchParams = new URLSearchParams(window.location.search);
        if(value && value != defaultValue)
            searchParams.set(param, value);
        else 
            searchParams.delete(param);
        let newurl = window.location.protocol + "//" + window.location.host + window.location.pathname + '?' + searchParams.toString();
        window.history.pushState({path:newurl},'',newurl);
    }
}

const globalOptPrefix = "cooddyResultViewOption_";
function getGlobalOption(opt, defaultValue = "default") {
    return window.localStorage[globalOptPrefix + opt] ?? defaultValue;
}
function setGlobalOption(opt, val) {
    window.localStorage[globalOptPrefix + opt] = val;
}
function uuidv4() {
  return ([1e7]+-1e3+-4e3+-8e3+-1e11).replace(/[018]/g, c =>
    (c ^ crypto.getRandomValues(new Uint8Array(1))[0] & 15 >> c / 4).toString(16)
  );
}

var scrollTo = function(el) {
    qsa(".selected", s => s.classList.remove("selected"));
    qsa(".gselected", s => s.classList.remove("gselected"));
    try {
        let e = ge("g" + el.id);
        if(e)
            e.classList.add("gselected");
        el.classList.add("selected");
        el.scrollIntoView({ behavior: "smooth", block: "center", inline: "nearest" });
    }catch{}
};

function SetElementContent(id, content) {
    var e = ge(id);
    e.textContent = '';
    e.appendChild(content);
}
function SetPreContent(id, content) {
    var codeContainer = ce('div');
    codeContainer.innerHTML = content;
    SetElementContent(id, codeContainer);
}
if (!Array.prototype.last){
    Array.prototype.last = function(){
        return this[this.length - 1];
    };
};
function SetupResizer() {
    window.onresize = () => window.location.reload()
    const sidebar = ge("sidebar");
    let w = getGlobalOption("sidebarWidth", '400px');
    if(w.endsWith('px')) w = w.slice(0, -2);
    w = w > window.innerWidth - 30 ? '400px' : (w+'px');
    
    sidebar.style.flexBasis = w;
    document.querySelector("#resizer").addEventListener("mousedown", (event) => {
        function resize(e) {
            if(e.x > window.innerWidth - 10)
                e.x = window.innerWidth - 10;
            sidebar.style.flexBasis = `${e.x}px`;
            setGlobalOption("sidebarWidth", e.x);
            let resizer = ge("gridFullSize")
            if(resizer) resizer.innerHTML = String.fromCharCode(9655);
        }
        document.querySelector("body").classList.add("noSelection");
        document.addEventListener("mousemove", resize, false);
        document.addEventListener("mouseup", () => {
            document.removeEventListener("mousemove", resize, false);
            document.querySelector("body").classList.remove("noSelection");
        }, false);
    });
    document.querySelector("#resizer2").addEventListener("mousedown", (event) => {
        function resize(e) {
            if(e.x > window.innerWidth - 10)
                e.x = window.innerWidth - 10;
            let w = window.innerWidth - e.x;
            ge("rightSidebar").style.flexBasis = `${w}px`;
        }
        document.querySelector("body").classList.add("noSelection");
        document.addEventListener("mousemove", resize, false);
        document.addEventListener("mouseup", () => {
            document.removeEventListener("mousemove", resize, false);
            document.querySelector("body").classList.remove("noSelection");
        }, false);
    });
}
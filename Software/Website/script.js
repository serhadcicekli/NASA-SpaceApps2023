var l0 = document.getElementById("bg-layer0");
var l1 = document.getElementById("bg-layer1");
var l2 = document.getElementById("bg-layer2");
var l3 = document.getElementById("bg-layer3");
var l4 = document.getElementById("bg-layer4");
var l5 = document.getElementById("bg-layer5");
var navb = document.getElementById("navbar");
var navbloc = 0;
var menuvisible = false;

function pageScroll(){
    var scrollState = document.documentElement.scrollTop;
    l0.style.top = -scrollState / 2 + "px";
    l1.style.top = -scrollState / 3 + "px";
    l2.style.top = -scrollState / 4 + "px";
    l3.style.top = -scrollState / 5 + "px";
    l4.style.top = -scrollState / 6 + "px";
    l5.style.top = -scrollState / 8 + "px";
    if(scrollState > navbloc + navb.offsetHeight){
        navbloc = scrollState - navb.offsetHeight;
    }else if(scrollState < navbloc){
        navbloc = scrollState;
    }
    if(navbloc > scrollState - navb.offsetHeight / 2 || navbloc < navb.offsetHeight || menuvisible){
        navb.style.top = "0px";
    }else{
        navb.style.top = -navb.offsetHeight + "px";
    }
}
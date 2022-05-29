const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_RELOAD=201;const UPDATE_OFFSET=100;const UI_EXTEND_GUI=210;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPDATE_ACCEL=118;const UI_SEPARATOR=19;const UPDATE_SEPARATOR=119;const UI_TIME=20;const UPDATE_TIME=120;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_DARK:case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart,});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);if(window.location.port!=""||window.location.port!=80||window.location.port!=443){websock=new WebSocket("ws://"+window.location.hostname+":"+window.location.port+"/ws");}else{websock=new WebSocket("ws://"+window.location.hostname+"/ws");}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>(data.controls.length-1)){websock.send("uiok:"+(data.controls.length-1));}
break;case UI_EXTEND_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>data.startindex+(data.controls.length-1)){websock.send("uiok:"+(data.startindex+(data.controls.length-1)));}
break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GAUGE:case UI_SEPARATOR:if(data.visible)addToHTML(data);break;case UI_BUTTON:if(data.visible){addToHTML(data);$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);},});}
break;case UI_SWITCHER:if(data.visible){addToHTML(data);switcher(data.id,data.value);}
break;case UI_CPAD:case UI_PAD:if(data.visible){addToHTML(data);$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});}
break;case UI_SLIDER:if(data.visible){addToHTML(data);rangeSlider(!sliderContinuous);}
break;case UI_TAB:if(data.visible){$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});}
break;case UI_OPTION:if(data.parentControl){var parent=$("#select"+data.parentControl);parent.append("<option id='option"+
data.id+
"' value='"+
data.value+
"' "+
data.selected+
">"+
data.label+
"</option>");}
break;case UI_MIN:if(data.parentControl){if($('#sl'+data.parentControl).length){$('#sl'+data.parentControl).attr("min",data.value);}else if($('#num'+data.parentControl).length){$('#num'+data.parentControl).attr("min",data.value);}}
break;case UI_MAX:if(data.parentControl){if($('#sl'+data.parentControl).length){$('#sl'+data.parentControl).attr("max",data.value);}else if($('#text'+data.parentControl).length){$('#text'+data.parentControl).attr("maxlength",data.value);}else if($('#num'+data.parentControl).length){$('#num'+data.parentControl).attr("max",data.value);}}
break;case UI_STEP:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){parent.attr("step",data.value);}}
break;case UI_GRAPH:if(data.visible){addToHTML(data);graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);}
break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_ACCEL:if(hasAccel)break;hasAccel=true;if(data.visible){addToHTML(data);requestOrientationPermission();}
break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);if(data.hasOwnProperty('elementStyle')){$("#l"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SLIDER:$("#sl"+data.id).attr("value",data.value)
slider_move($("#id"+data.id),data.value,"100",false);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#num"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#text"+data.id).attr("style",data.elementStyle);}
if(data.hasOwnProperty('inputType')){$("#text"+data.id).attr("type",data.inputType);}
break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#select"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_BUTTON:$("#btn"+data.id).val(data.value);$("#btn"+data.id).text(data.value);if(data.hasOwnProperty('elementStyle')){$("#btn"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#gauge"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_ACCEL:break;case UPDATE_TIME:var rv=new Date().toISOString();websock.send("time:"+rv+":"+data.id);break;default:console.error("Unknown type or event");break;}
if(data.type>=UI_TITEL&&data.type<UPDATE_OFFSET){processEnabled(data);}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.hasOwnProperty('panelStyle')){$("#id"+data.id).attr("style",data.panelStyle);}
if(data.hasOwnProperty('visible')){if(data['visible'])
$("#id"+data.id).show();else
$("#id"+data.id).hide();}
if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}
processEnabled(data);}
$(".range-slider__range").each(function(){$(this)[0].value=$(this).attr("value");$(this).next().html($(this).attr("value"));});};websock.onmessage=handleEvent;}
function sliderchange(number){var val=$("#sl"+number).val();websock.send("slvalue:"+val+":"+number);$(".range-slider__range").each(function(){$(this).attr("value",$(this)[0].value);});}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function tabclick(number){var val=$("#tab"+number).val();websock.send("tabvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){if($("#id"+number+" nav").hasClass("disabled")){return;}
switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if(!$("#sl"+number).hasClass("checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var range=$(".range-slider__range");var slidercb=function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));};range.on({input:function(){$(this).next().html(this.value)}});range.each(function(){$(this).next().html(this.value);if($(this).attr("callbackSet")!="true"){if(!isDiscrete){$(this).on({input:slidercb});}else{$(this).on({change:slidercb});}
$(this).attr("callbackSet","true");}});};var addToHTML=function(data){panelStyle=data.hasOwnProperty('panelStyle')?" style='"+data.panelStyle+"' ":"";panelwide=data.hasOwnProperty('wide')?"wide":"";if(!data.hasOwnProperty('parentControl')||$("#tab"+data.parentControl).length>0){var parent=data.hasOwnProperty('parentControl')?$("#tab"+data.parentControl):$("#row");var html="";switch(data.type){case UI_LABEL:case UI_BUTTON:case UI_SWITCHER:case UI_CPAD:case UI_PAD:case UI_SLIDER:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GRAPH:case UI_GAUGE:case UI_ACCEL:html="<div id='id"+data.id+"' "+panelStyle+" class='two columns "+panelwide+" card tcenter "+
colorClass(data.color)+"'><h5>"+data.label+"</h5><hr/>"+
elementHTML(data)+
"</div>";break;case UI_SEPARATOR:html="<div id='id"+data.id+"' "+panelStyle+" class='sectionbreak columns'>"+
"<h5>"+data.label+"</h5><hr/></div>";break;case UI_TIME:break;}
parent.append(html);}else{var parent=$("#id"+data.parentControl);parent.append(elementHTML(data));}}
var elementHTML=function(data){var id=data.id
var elementStyle=data.hasOwnProperty('elementStyle')?" style='"+data.elementStyle+"' ":"";var inputType=data.hasOwnProperty('inputType')?" type='"+data.inputType+"' ":"";switch(data.type){case UI_LABEL:return"<span id='l"+id+"' "+elementStyle+
" class='label label-wrap'>"+data.value+"</span>";case UI_BUTTON:return"<button id='btn"+id+"' "+elementStyle+
" onmousedown='buttonclick("+id+", true)'"+
" onmouseup='buttonclick("+id+", false)'>"+
data.value+"</button>";case UI_SWITCHER:return"<label id='sl"+id+"' "+elementStyle+
" class='switch "+(data.value=="1"?"checked":"")+
(data.hasOwnProperty('vertical')?" vert-switcher ":"")+
"'>"+
"<div class='in'>"+
"<input type='checkbox' id='s"+id+"' onClick='switcher("+id+",null)' "+
(data.value=="1"?"checked":"")+"/></div></label>";case UI_CPAD:case UI_PAD:return"<nav class='control'><ul>"+
"<li><a onmousedown='padclick(UP, "+id+", true)' "+
"onmouseup='padclick(UP, "+id+", false)' id='pf"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+id+", true)' "+
"onmouseup='padclick(RIGHT, "+id+", false)' id='pr"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+id+", true)' "+
"onmouseup='padclick(LEFT, "+id+", false)' id='pl"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(DOWN, "+id+", true)' "+
"onmouseup='padclick(DOWN, "+id+", false)' id='pb"+id+"'>&#9650;</a></li>"+
"</ul>"+
(data.type==UI_CPAD?"<a class='confirm' onmousedown='padclick(CENTER,"+id+", true)' "+
"onmouseup='padclick(CENTER, "+id+", false)' id='pc"+id+"'>OK</a>":"")+
"</nav>";case UI_SLIDER:return"<div class='range-slider "+
(data.hasOwnProperty('vertical')?" vert-slider ":"")+
"'>"+
"<input id='sl"+id+"' type='range' min='0' max='100' value='"+data.value+"' "+
elementStyle+" class='range-slider__range'><span class='range-slider__value'>"+
data.value+"</span></div>";case UI_NUMBER:return"<input style='color:black;' "+elementStyle+" id='num"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_TEXT_INPUT:return"<input "+inputType+"style='color:black;' "+elementStyle+" id='text"+id+
"' value='"+data.value+"' onchange='textchange("+id+")' />";case UI_SELECT:return"<select style='color:black;' "+elementStyle+" id='select"+id+
"' onchange='selectchange("+id+")' />";case UI_GRAPH:return"<figure id='graph"+id+"'><figcaption>"+data.label+"</figcaption></figure>";case UI_GAUGE:return"WILL BE A GAUGE <input style='color:black;' id='gauge"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_ACCEL:return"ACCEL // Not implemented fully!<div class='accelerometer' id='accel"+id+
"' ><div class='ball"+id+"'></div><pre class='accelerometeroutput"+id+"'></pre>";default:return"";}}
var processEnabled=function(data){switch(data.type){case UI_SWITCHER:case UPDATE_SWITCHER:if(data.enabled){$("#sl"+data.id).removeClass('disabled');$("#s"+data.id).prop("disabled",false);}else{$("#sl"+data.id).addClass('disabled');$("#s"+data.id).prop("disabled",true);}
break;case UI_SLIDER:case UPDATE_SLIDER:$("#sl"+data.id).prop("disabled",!data.enabled);break;case UI_NUMBER:case UPDATE_NUMBER:$("#num"+data.id).prop("disabled",!data.enabled);break;case UI_TEXT_INPUT:case UPDATE_TEXT_INPUT:$("#text"+data.id).prop("disabled",!data.enabled);break;case UI_SELECT:case UPDATE_SELECT:$("#select"+data.id).prop("disabled",!data.enabled);break;case UI_BUTTON:case UPDATE_BUTTON:$("#btn"+data.id).prop("disabled",!data.enabled);break;case UI_PAD:case UI_CPAD:case UPDATE_PAD:case UPDATE_CPAD:if(data.enabled){$("#id"+data.id+" nav").removeClass('disabled');}else{$("#id"+data.id+" nav").addClass('disabled');}
break;}}
)=====";

const uint8_t JS_CONTROLS_GZIP[4212] PROGMEM = { 31,139,8,0,13,167,140,98,2,255,197,27,105,119,218,72,242,187,127,133,172,228,5,177,198,28,185,38,3,150,243,8,38,9,59,190,214,198,147,121,155,201,250,9,104,140,158,133,164,145,132,143,113,252,223,183,186,250,80,183,14,32,120,102,247,75,140,170,171,171,171,171,170,187,174,206,56,240,227,196,184,24,92,14,142,7,195,65,247,240,242,211,197,192,126,217,108,118,198,98,224,172,127,120,210,61,0,88,75,192,78,15,186,195,254,229,201,199,143,231,253,161,221,82,113,251,191,13,251,199,7,140,70,75,129,15,7,195,254,161,173,0,78,129,98,134,30,130,210,69,6,151,61,10,121,169,35,245,24,214,203,20,235,195,197,112,120,114,108,191,210,241,56,180,213,124,149,98,30,118,63,0,19,175,117,68,6,108,53,95,167,120,231,95,6,195,222,231,254,153,253,70,71,149,240,86,243,141,130,125,56,56,0,216,219,12,46,131,182,154,111,83,204,227,139,163,15,0,251,73,199,228,208,86,243,39,69,92,32,71,80,200,233,197,208,126,167,99,43,35,173,230,187,116,198,167,179,238,233,103,251,103,14,232,30,28,48,200,229,233,201,224,152,162,242,129,222,97,191,123,198,145,91,205,159,149,21,187,31,236,86,70,33,12,166,40,228,188,127,216,239,1,181,140,78,4,184,165,104,229,228,116,56,160,242,207,168,69,128,91,138,90,142,6,0,200,104,5,97,45,69,39,71,221,223,236,86,70,31,8,107,169,170,24,246,79,237,86,86,19,8,108,41,122,248,212,189,248,212,183,91,25,61,112,104,75,81,67,183,215,163,182,145,81,1,135,182,222,169,130,57,237,158,117,135,39,160,198,159,179,178,145,35,45,85,220,131,163,62,156,168,140,188,41,176,165,64,229,129,57,56,249,114,44,143,203,97,255,227,80,30,139,179,193,167,207,67,105,252,189,254,241,16,108,73,200,173,119,57,188,56,251,215,197,201,224,188,47,73,245,46,251,71,253,179,238,97,122,252,122,151,167,125,152,117,54,248,21,166,190,148,192,47,253,97,247,252,244,115,247,80,33,127,121,126,113,252,241,240,228,139,182,70,175,123,118,118,50,148,135,165,119,217,61,28,252,187,123,6,26,124,43,65,7,221,179,95,164,225,247,46,143,79,142,97,251,111,222,116,110,156,200,184,138,156,112,118,224,36,142,237,147,91,163,27,69,206,189,85,197,145,153,19,119,199,99,226,217,83,199,139,9,130,98,207,157,144,168,23,248,137,235,47,130,69,204,135,166,11,127,156,184,129,111,140,3,47,136,122,158,19,199,22,254,28,76,170,15,252,135,125,188,152,143,72,36,225,157,248,214,77,198,51,5,207,137,137,42,179,118,68,146,69,228,155,240,207,31,139,192,141,137,217,225,40,92,132,2,129,204,73,228,120,19,57,156,202,83,96,132,36,33,81,228,222,144,72,34,165,242,21,72,183,36,113,226,112,230,120,137,68,146,242,22,56,241,194,159,122,193,173,66,135,137,95,140,143,157,40,10,210,249,66,21,98,216,241,220,63,157,200,245,37,2,85,76,155,255,166,90,17,136,19,39,186,54,59,19,50,117,22,94,34,128,102,231,241,113,139,106,225,150,140,226,96,124,221,81,126,131,74,124,50,78,200,36,171,145,136,252,177,32,113,114,18,185,196,79,28,10,58,37,209,220,141,99,248,101,85,31,30,183,36,102,236,220,144,79,194,24,96,200,11,198,142,119,158,4,145,115,69,234,49,73,6,9,153,91,38,137,195,133,139,54,19,155,181,127,158,159,28,215,227,4,118,116,229,78,239,45,105,74,213,106,71,33,28,193,250,65,164,208,118,65,219,104,77,176,226,4,77,79,91,235,170,112,173,148,120,199,157,90,114,234,182,237,47,60,175,250,144,210,66,166,66,39,138,73,138,85,237,48,25,166,75,126,117,39,223,128,73,6,254,250,45,203,175,19,37,32,130,231,214,36,24,47,230,32,185,106,221,153,76,44,243,31,102,181,30,76,167,112,64,158,91,230,179,40,184,133,239,89,50,247,44,211,172,118,184,42,234,99,47,128,181,193,192,25,21,149,52,156,192,115,208,194,34,238,131,157,68,176,66,137,250,40,117,137,11,107,68,100,30,220,16,118,176,76,60,49,187,87,17,33,190,89,205,161,2,159,26,94,68,38,5,88,140,105,228,194,104,24,199,129,193,57,160,76,190,120,246,238,237,171,55,157,130,89,233,214,53,168,111,61,140,61,119,124,221,230,146,171,61,106,155,158,57,254,196,35,191,186,177,59,114,61,55,185,239,1,224,10,4,244,0,122,220,206,10,224,197,139,109,33,243,250,204,157,76,136,95,125,144,10,161,7,32,181,87,174,36,137,14,59,239,223,192,143,67,55,78,136,15,87,141,121,35,215,28,227,154,102,173,152,151,26,74,29,13,235,214,245,39,193,109,157,26,36,93,165,30,6,81,178,109,155,230,247,239,197,35,239,154,101,35,175,95,191,146,250,197,187,245,11,25,157,195,111,146,88,230,109,220,110,52,204,157,236,196,89,16,39,190,51,39,59,102,59,63,72,169,238,152,141,91,16,121,231,145,0,191,27,19,231,52,182,132,193,6,126,16,18,223,22,146,181,200,77,66,175,109,63,14,60,2,243,175,128,36,195,52,40,222,26,54,87,102,155,9,185,3,254,164,174,211,51,147,218,127,18,45,72,231,177,147,178,134,167,105,29,222,16,17,72,102,207,152,70,141,80,216,18,106,20,80,68,129,249,67,106,59,104,98,171,40,80,244,73,230,50,130,129,250,4,239,34,58,74,108,105,183,163,96,114,143,176,49,124,145,8,140,77,248,70,138,94,79,238,67,194,189,163,158,39,180,11,238,32,10,74,156,81,236,59,55,69,96,96,20,142,70,162,14,129,205,227,50,89,215,14,87,106,214,217,23,226,129,29,33,156,146,142,2,47,174,79,131,168,239,0,243,196,35,116,123,246,62,222,245,224,201,238,152,232,30,40,122,59,227,58,56,114,181,246,216,81,196,108,201,89,160,131,148,213,36,72,28,79,172,183,111,233,203,123,196,191,74,102,187,173,170,60,124,224,188,124,184,187,23,110,112,13,199,170,20,29,54,50,138,136,115,221,17,162,78,179,169,246,255,119,135,76,238,244,186,131,35,77,238,118,54,218,241,58,243,243,50,96,217,103,59,123,151,68,196,11,156,9,156,12,29,27,243,204,182,52,236,196,77,60,194,172,198,115,70,196,67,51,156,59,174,255,153,56,96,67,194,10,83,132,44,61,204,14,219,226,139,165,106,242,51,205,197,36,136,165,66,242,19,115,10,101,144,231,2,109,33,101,116,15,30,169,194,245,53,12,62,15,143,14,45,118,62,117,38,88,46,155,155,244,144,157,69,55,55,74,124,115,7,209,32,198,65,199,152,4,139,241,12,101,223,78,239,140,234,3,169,135,17,161,106,63,96,49,30,21,229,34,73,232,117,7,142,212,226,36,106,244,54,4,165,212,144,10,232,116,19,26,220,183,61,50,175,172,239,77,228,212,171,119,199,110,36,112,170,130,44,67,119,60,100,48,67,150,150,9,164,220,233,239,181,132,23,78,55,150,93,232,76,216,166,47,78,107,155,202,174,136,134,42,59,100,209,123,58,139,52,129,124,58,147,26,149,28,155,209,211,217,196,212,246,233,124,234,100,114,140,142,158,206,40,205,206,159,206,167,70,37,199,230,248,233,108,178,226,192,211,25,205,208,89,122,186,177,10,182,250,240,69,52,6,62,71,191,110,109,231,226,128,28,217,97,247,67,158,166,30,120,56,97,136,30,104,207,115,247,247,28,35,240,231,64,137,44,66,187,2,56,108,31,82,162,59,102,181,98,204,34,50,181,43,148,130,2,175,236,243,15,188,102,118,204,189,134,179,191,215,0,146,133,17,141,92,115,226,222,24,238,4,151,210,136,237,53,96,132,206,165,243,108,32,80,215,9,192,215,136,76,122,236,219,130,252,55,8,219,84,77,143,85,140,219,44,211,9,93,182,178,3,216,83,215,131,96,205,146,106,163,105,10,166,152,207,173,100,230,198,192,79,146,64,10,66,55,102,86,109,219,134,72,152,238,123,55,9,118,217,93,10,233,60,26,147,137,112,179,166,26,0,141,39,128,29,59,140,130,121,8,33,243,208,25,25,73,96,176,137,244,151,229,99,53,197,128,244,13,108,242,61,139,227,182,233,126,234,12,9,102,88,240,9,49,129,227,145,136,146,152,57,9,165,105,76,2,18,27,126,144,24,228,14,18,37,163,253,251,239,152,12,20,216,92,129,73,177,10,162,84,63,68,182,48,163,199,34,9,198,54,3,81,241,62,139,33,230,25,39,92,7,58,106,135,125,166,74,11,66,76,234,168,222,216,79,115,103,75,232,110,203,172,24,104,2,118,69,64,153,69,208,1,1,97,139,17,138,189,47,96,24,84,0,96,175,193,104,238,155,249,29,29,13,74,183,3,224,231,86,229,89,236,85,138,182,192,227,38,106,250,229,56,204,8,230,174,111,234,14,147,230,111,6,167,15,170,92,189,64,41,82,217,10,185,125,118,127,251,251,247,233,220,45,217,39,205,254,86,175,80,142,37,215,96,232,127,183,68,115,123,201,93,175,195,254,233,154,39,193,157,20,157,130,29,211,112,253,112,145,176,211,203,79,68,236,254,73,44,56,182,226,128,32,51,113,66,194,21,220,96,87,97,245,93,47,203,104,95,249,233,250,102,231,234,115,194,213,117,128,3,240,5,56,112,126,115,101,229,231,214,76,132,165,222,81,63,94,153,70,72,27,175,181,216,62,114,146,89,61,10,22,112,240,105,233,2,40,194,142,105,217,111,232,206,225,87,163,213,108,54,139,56,173,135,139,120,102,61,220,181,147,184,118,223,78,165,1,247,84,166,120,185,9,231,10,223,74,159,166,93,32,176,175,223,254,226,245,68,179,131,170,79,84,221,171,108,92,22,225,177,48,178,218,149,47,45,247,102,46,63,165,5,135,181,4,53,184,77,51,51,110,112,98,105,96,232,228,214,63,141,130,16,220,202,189,85,225,121,237,121,114,239,145,74,149,5,2,42,33,97,192,48,204,45,88,157,81,204,146,76,75,150,100,29,224,81,155,230,251,102,187,245,99,172,197,79,230,141,5,85,37,180,144,53,237,164,110,177,128,234,146,22,112,45,237,46,128,89,10,98,205,4,179,55,149,74,228,255,110,75,60,177,166,180,224,82,84,136,1,99,27,219,128,78,105,19,182,148,4,31,163,61,112,12,127,17,111,25,82,171,153,43,35,142,215,247,240,62,92,78,153,86,240,56,97,57,161,196,184,88,1,35,23,60,61,117,195,57,98,155,232,131,23,66,242,117,142,44,103,121,12,44,252,110,202,188,78,106,19,206,211,114,68,250,140,161,157,71,99,245,34,186,228,149,179,184,34,127,145,236,179,180,54,217,0,243,14,5,71,100,112,212,71,175,26,221,216,138,43,77,130,193,249,201,57,150,30,173,180,47,197,138,130,9,248,216,182,185,19,221,96,147,33,227,141,68,199,81,20,180,177,92,110,153,23,254,181,31,220,250,6,53,100,154,114,96,158,96,138,73,233,241,160,227,251,182,40,4,190,120,33,129,123,218,163,21,8,110,162,96,76,226,184,239,59,224,197,38,220,119,229,232,168,115,52,98,90,25,156,5,91,162,22,155,189,97,75,85,21,58,62,241,52,69,169,211,10,180,148,78,88,118,35,112,207,76,105,114,148,175,18,246,173,186,149,91,39,158,5,183,160,35,26,189,230,7,103,224,57,172,172,96,108,91,243,68,213,7,190,117,189,73,200,188,206,174,108,158,243,214,253,46,111,150,139,207,180,51,46,32,105,27,92,64,100,211,91,0,88,151,91,124,201,166,54,221,5,17,157,56,157,13,115,71,121,25,192,171,207,240,93,21,157,172,194,45,164,188,11,166,21,110,85,54,37,127,156,177,101,28,149,242,177,85,98,147,180,78,128,5,146,93,238,200,47,241,203,172,214,9,237,1,40,249,63,79,252,191,54,191,241,8,69,175,4,176,208,128,222,142,12,234,211,59,145,7,90,133,152,44,1,87,154,87,115,96,207,185,34,182,210,63,80,251,172,140,63,214,233,228,245,1,118,54,128,158,45,2,5,14,199,27,45,123,53,196,30,174,12,215,2,252,197,235,129,99,119,126,88,8,122,56,148,149,12,47,45,72,206,217,50,75,56,199,96,98,25,235,126,25,231,202,42,212,9,45,89,131,185,238,101,139,36,235,44,34,234,91,133,75,208,138,212,210,21,156,209,26,107,48,111,190,76,209,220,221,47,85,246,26,11,169,237,4,54,92,115,227,9,120,2,188,220,248,79,157,236,136,194,20,106,152,150,103,80,22,97,241,114,178,198,73,175,185,90,126,65,113,63,178,17,200,158,121,151,211,137,249,141,49,113,99,60,188,112,118,120,53,14,200,243,94,170,210,70,101,37,212,118,217,22,194,241,234,61,132,99,109,19,154,103,46,39,60,93,131,240,180,140,48,173,80,151,147,94,67,238,225,168,140,52,109,38,148,147,246,214,32,237,149,145,198,250,127,57,237,104,13,218,81,1,109,237,33,136,72,18,185,197,196,144,248,18,52,24,252,101,243,23,66,180,64,154,189,5,83,211,1,2,227,107,102,57,153,115,226,192,42,55,68,191,12,53,34,202,235,7,65,68,127,160,33,40,185,254,74,90,250,67,159,148,156,44,111,241,45,181,210,212,111,25,27,89,28,240,113,97,58,46,154,16,25,218,205,60,237,18,182,86,144,23,189,9,246,106,77,233,50,164,207,39,220,248,192,141,199,112,86,121,209,27,145,236,82,119,163,60,66,28,143,108,197,237,104,190,79,247,65,112,99,208,13,132,158,51,38,86,227,63,191,31,236,52,174,106,38,186,87,214,249,192,142,14,38,102,237,2,71,166,186,105,10,225,30,236,241,145,247,77,74,61,96,201,196,14,94,100,42,131,99,199,243,70,206,248,250,156,64,76,189,109,155,84,43,38,179,87,85,60,98,82,202,173,16,196,163,176,55,21,133,201,66,195,217,42,95,183,198,87,237,60,62,138,231,46,178,180,148,202,25,99,162,135,52,16,182,87,70,214,239,77,3,99,104,90,185,207,68,209,59,180,120,223,54,205,14,130,110,129,205,98,114,116,132,18,162,127,17,159,10,166,100,97,165,184,90,169,126,255,46,125,110,121,45,120,191,169,21,107,215,161,251,126,25,217,244,81,14,123,51,4,218,95,254,160,71,127,227,192,115,236,92,127,190,180,179,158,233,245,109,254,86,2,139,157,197,47,39,88,246,201,182,34,187,107,74,162,130,154,220,81,53,107,140,233,85,97,87,146,219,128,190,9,94,204,253,88,96,80,53,82,4,39,154,24,9,123,241,68,123,56,197,113,57,182,237,102,111,68,31,144,245,115,204,189,6,128,246,102,81,131,54,122,120,132,159,214,64,177,223,131,157,190,76,137,53,125,0,178,201,86,98,246,58,18,73,138,61,209,6,37,172,182,132,193,98,78,48,109,23,201,179,222,9,163,172,137,227,188,86,19,33,219,74,203,201,67,94,192,202,72,246,72,211,97,144,4,23,130,138,189,228,156,235,197,142,220,73,87,135,229,89,199,133,68,13,204,94,85,83,3,154,152,241,10,146,114,72,210,91,117,174,248,211,233,189,24,84,138,186,6,111,37,180,172,49,184,37,21,141,58,52,240,223,221,219,200,9,243,93,104,74,108,223,204,62,16,18,75,177,160,25,23,195,202,85,217,114,188,41,78,3,32,192,84,34,109,54,165,102,160,119,166,29,79,83,109,160,23,97,50,71,91,217,215,187,163,192,41,67,86,120,149,23,138,224,150,237,150,50,27,175,22,13,239,63,139,39,101,162,14,223,50,223,75,167,15,74,129,19,88,82,27,129,63,46,120,29,84,44,253,216,21,161,155,193,231,153,252,68,209,147,201,215,116,125,14,67,229,115,123,192,213,70,193,93,133,113,46,24,15,252,30,149,139,96,148,62,1,102,18,194,8,16,123,197,43,56,55,197,145,221,107,160,100,20,209,229,174,94,33,66,200,66,4,179,252,77,29,92,90,11,143,113,173,61,128,96,186,86,31,26,25,186,178,41,135,166,162,236,66,84,174,109,220,58,125,51,197,94,55,188,120,246,243,219,55,205,78,250,70,98,213,234,236,101,206,186,12,100,176,53,30,162,141,121,192,87,76,235,178,160,35,107,28,120,27,115,128,239,126,214,229,64,71,214,56,24,45,229,160,193,204,65,43,226,49,139,122,111,2,107,169,245,76,221,104,94,41,97,149,191,252,89,147,87,142,93,204,237,88,112,123,242,11,101,84,156,190,189,6,216,178,122,91,176,160,66,24,186,114,42,213,232,60,61,86,107,28,120,62,67,63,238,236,104,235,151,16,59,232,184,78,197,152,187,32,136,38,252,117,238,236,74,171,217,84,222,131,168,23,30,74,67,247,59,69,12,243,116,2,78,41,186,133,66,12,164,88,112,163,226,221,47,188,122,38,218,18,114,98,219,225,222,16,3,153,246,8,178,143,235,78,238,106,53,113,211,88,221,226,239,92,216,182,89,66,85,186,75,184,255,49,186,23,136,60,239,97,162,3,29,55,20,222,148,208,79,231,15,176,83,119,186,62,179,172,76,150,125,149,83,198,159,82,115,43,228,142,71,161,210,77,99,225,234,7,68,39,42,93,156,159,116,97,173,70,86,184,52,139,120,197,202,83,247,106,17,17,164,201,27,245,252,209,24,12,140,29,254,122,40,19,228,41,67,248,1,4,84,250,24,67,115,250,95,6,135,135,198,135,190,209,53,16,108,44,179,17,228,129,53,141,254,46,163,96,17,61,231,13,63,140,6,253,111,64,112,8,231,33,147,50,153,24,83,240,154,247,219,234,177,119,232,83,4,18,5,115,90,136,103,156,34,72,114,186,175,98,143,32,193,148,114,100,94,53,4,25,23,209,10,22,9,125,8,35,145,67,20,101,217,127,128,211,75,245,217,96,182,60,38,204,164,83,153,199,6,162,201,66,24,217,130,230,186,90,6,169,136,98,99,133,215,65,20,60,86,7,145,213,72,89,8,225,73,122,182,101,47,10,55,63,66,146,151,110,138,95,124,174,243,94,33,75,112,91,219,123,38,97,209,210,201,21,143,6,126,136,112,46,49,93,167,245,255,67,43,104,121,238,234,46,251,15,209,214,178,245,21,77,242,31,34,172,134,154,189,108,247,186,176,155,93,104,188,90,98,203,171,229,101,54,156,26,103,209,172,66,27,21,214,247,248,248,95,68,190,140,182,199,64,0,0 };

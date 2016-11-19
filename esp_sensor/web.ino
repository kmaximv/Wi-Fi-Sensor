#ifdef USE_WEBSERVER

//#define STR_HELPER(x) #x
//#define STR(x) STR_HELPER(x)




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         HTML SNIPPLETS

const char headerStartP[] PROGMEM = "<html lang='en'><head><title>";
//JConf.module_id
const char headerStart2P[] PROGMEM = "</title><meta charset='utf-8'>";

const char headerRefreshStatusP[] PROGMEM = "<META HTTP-EQUIV='Refresh' CONTENT='20; URL=/'>";

const char headerEndP[] PROGMEM = 
"<meta name='viewport' content='width=device-width, initial-scale=1'>\
<link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>\
<script src='https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js'></script>\
<script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script></head>";

const char javaScriptPinControlP[] PROGMEM = 
"<div id='content'></div>\
<div id='pin1'></div>\
<script>\
function show()\
{\
$.ajax({\
url: 'controlstatus',\
cache: false,\
success: function(html){\
$('#content').html(html);\
}\
});\
}\
function Pin1()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '1=1',\
success: function(data){\
show();\
}\
});\
}\
function Auto1()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '1=2',\
success: function(data){\
show();\
}\
});\
}\
function Pin2()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '2=1',\
success: function(data){\
show();\
}\
});\
}\
function Auto2()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '2=2',\
success: function(data){\
show();\
}\
});\
}\
$(document).ready(function(){\
show();\
setInterval('show()',5000);\
});\
</script>";

const char div1P[] PROGMEM =
"<div class='col-sm-8 col-md-6 col-lg-5'><h2>Control Pins</h2>\
<table class='table table-hover'>\
<tbody>\
  <tr>\
    <td class='active'><h4>Pins</h4></td>\
    <td class='active'></td><td class='active'></td>\
    <td class='active'><h4>Status</h4></td>\
    <td class='active'><h4>Mode</h4></td>";

const char div2P[] PROGMEM =
 "</tr>\
  <tr>\
    <td class='active'><h4>Light1</h4></td>\
    <td class='active'><div onclick='Pin1();'><input id='OnOff' type='submit' class='btn btn-";


const char navbarStartP[] PROGMEM = 
"<nav class='navbar navbar-inverse'><div class='container-fluid'><div class='navbar-header'>\
<a class='navbar-brand' href='/'>";
//JConf.module_id
const char navbarStart2P[] PROGMEM = "</a></div><div><ul class='nav navbar-nav'>";

const char navbarNonActiveP[] PROGMEM = "<li>";
const char navbarActiveP[] PROGMEM = "<li class='active'>";

const char navbarBeginP[] PROGMEM  = 
"<a href='/'><span class='glyphicon glyphicon-dashboard'></span> Status</a></li>\
<li><a href='/pincontrol'><span class='glyphicon glyphicon-tasks'></span> Control Pins</a></li>";

#if defined(UART_ON)
const char navbarUartP[] PROGMEM = 
"<li><a href='/analog'><span class='glyphicon glyphicon-tasks'></span> Analog Pins UART</a></li>";
#endif

const char navbarEndP[] PROGMEM =
"<li class='dropdown'><a class='dropdown-toggle' data-toggle='dropdown' href='#'>\
<span class='glyphicon glyphicon-cog'></span> Configure<span class='caret'></span></a><ul class='dropdown-menu'>\
<li><a href='/wificonf'>Wi-Fi</a></li>\
<li><a href='/sensorsconf'>Sensors</a></li>\
<li><a href='/espconf'>ESP</a></li>\
<li><a href='/mqttconf'>MQTT</a></li>\
<li><a href='/ntpconf'>NTP time</a></li>\
<li><a href='/update'>Update frimware</a></li>\
<li><a href='/reboot'>Reboot ESP</a></li>\
</ul></li></ul></div></div></nav>"; 

const char containerStartP[] PROGMEM    =  "<div class='container'><div class='row'>";
const char containerEndP[] PROGMEM      =  "<div class='clearfix visible-lg'></div></div></div>";
const char siteEndP[] PROGMEM         =  "</body></html>";
  
const char panelHeaderNameP[] PROGMEM     =  "<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>";
const char panelHeaderEndP[] PROGMEM    =  "</h2></div>";
const char panelEndP[] PROGMEM        =  "</div>";
  
const char panelBodySymbolP[] PROGMEM     =  
"<div class='panel panel-default'><div class='panel-body'><span class='glyphicon glyphicon-";

const char panelBodyNameP[] PROGMEM     =  "'></span> ";
const char panelBodyValueP[] PROGMEM    =  "<span class='pull-right'";
const char closingAngleBracketP[] PROGMEM    =  ">";

const char panelBodyEndP[] PROGMEM      =  "</span></div></div>";

const char inputBodyStartP[] PROGMEM    =  
"<form action='' method='POST'><div class='panel panel-default'><div class='panel-body'>";

const char inputBodyNameP[] PROGMEM     =  
"<div class='form-group'><div class='input-group'><span class='input-group-addon' id='basic-addon1'>";

const char inputBodyPOSTP[] PROGMEM     =  "</span><input type='text' name='";
const char inputPlaceHolderP[] PROGMEM  =  "' class='form-control' value='";

const char inputBodyCloseP[] PROGMEM    =  "' aria-describedby='basic-addon1'>";

const char inputBodyUnitStartP[] PROGMEM    =  "<span class='input-group-addon' id='basic-addon1'>";
const char inputBodyUnitEndP[] PROGMEM    =  "</span>";

const char inputBodyCloseDivP[] PROGMEM    =  "</div></div>";

const char inputBodyEndP[] PROGMEM      = 
"</div><div class='panel-footer clearfix'><div class='pull-right'>\
<button type='submit' class='btn btn-default' name='save_conf' value='1'>Save</button></div></div></div></form>";

const char sketchUploadFormP[] PROGMEM  = 
"<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>Update Frimware</h2></div>\
<form method='POST' action='/upload_sketch' enctype='multipart/form-data'>\
<p><input type='file' class='btn btn-primary' name='sketch'></p>\
<h3><small>Выберите файл формата *.bin</small></h3>\
<p><input type='submit' value='Upload' class='btn btn-danger'></p></form></div>";


const char ClassInfoP[] PROGMEM  = "info";
const char ClassDangerP[] PROGMEM  = "danger";
const char ClassDefaultP[] PROGMEM  = "default";
const char ClassSuccessP[] PROGMEM  = "success";

const char AUTOP[] PROGMEM  = "AUTO";
const char ONP[] PROGMEM  = "ON";
const char OFFP[] PROGMEM  = "OFF";


const char bodyAjaxP[] PROGMEM = "<body onload='process()'>";
const char bodyNonAjaxP[] PROGMEM = "<body>";



/*********************************************************************************************\
 * 
 * Template
 * 
\*********************************************************************************************/
const char HTTP_HEAD[] PROGMEM = 
"<html lang='en'>"
  "<head>"
    "<title>{module_id}</title>"
    "<meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>"
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js'></script>"
    "<script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script>"
  "</head>";


const char HTTP_BODY[] PROGMEM = 
  "<body>"
  "<nav class='navbar navbar-inverse'>"
    "<div class='container-fluid'>"
      "<div class='navbar-header'>"
        "<a class='navbar-brand' href='/'><span class='glyphicon glyphicon-home'></span> {module_id}</a>"
      "</div>"
      "<div>"
        "<ul class='nav navbar-nav'>"
          "<li><a href='/pincontrol'><span class='glyphicon glyphicon-tasks'></span> Control Pins</a></li>"
          "<li><a href='/cm'><span class='glyphicon glyphicon-pencil'></span> Console</a></li>"
          "<li class='dropdown'><a class='dropdown-toggle' data-toggle='dropdown' href='#'>"
          "<span class='glyphicon glyphicon-cog'></span> Configure<span class='caret'></span></a>"
            "<ul class='dropdown-menu'>"
              "<li><a href='/wificonf'>Wi-Fi</a></li>"
              "<li><a href='/sensorsconf'>Sensors</a></li>"
              "<li><a href='/espconf'>ESP</a></li>"
              "<li><a href='/mqttconf'>MQTT</a></li>"
              "<li><a href='/ntpconf'>NTP time</a></li>"
              "<li><a href='/log'>Logging</a></li>"
              "<li><a href='/update'>Update frimware</a></li>"
              "<li><a href='/reboot'>Reboot ESP</a></li>"
            "</ul>"
          "</li>"
        "</ul>"
      "</div>"
    "</div>"
  "</nav>"
  "<div class='container'>"
    "<div class='row'>";



const char HTTP_BODY_END[] PROGMEM =  
        "<div class='clearfix visible-lg'>"
        "</div>"
      "</div>"
    "</div>"
  "</body>"
"</html>";
/*********************************************************************************************\
 * 
 *                                                                                    Template
 * 
\*********************************************************************************************/



/*********************************************************************************************\
 * 
 * For Web Page <Root>
 * 
\*********************************************************************************************/

const char javaScriptP[] PROGMEM = 
"<SCRIPT>\
var xmlHttp=createXmlHttpObject();\
function createXmlHttpObject(){\
  if(window.XMLHttpRequest){\
    xmlHttp=new XMLHttpRequest();\
  }else{\
    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\
 }\
 return xmlHttp;\
}\
function process(){\
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\
   xmlHttp.open('PUT','xml',true);\
   xmlHttp.onreadystatechange=handleServerResponse;\
   xmlHttp.send(null);\
 }\
 setTimeout('process()',10000);\
}\
function handleServerResponse(){\
 if(xmlHttp.readyState==4 && xmlHttp.status==200){\
   xmlResponse=xmlHttp.responseXML;";

#if defined(UART_ON)
const char javaScript2P[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('apin0');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin0Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin1');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin1Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin2');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin2Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin3');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin3Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin4');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin4Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin5');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin5Id').innerHTML=message;";
#endif

const char jsTemperatureP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('temperature');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('temperatureId').innerHTML=message;";

const char jsHumidityP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('humidity');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('humidityId').innerHTML=message;";

const char jsIlluminanceP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('illuminance');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('illuminanceId').innerHTML=message;";

const char jsPressureP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('pressure');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pressureId').innerHTML=message;";


#if defined(PZEM_ON)
const char jsPzemP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('pzemVoltage');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemVoltageId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('pzemCurrent');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemCurrentId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('pzemPower');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemPowerId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('pzemEnergy');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemEnergyId').innerHTML=message;";
#endif


const char jsNtpP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('ntpTime');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('ntpTimeId').innerHTML=message;";

// Длина строки не должна быть больше 1024 символов
const char javaScriptEndP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('vcc');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('vccId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('uptime');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('uptimeId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('freeMemory');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('freeMemoryId').innerHTML=message;\
 }\
}\
</SCRIPT>";



const char HTTP_ROOT_SENSOR_START[] PROGMEM =
"<div class='col-sm-6 col-md-5 col-lg-4'>"
    "<div class='page-header'>"
        "<h2>Sensor Data</h2>"
    "</div>";

const char HTTP_ROOT_SENSOR_T_H[] PROGMEM =
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-fire'></span> Temperature<span class='pull-right' id='temperatureId'></span></div>"
    "</div>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-tint'></span> Humidity<span class='pull-right' id='humidityId'></span></div>"
    "</div>";

const char HTTP_ROOT_SENSOR_P[] PROGMEM =
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-cloud'></span> Pressure<span class='pull-right' id='pressureId'></span></div>"
    "</div>";

const char HTTP_ROOT_SENSOR_PZEM[] PROGMEM =
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-flash'></span> Voltage<span class='pull-right' id='pzemVoltageId'></span></div>"
    "</div>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-flash'></span> Current<span class='pull-right' id='pzemCurrentId'></span></div>"
    "</div>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-flash'></span> Power<span class='pull-right' id='pzemPowerId'></span></div>"
    "</div>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-flash'></span> Energy<span class='pull-right' id='pzemEnergyId'></span></div>"
    "</div>";

const char HTTP_ROOT_SENSOR_L[] PROGMEM =
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-asterisk'></span> illuminance<span class='pull-right' id='illuminanceId'></span></div>"
    "</div>";

const char HTTP_ROOT_SENSOR_END[] PROGMEM =
"</div>";



const char HTTP_ROOT_SETTINGS_START[] PROGMEM =
"<div class='col-sm-6 col-md-5 col-lg-4'>"
    "<div class='page-header'>"
        "<h2>Settings</h2>"
    "</div>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-globe'></span> IP Address<span class='pull-right'>192.168.2.121</span></div>"
    "</div>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-time'></span> Uptime<span class='pull-right' id='uptimeId'></span></div>"
    "</div>";

const char HTTP_ROOT_SETTINGS_NTP[] PROGMEM =
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-time'></span> NTP time<span class='pull-right' id='ntpTimeId'></span></div>"
    "</div>";

const char HTTP_ROOT_SETTINGS_END[] PROGMEM =
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-flash'></span> Voltage<span class='pull-right' id='vccId'></span></div>"
    "</div>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-flash'></span> Free Memory<span class='pull-right' id='freeMemoryId'></span></div>"
    "</div>"
"</div>";
/*********************************************************************************************\
 * 
 *                                                                         For Web Page <Root>
 * 
\*********************************************************************************************/



/*********************************************************************************************\
 * 
 * For Web Page <Log Config>
 * 
\*********************************************************************************************/
const char HTTP_FORM_LOG[] PROGMEM =
"<div class='col-sm-6 col-md-5 col-lg-4'>"
    "<div class='page-header'>"
        "<h2>Logging Configuration</h2>"
    "</div>"
    "<form method='post' action='save'>"
        "<div class='panel panel-default'>"
            "<div class='panel-body'>"
                "<div class='form-group'>"
                    "<div class='input-group'>"
                        "<input id='w' name='w' value='3' hidden>"
                        "<input id='r' name='r' value='0' hidden>"
                        "<span class='input-group-addon' id='basic-addon1'>Serial log level</span>"
                        "<select id='ls' name='ls' class='form-control' aria-describedby='basic-addon1'>"
                        "<option{a0value='0'>0 None</option>"
                        "<option{a1value='1'>1 Error</option>"
                        "<option{a2value='2'>2 Info</option>"
                        "<option{a3value='3'>3 Debug</option>"
                        "<option{a4value='4'>4 More debug</option>"
                        "</select>"
                    "</div>"
                "</div>"
                "<div class='form-group'>"
                    "<div class='input-group'>"                      
                        "<span class='input-group-addon' id='basic-addon1'>Web log level</span>"
                        "<select id='lw' name='lw' class='form-control' aria-describedby='basic-addon1'>"
                        "<option{b0value='0'>0 None</option>"
                        "<option{b1value='1'>1 Error</option>"
                        "<option{b2value='2'>2 Info</option>"
                        "<option{b3value='3'>3 Debug</option>"
                        "<option{b4value='4'>4 More debug</option>"
                        "</select>" 
                    "</div>"
                "</div>"
                "<hr>"
                "<h4>Syslog Settings</h4>"
                "<div class='form-group'>"
                    "<div class='input-group'>"                      
                        "<span class='input-group-addon' id='basic-addon1'>Syslog level</span>"
                        "<select id='ll' name='ll' class='form-control' aria-describedby='basic-addon1'>"
                        "<option{c0value='0'>0 None</option>"
                        "<option{c1value='1'>1 Error</option>"
                        "<option{c2value='2'>2 Info</option>"
                        "<option{c3value='3'>3 Debug</option>"
                        "<option{c4value='4'>4 More debug</option>"
                        "</select>" 
                    "</div>"                  
                "</div>"
                "<div class='form-group'>"
                    "<div class='input-group'>"
                        "<span class='input-group-addon' id='basic-addon1'>Syslog host</span>"
                        "<input type='text'  id='lh' name='lh' length=32 placeholder='{sys_log_host}' value='{sys_log_host}' class='form-control' aria-describedby='basic-addon1'>"
                    "</div>"
                "</div>"
                "<div class='form-group'>"
                    "<div class='input-group'>"
                        "<span class='input-group-addon' id='basic-addon1'>Syslog port</span>"
                        "<input type='text'  id='lp' name='lp' length=5 placeholder='{sys_log_port}' value='{sys_log_port}' class='form-control' aria-describedby='basic-addon1'>"
                    "</div>"
                "</div>"
            "</div>"
            "<div class='panel-footer clearfix'>"
                "<div class='pull-right'><button type='submit' class='btn btn-default'>Save</button></div>"
            "</div>"
        "</div>"
    "</form>"
    "<div class='clearfix visible-lg'></div>"
"</div>";
/*********************************************************************************************\
 * 
 *                                                                   For Web Page <Log Config>
 * 
\*********************************************************************************************/



/*********************************************************************************************\
 * 
 * For Web Page <Console>
 * 
\*********************************************************************************************/
const char HTTP_CONSOLE[] PROGMEM =
"<div class='col-sm-12 col-md-9 col-lg-8'>"
    "<div class='page-header'>"
        "<h3>Console</h3>"
    "</div>"
    "<textarea class='form-control' rows='30' readonly id='t1' name='t1' cols='50' wrap='off'></textarea>"
    "<form class='form-horizontal' role='form'  method='post' action='cm'>"
      "<br>"
      "<div>"
          "<input type='text'  id='" SUB_PREFIX "' name='" SUB_PREFIX "' length=80 placeholder='Enter command' class='form-control' autofocus>"
      "</div>"
    "</form>"
    "<div class='clearfix visible-lg'></div>"
"</div>";

const char JS_CONSOLE[] PROGMEM =
  "<script>"
  "var cn=120;"
  "function u(){"
    "if(cn>=0){"
      "document.getElementById('t').innerHTML='Restart in '+cn+' seconds';"
      "cn--;"
      "setTimeout(u,1000);"
    "}"
  "}"
  "function c(l){"
    "document.getElementById('s').value=l.innerText||l.textContent;"
    "document.getElementById('p').focus();"
  "}"
  "var sn=0;"
  "function l(){"
    "var e=document.getElementById('t1');"
    "if(e.scrollTop>=sn){"
      "var x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "e.value=x.responseText;"
          "e.scrollTop=100000;"
          "sn=e.scrollTop;"
        "}"
      "};"
      "x.open('GET','ax',true);"
      "x.send();"
    "}"
    "setTimeout(l,2000);"
  "}"
  "</script>";
/*********************************************************************************************\
 * 
 *                                                                      For Web Page <Console>
 * 
\*********************************************************************************************/



void showPage(String &page)
{
  WebServer.send(200, "text/html", page);
}



void handleRoot()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleRoot Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String body = FPSTR(HTTP_BODY);
  body.replace("<body>", "<body onload='process()'>");
  body.replace("{module_id}", String(JConf.module_id));

//JavaScript//////////////////////////////////////////////////////////////////////////////////////////////
  String js = FPSTR(javaScriptP);

  if (atoi(JConf.bme280_enable) == 1 || atoi(JConf.sht21_enable) == 1 || atoi(JConf.dht_enable) == 1) {
    js += FPSTR(jsTemperatureP);
    js += FPSTR(jsHumidityP);
  }
  if (atoi(JConf.bme280_enable) == 1) {
    js += FPSTR(jsPressureP);
  }
  if (atoi(JConf.bh1750_enable) == 1) {
    js += FPSTR(jsIlluminanceP);
  }

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      js += FPSTR(jsPzemP);
    }
  #endif //PZEM_ON

  if (atoi(JConf.ntp_enable) == 1) {
    js += FPSTR(jsNtpP);
  }

  js += FPSTR(javaScriptEndP);

//Sensors//////////////////////////////////////////////////////////////////////////////////////////////////
  String sensors = FPSTR(HTTP_ROOT_SENSOR_START);

  if (atoi(JConf.bme280_enable) == 1 || atoi(JConf.sht21_enable) == 1 || atoi(JConf.dht_enable) == 1 ){
    sensors += FPSTR(HTTP_ROOT_SENSOR_T_H);
  }

  #ifdef BME280_ON
    if (atoi(JConf.bme280_enable) == 1){
      sensors += FPSTR(HTTP_ROOT_SENSOR_P);
    }
  #endif //BME280_ON

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      sensors += FPSTR(HTTP_ROOT_SENSOR_PZEM);
    }
  #endif //PZEM_ON

  if (atoi(JConf.bh1750_enable) == 1){
    sensors += FPSTR(HTTP_ROOT_SENSOR_L);
  }

  sensors += FPSTR(HTTP_ROOT_SENSOR_END);

//Settings/////////////////////////////////////////////////////////////////////////////////////////////////
  String settings = FPSTR(HTTP_ROOT_SETTINGS_START);

  if (atoi(JConf.ntp_enable) == 1) {
    settings += FPSTR(HTTP_ROOT_SETTINGS_NTP);
  }

  settings += FPSTR(HTTP_ROOT_SETTINGS_END);
/////////////////////////////////////////////////////////////////////////////////////////////////Settings//

  String page = head + body + js + sensors + settings + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleRoot load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handleLogConfig()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleLogConfig Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));
  
  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String settings = FPSTR(HTTP_FORM_LOG);
  settings.replace("{serial_log_level}", String(JConf.serial_log_level));
  settings.replace("{web_log_level}", String(JConf.web_log_level));
  settings.replace("{sys_log_level}", String(JConf.sys_log_level));
  settings.replace("{sys_log_host}", String(JConf.sys_log_host));
  settings.replace("{sys_log_port}", String(JConf.sys_log_port));
  for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
    settings.replace("{a" + String(i), (i == atoi(JConf.serial_log_level)) ? " selected " : " ");
    settings.replace("{b" + String(i), (i == atoi(JConf.web_log_level)) ? " selected " : " ");
    settings.replace("{c" + String(i), (i == atoi(JConf.sys_log_level)) ? " selected " : " ");
  }

  String page = head + body + settings + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleLogConfig load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}






void handleConsole()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle console"));

  if (strlen(WebServer.arg(SUB_PREFIX).c_str())) {
    snprintf_P(svalue, sizeof(svalue), WebServer.arg(SUB_PREFIX).c_str());
    //do_cmnd(svalue);
  }

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String body = FPSTR(HTTP_BODY);
  body.replace("<li><a href='/cm'>", "<li class='active'><a href='/cm'>");
  body.replace("<body>", "<body onload='l()'>");
  body.replace("{module_id}", String(JConf.module_id));

  String page = head + body + FPSTR(JS_CONSOLE) + FPSTR(HTTP_CONSOLE) + FPSTR(HTTP_BODY_END);

  showPage(page);
}

void handleAjax()
{
  String message = "";

  byte counter = logidx;  // Points to oldest entry
  do {
    if (Log[counter].length()) {
      if (message.length()) message += F("\n");
      message += Log[counter];
    }
    counter++;
    if (counter > MAX_LOG_LINES -1) counter = 0;
  } while (counter != logidx);
  WebServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  WebServer.sendHeader("Pragma", "no-cache");
  WebServer.sendHeader("Expires", "-1");
  WebServer.send(200, "text/plain", message);
}























void handleSave()
{
  char log[LOGSZ];
  byte what = 0, restart;

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Parameter save"));

  if (strlen(WebServer.arg("w").c_str())) what = atoi(WebServer.arg("w").c_str());
  switch (what) {
  case 1:
    break;
  case 2:
    break;
  case 3:  //Logging Config save
    strlcpy(JConf.serial_log_level, (!strlen(WebServer.arg("ls").c_str())) ? JConf.serial_log_level : WebServer.arg("ls").c_str(), sizeof(JConf.serial_log_level));
    strlcpy(JConf.web_log_level, (!strlen(WebServer.arg("lw").c_str())) ? JConf.web_log_level : WebServer.arg("lw").c_str(), sizeof(JConf.web_log_level));
    strlcpy(JConf.sys_log_level, (!strlen(WebServer.arg("ll").c_str())) ? JConf.sys_log_level : WebServer.arg("ll").c_str(), sizeof(JConf.sys_log_level));
    strlcpy(JConf.sys_log_host, (!strlen(WebServer.arg("lh").c_str())) ? JConf.sys_log_host : WebServer.arg("lh").c_str(), sizeof(JConf.sys_log_host));
    strlcpy(JConf.sys_log_port, (!strlen(WebServer.arg("lp").c_str())) ? JConf.sys_log_port : WebServer.arg("lp").c_str(), sizeof(JConf.sys_log_port));

    snprintf_P(log, sizeof(log), PSTR("HTTP: Logging Seriallog %s, Weblog %s, Syslog %s, Host %s, Port %s"),
      JConf.serial_log_level, JConf.web_log_level, JConf.sys_log_level, JConf.sys_log_host, JConf.sys_log_port);
    addLog(LOG_LEVEL_INFO, log);
    JConf.saveConfig();
    handleLogConfig();
    break;
  }
}



















//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////   WEB PAGES  Start  //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


void WebReboot(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebReboot Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);

  String data = String(F("<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>Reboot ESP</h2></div><div class='alert alert-info' role='alert'><a href='#' class='alert-link'>Rebooting...</a></div></div>"));
  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerRefreshStatus + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebReboot load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  restartESP();
}



void WebUpdate(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebUpdate Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String sketchUploadForm;      sketchUploadForm += FPSTR(sketchUploadFormP);

  WebServer.send(200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + sketchUploadForm + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebUpdate load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebFileUpload(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebFileUpload Start");

  if (WebServer.uri() != "/upload_sketch") return;
  HTTPUpload& upload = WebServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();

    snprintf_P(log, sizeof(log), PSTR("WebFileUpload: Sketch: %s"), upload.filename.c_str());
    addLog(LOG_LEVEL_INFO, log);

    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace)) { //start with max available size
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { //true to set the size to the current progress
      snprintf_P(log, sizeof(log), PSTR("WebFileUpload: Update Success: %u"), upload.totalSize);
      addLog(LOG_LEVEL_INFO, log);
    } else {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  }
  yield();

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebFileUpload load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebUploadSketch(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebUploadSketch Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);

  String varDataString = String(F("<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>Update Frimware</h2></div><div class='alert alert-success'>")) + ((Update.hasError()) ? String(F("FAIL")) : String(F("Update Frimware: OK"))) + String(F("</div></div>"));


  WebServer.send(200, "text/html", headerStart + JConf.module_id + headerStart2 + headerRefreshStatus + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + varDataString + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebUploadSketch load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  restartESP();
}



void WebWiFiConf(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebWiFiConf Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);
  String panelEnd;              panelEnd += FPSTR(panelEndP);
  String panelBodySymbol;       panelBodySymbol += FPSTR(panelBodySymbolP);
  String panelBodyName;         panelBodyName += FPSTR(panelBodyNameP);
  String panelBodyValue;        panelBodyValue += FPSTR(panelBodyValueP);
  String panelBodyEnd;          panelBodyEnd += FPSTR(panelBodyEndP);
  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  bool config_changed = false;
  bool enable = false;
  String payload = "";

  payload=WebServer.arg("module_id");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.module_id, sizeof(JConf.module_id));
    config_changed = true;
  }

  payload=WebServer.arg("wifi_mode");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_mode, sizeof(JConf.wifi_mode));
    config_changed = true;
  }

  payload=WebServer.arg("wifi_phy_mode");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_phy_mode, sizeof(JConf.wifi_phy_mode));
    config_changed = true;
  }
  payload=WebServer.arg("wifi_channel");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_channel, sizeof(JConf.wifi_channel));
    config_changed = true;
  }
  payload=WebServer.arg("wifi_auth");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_auth, sizeof(JConf.wifi_auth));
    config_changed = true;
  }

  payload=WebServer.arg("sta_ssid");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.sta_ssid, sizeof(JConf.sta_ssid));
    config_changed = true;
  }

  payload=WebServer.arg("sta_pwd");
  if (payload.length() > 7 &&  payload != String(F("********"))) {
    payload.toCharArray(JConf.sta_pwd, sizeof(JConf.sta_pwd));
    config_changed = true;
  }

  payload=WebServer.arg("ap_pwd");
  if (payload.length() > 7 &&  payload != String(F("********"))) {
    payload.toCharArray(JConf.ap_pwd, sizeof(JConf.ap_pwd));
    config_changed = true;
  }

  payload=WebServer.arg("static_ip_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.static_ip_enable, sizeof(JConf.static_ip_enable));
    config_changed = true;
    enable = true;
  }

  payload=WebServer.arg("static_ip");
  if (payload.length() > 6 ) {
    payload.toCharArray(JConf.static_ip, sizeof(JConf.static_ip));
    config_changed = true;
  }

  payload=WebServer.arg("static_gateway");
  if (payload.length() > 6 ) {
    payload.toCharArray(JConf.static_gateway, sizeof(JConf.static_gateway));
    config_changed = true;
  }

  payload=WebServer.arg("static_subnet");
  if (payload.length() > 6 ) {
    payload.toCharArray(JConf.static_subnet, sizeof(JConf.static_subnet));
    config_changed = true;
  }

  if (config_changed){
    if (!enable){
      JConf.static_ip_enable[0] = '0';
      JConf.static_ip_enable[1] = '\0';
    }
    timer.deleteTimer(wifiReconnectTimer);
    JConf.saveConfig();
  }

  String data = panelHeaderName + String(F("Wi-Fi Configuration")) + panelHeaderEnd;
  data += inputBodyStart;

  data += inputBodyName + String(F("Module ID")) + inputBodyPOST + String(F("module_id"))  + inputPlaceHolder + JConf.module_id + inputBodyClose + inputBodyCloseDiv;

  data += String(F("<div class='form-group'><div class='input-group'><span class='input-group-addon'>AP type</span>"));
  if (atoi(JConf.wifi_mode) == STA){
    data += String(F("<select class='form-control' name='wifi_mode'><option value='0'>AP</option><option value='1' selected>STA</option><option value='2'>AP_STA</option></select></div></div>"));
  } else if (atoi(JConf.wifi_mode) == AP_STA){
    data += String(F("<select class='form-control' name='wifi_mode'><option value='0'>AP</option><option value='1'>STA</option><option value='2' selected>AP_STA</option></select></div></div>"));
  } else {
    data += String(F("<select class='form-control' name='wifi_mode'><option value='0' selected>AP</option><option value='1'>STA</option><option value='2'>AP_STA</option></select></div></div>"));
  }

  if ( atoi(JConf.wifi_mode) != STA){
    data += String(F("<div class='form-group'><div class='input-group'><span class='input-group-addon'>AP mode</span>"));
    if (atoi(JConf.wifi_phy_mode) == G){
      data += String(F("<select class='form-control' name='wifi_phy_mode'><option value='0'>11B</option><option value='1' selected>11G</option><option value='2'>11N</option></select></div></div>"));
    } else if (atoi(JConf.wifi_phy_mode) == N){
      data += String(F("<select class='form-control' name='wifi_phy_mode'><option value='0'>11B</option><option value='1'>11G</option><option value='2' selected>11N</option></select></div></div>"));
    } else {
      data += String(F("<select class='form-control' name='wifi_phy_mode'><option value='0' selected>11B</option><option value='1'>11G</option><option value='2'>11N</option></select></div></div>"));
    }

    data += inputBodyName + String(F("AP Channel")) + inputBodyPOST + String(F("wifi_channel"))  + inputPlaceHolder + JConf.wifi_channel + inputBodyClose + inputBodyCloseDiv;

    data += String(F("<div class='form-group'><div class='input-group'><span class='input-group-addon'>AP auth</span>"));
    if (atoi(JConf.wifi_auth) == WPA_PSK){
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0'>OPEN</option><option value='1' selected>WPA_PSK</option><option value='2'>WPA2_PSK</option><option value='3'>WPA_WPA2_PSK</option></select></div></div>"));
    } else if (atoi(JConf.wifi_auth) == WPA2_PSK){
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0'>OPEN</option><option value='1'>WPA_PSK</option><option value='2' selected>WPA2_PSK</option><option value='3'>WPA_WPA2_PSK</option></select></div></div>"));
    } else if (atoi(JConf.wifi_auth) == WPA_WPA2_PSK){
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0'>OPEN</option><option value='1'>WPA_PSK</option><option value='2'>WPA2_PSK</option><option value='3' selected>WPA_WPA2_PSK</option></select></div></div>"));
    } else {
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0' selected>OPEN</option><option value='1'>WPA_PSK</option><option value='2'>WPA2_PSK</option><option value='3'>WPA_WPA2_PSK</option></select></div></div>"));
    }

    if (atoi(JConf.wifi_auth) != OPEN){
      data += inputBodyName + String(F("AP Password")) + String(F("</span><input type='password' name='")) + String(F("ap_pwd")) + inputPlaceHolder + String(F("********")) + inputBodyClose + inputBodyCloseDiv;
    }
  }

  if ( atoi(JConf.wifi_mode) != AP){
    data += inputBodyName + String(F("STA SSID")) + inputBodyPOST + String(F("sta_ssid"))  + inputPlaceHolder + JConf.sta_ssid + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("STA Password")) + String(F("</span><input type='password' name='")) + String(F("sta_pwd")) + inputPlaceHolder + String(F("********")) + inputBodyClose + inputBodyCloseDiv;

    if (atoi(JConf.static_ip_enable) == 1){
      data += String(F("<div class='checkbox'><label><input type='checkbox' name='static_ip_enable' value='1' checked='true'>Static IP Mode</label></div>"));
      data += inputBodyName + String(F("Static IP"))      + inputBodyPOST + String(F("static_ip"))      + inputPlaceHolder + JConf.static_ip      + inputBodyClose + inputBodyCloseDiv;
      data += inputBodyName + String(F("Static Gateway")) + inputBodyPOST + String(F("static_gateway")) + inputPlaceHolder + JConf.static_gateway + inputBodyClose + inputBodyCloseDiv;
      data += inputBodyName + String(F("Static Subnet"))  + inputBodyPOST + String(F("static_subnet"))  + inputPlaceHolder + JConf.static_subnet  + inputBodyClose + inputBodyCloseDiv;
    } else {
      data += String(F("<div class='checkbox'><label><input type='checkbox' name='static_ip_enable' value='1'>Static IP Mode</label></div>"));
    }
  }

  data += inputBodyEnd;
  data += String(F("</div>"));

  data += panelHeaderName + String(F("Scan AP")) + panelHeaderEnd;
  data += network_html;

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebWiFiConf load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebSensorsConf(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebSensorsConf Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("Sensors Configuration"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  bool config_changed = false;

  bool dht_enable = false;
  bool bme280_enable = false;
  bool sht21_enable = false;
  bool bh1750_enable = false;
  bool motion_sensor_enable = false;
  bool pzem_enable = false;

  String payload = "";

  payload=WebServer.arg("save_conf");
  if (payload.length() > 0) {
    config_changed = true;
  } 

  payload=WebServer.arg("bme280_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.bme280_enable, sizeof(JConf.bme280_enable));
    bme280_enable = true;
  } 

  payload=WebServer.arg("sht21_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.sht21_enable, sizeof(JConf.sht21_enable));
    sht21_enable = true;
  } 

  payload=WebServer.arg("dht_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.dht_enable, sizeof(JConf.dht_enable));
    dht_enable = true;
  } 

  payload=WebServer.arg("bh1750_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.bh1750_enable, sizeof(JConf.bh1750_enable));
    bh1750_enable = true;
  } 

  payload=WebServer.arg("motion_sensor_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.motion_sensor_enable, sizeof(JConf.motion_sensor_enable));
    motion_sensor_enable = true;
  } 

  payload=WebServer.arg("pzem_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.pzem_enable, sizeof(JConf.pzem_enable));
    pzem_enable = true;
  } 


  if (config_changed){
    if (!bme280_enable){
      JConf.bme280_enable[0] = '0';
      JConf.bme280_enable[1] = '\0';
    }
    if (!sht21_enable){
      JConf.sht21_enable[0] = '0';
      JConf.sht21_enable[1] = '\0';
    }
    if (!dht_enable){
      JConf.dht_enable[0] = '0';
      JConf.dht_enable[1] = '\0';
    }
    if (!bh1750_enable){
      JConf.bh1750_enable[0] = '0';
      JConf.bh1750_enable[1] = '\0';
      luxString = "0";
    }
    if (!motion_sensor_enable){
      JConf.motion_sensor_enable[0] = '0';
      JConf.motion_sensor_enable[1] = '\0';
    }
    if (!pzem_enable){
      JConf.pzem_enable[0] = '0';
      JConf.pzem_enable[1] = '\0';
    }
    JConf.saveConfig();
  }

  if (atoi(JConf.bme280_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bme280_enable' value='1' checked='true'>BME280</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bme280_enable' value='1'>BME280</label></div>"));
  }

  if (atoi(JConf.sht21_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='sht21_enable' value='1' checked='true'>SHT21</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='sht21_enable' value='1'>SHT21</label></div>"));
  }

  if (atoi(JConf.dht_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='dht_enable' value='1' checked='true'>DHT</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='dht_enable' value='1'>DHT</label></div>"));
  }

  if (atoi(JConf.bh1750_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bh1750_enable' value='1' checked='true'>BH1750</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bh1750_enable' value='1'>BH1750</label></div>"));
  }

  if (atoi(JConf.motion_sensor_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='motion_sensor_enable' value='1' checked='true'>Motion Sensor</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='motion_sensor_enable' value='1'>Motion Sensor</label></div>"));
  }

  if (atoi(JConf.pzem_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='pzem_enable' value='1' checked='true'>Energy Monitor</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='pzem_enable' value='1'>Energy Monitor</label></div>"));
  }

  data += inputBodyEnd;

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebSensorsConf load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebEspConf(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebEspConf Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);
  String panelEnd;              panelEnd += FPSTR(panelEndP);
  String panelBodySymbol;       panelBodySymbol += FPSTR(panelBodySymbolP);
  String panelBodyName;         panelBodyName += FPSTR(panelBodyNameP);
  String panelBodyValue;        panelBodyValue += FPSTR(panelBodyValueP);
  String panelBodyEnd;          panelBodyEnd += FPSTR(panelBodyEndP);
  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  bool config_changed = false;
  bool enable_light_smooth = false;
  bool enable_light2_smooth = false;
  String payload = "";

  payload=WebServer.arg("light_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light_pin, sizeof(JConf.light_pin));
    config_changed = true;
  }

  payload=WebServer.arg("lightoff_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.lightoff_delay, sizeof(JConf.lightoff_delay));
    if (atoi(JConf.mqtt_enable) == 1){
      MqttPubLightOffDelay();
    }
    config_changed = true;
  }

  payload=WebServer.arg("lighton_lux");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.lighton_lux, sizeof(JConf.lighton_lux));
    config_changed = true;
  }

  payload=WebServer.arg("light_smooth");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light_smooth, sizeof(JConf.light_smooth));
    config_changed = true;
    enable_light_smooth = true;
  }

  payload=WebServer.arg("light2_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2_pin, sizeof(JConf.light2_pin));
    config_changed = true;
  }

  payload=WebServer.arg("light2off_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2off_delay, sizeof(JConf.light2off_delay));
    if (atoi(JConf.mqtt_enable) == 1){
      MqttPubLightOffDelay();
    }
    config_changed = true;
  }

  payload=WebServer.arg("light2on_lux");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2on_lux, sizeof(JConf.light2on_lux));
    config_changed = true;
  }

  payload=WebServer.arg("light2_smooth");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2_smooth, sizeof(JConf.light2_smooth));
    config_changed = true;
    enable_light2_smooth = true;
  }

  payload=WebServer.arg("motion_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.motion_pin, sizeof(JConf.motion_pin));
    config_changed = true;
  }

  payload=WebServer.arg("dht_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.dht_pin, sizeof(JConf.dht_pin));
    config_changed = true;
  }

  payload=WebServer.arg("get_data_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.get_data_delay, sizeof(JConf.get_data_delay));
    config_changed = true;
  }

  payload=WebServer.arg("motion_read_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.motion_read_delay, sizeof(JConf.motion_read_delay));
    config_changed = true;
  }

  payload=WebServer.arg("reboot_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.reboot_delay, sizeof(JConf.reboot_delay));
    config_changed = true;
  }

  if (config_changed){
    if (!enable_light_smooth){
      JConf.light_smooth[0] = '0';
      JConf.light_smooth[1] = '\0';
    }
    if (!enable_light2_smooth){
      JConf.light2_smooth[0] = '0';
      JConf.light2_smooth[1] = '\0';
    }
    JConf.saveConfig();
  }

  String data = panelHeaderName + String(F("ESP Configuration")) + panelHeaderEnd;
  data += inputBodyStart;

  data += String(F("<h4>Light 1</h4>"));
  data += inputBodyName + String(F("Pin")) + inputBodyPOST + String(F("light_pin")) + inputPlaceHolder + JConf.light_pin + inputBodyClose + inputBodyCloseDiv;
  data += inputBodyName + String(F("Off Delay")) + inputBodyPOST + String(F("lightoff_delay")) + inputPlaceHolder + JConf.lightoff_delay + inputBodyClose + inputBodyUnitStart + String(F("min")) + inputBodyUnitEnd + inputBodyCloseDiv;
  if (atoi(JConf.bh1750_enable) == 1){
    data += inputBodyName + String(F("On Lux")) + inputBodyPOST + String(F("lighton_lux")) + inputPlaceHolder + JConf.lighton_lux + inputBodyClose + inputBodyUnitStart + String(F("Lux")) + inputBodyUnitEnd + inputBodyCloseDiv;
  }

  if (atoi(JConf.light_smooth) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light_smooth' value='1' checked='true'>Smooth Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light_smooth' value='1'>Smooth Enable</label></div>"));
  }
  data += String(F("<hr>"));

  data += String(F("<h4>Light 2</h4>"));
  data += inputBodyName + String(F("Pin")) + inputBodyPOST + String(F("light2_pin")) + inputPlaceHolder + JConf.light2_pin + inputBodyClose + inputBodyCloseDiv;
  data += inputBodyName + String(F("Off Delay")) + inputBodyPOST + String(F("light2off_delay")) + inputPlaceHolder + JConf.light2off_delay + inputBodyClose + inputBodyUnitStart + String(F("min")) + inputBodyUnitEnd + inputBodyCloseDiv;
  if (atoi(JConf.bh1750_enable) == 1){
    data += inputBodyName + String(F("On Lux")) + inputBodyPOST + String(F("light2on_lux")) + inputPlaceHolder + JConf.light2on_lux + inputBodyClose + inputBodyUnitStart + String(F("Lux")) + inputBodyUnitEnd + inputBodyCloseDiv;
  }

  if (atoi(JConf.light2_smooth) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light2_smooth' value='1' checked='true'>Smooth Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light2_smooth' value='1'>Smooth Enable</label></div>"));
  }
  data += String(F("<hr>"));

  if (atoi(JConf.motion_sensor_enable) == 1){
    data += inputBodyName + String(F("Motion Pin")) + inputBodyPOST + String(F("motion_pin")) + inputPlaceHolder + JConf.motion_pin + inputBodyClose + inputBodyCloseDiv;
  }

  if (atoi(JConf.dht_enable) == 1){
    data += inputBodyName + String(F("DHT Pin")) + inputBodyPOST + String(F("dht_pin")) + inputPlaceHolder + JConf.dht_pin + inputBodyClose + inputBodyCloseDiv;
  }
  data += String(F("<br>"));

  data += inputBodyName + String(F("Update Data Delay")) + inputBodyPOST + String(F("get_data_delay")) + inputPlaceHolder + JConf.get_data_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;

  if (atoi(JConf.motion_sensor_enable) == 1){
    data += inputBodyName + String(F("Motion Read Delay")) + inputBodyPOST + String(F("motion_read_delay")) + inputPlaceHolder + JConf.motion_read_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;
  }

  #ifdef REBOOT_ON
    data += inputBodyName + String(F("Reboot Delay")) + inputBodyPOST + String(F("reboot_delay")) + inputPlaceHolder + JConf.reboot_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;
  #endif

  data += inputBodyEnd;

  WebServer.send( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebEspConf load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebMqttConf(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebMqttConf Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("MQTT Configuration"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  bool config_changed = false;
  bool enable = false;
  bool enable_auth = false;
  String payload = "";

  payload=WebServer.arg("mqtt_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.mqtt_enable, sizeof(JConf.mqtt_enable));
    config_changed = true;
    enable = true;
  } 

  payload=WebServer.arg("mqtt_server");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.mqtt_server, sizeof(JConf.mqtt_server));
    config_changed = true;
  }

  payload=WebServer.arg("mqtt_port");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.mqtt_port, sizeof(JConf.mqtt_port));
    config_changed = true;
  }

  payload=WebServer.arg("mqtt_auth_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.mqtt_auth_enable, sizeof(JConf.mqtt_auth_enable));
    config_changed = true;
    enable_auth = true;
  }

  payload=WebServer.arg("mqtt_user");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.mqtt_user, sizeof(JConf.mqtt_user));
    config_changed = true;
  }

  payload=WebServer.arg("mqtt_pwd");
  if (payload.length() > 0 &&  payload != String(F("********"))) {
    payload.toCharArray(JConf.mqtt_pwd, sizeof(JConf.mqtt_pwd));
    config_changed = true;
  }

  payload=WebServer.arg("mqtt_name");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.mqtt_name, sizeof(JConf.mqtt_name));
    config_changed = true;
  }

  payload=WebServer.arg("publish_topic");
  if (payload.length() > 0 ) {
    payload.replace("%2F", String(F("/")));
    payload.toCharArray(JConf.publish_topic, sizeof(JConf.publish_topic));
    config_changed = true;
  }

  payload=WebServer.arg("subscribe_topic");
  if (payload.length() > 0 ) {
    payload.replace("%2F", String(F("/")));
    payload.toCharArray(JConf.subscribe_topic, sizeof(JConf.subscribe_topic));
    config_changed = true;
  }

  payload=WebServer.arg("publish_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.publish_delay, sizeof(JConf.publish_delay));
    config_changed = true;
  }

  payload=WebServer.arg("subscribe_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.subscribe_delay, sizeof(JConf.subscribe_delay));
    config_changed = true;
  }

  if (config_changed){
    if (!enable){
      JConf.mqtt_enable[0] = '0';
      JConf.mqtt_enable[1] = '\0';
    }
    if (!enable_auth){
      JConf.mqtt_auth_enable[0] = '0';
      JConf.mqtt_auth_enable[1] = '\0';
    }
    JConf.saveConfig();
  }

  if (atoi(JConf.mqtt_enable) == 1){

    data += String(F("<div class='checkbox'><label><input type='checkbox' name='mqtt_enable' value='1' checked='true'>MQTT Enable</label></div>"));
    data += inputBodyName + String(F("Server MQTT")) + inputBodyPOST + String(F("mqtt_server")) + inputPlaceHolder + JConf.mqtt_server + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Port MQTT")) + inputBodyPOST + String(F("mqtt_port")) + inputPlaceHolder + JConf.mqtt_port + inputBodyClose + inputBodyCloseDiv;

    data += String(F("<hr>"));
    if (atoi(JConf.mqtt_auth_enable) == 1){
      data += String(F("<div class='checkbox'><label><input type='checkbox' name='mqtt_auth_enable' value='1' checked='true'>MQTT Authentication</label></div>"));
      data += inputBodyName + String(F("MQTT User")) + inputBodyPOST + String(F("mqtt_user")) + inputPlaceHolder + JConf.mqtt_user + inputBodyClose + inputBodyCloseDiv;
      data += inputBodyName + String(F("MQTT Password")) + String(F("</span><input type='password' name='")) + String(F("mqtt_pwd")) + inputPlaceHolder + String(F("********")) + inputBodyClose + inputBodyCloseDiv;
    } else {
      data += String(F("<div class='checkbox'><label><input type='checkbox' name='mqtt_auth_enable' value='1'>MQTT Authentication</label></div>"));
    }
    data += String(F("<hr>"));

    data += inputBodyName + String(F("MQTT Postfix")) + inputBodyPOST + String(F("mqtt_name")) + inputPlaceHolder + JConf.mqtt_name + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Publish Topic")) + inputBodyPOST + String(F("publish_topic")) + inputPlaceHolder + JConf.publish_topic + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Subscribe Topic")) + inputBodyPOST + String(F("subscribe_topic")) + inputPlaceHolder + JConf.subscribe_topic + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Publish Delay")) + inputBodyPOST + String(F("publish_delay")) + inputPlaceHolder + JConf.publish_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;
    data += inputBodyName + String(F("Subscribe Delay")) + inputBodyPOST + String(F("subscribe_delay")) + inputPlaceHolder + JConf.subscribe_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;

  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='mqtt_enable' value='1'>MQTT Enable</label></div>"));
  }

  data += inputBodyEnd;

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebMqttConf load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebNTPConf(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebNTPConf Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("NTP Configuration"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  bool config_changed = false;
  bool enable = false;
  String payload = "";

  payload=WebServer.arg("ntp_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.ntp_enable, sizeof(JConf.ntp_enable));
    config_changed = true;
    enable = true;
  } 

  payload=WebServer.arg("ntp_server");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.ntp_server, sizeof(JConf.ntp_server));
    config_changed = true;
  }

  payload=WebServer.arg("my_time_zone");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.my_time_zone, sizeof(JConf.my_time_zone));
    config_changed = true;
  }

  if (config_changed){
    if (!enable){
      JConf.ntp_enable[0] = '0';
      JConf.ntp_enable[1] = '\0';
    }
    JConf.saveConfig();
    #ifdef NTP_ON
      NTPSettingsUpdate();
    #endif
  }

  if (atoi(JConf.ntp_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='ntp_enable' value='1' checked='true'>NTP Enable</label></div>"));

    data += inputBodyName + String(F("Server NTP")) + inputBodyPOST + String(F("ntp_server")) + inputPlaceHolder + JConf.ntp_server + inputBodyClose + inputBodyCloseDiv;

    data += inputBodyName + String(F("Time Zone")) + inputBodyPOST + String(F("my_time_zone")) + inputPlaceHolder + JConf.my_time_zone + inputBodyClose + inputBodyCloseDiv;

  } else {

    data += String(F("<div class='checkbox'><label><input type='checkbox' name='ntp_enable' value='1'>NTP Enable</label></div>"));
  }

  data += inputBodyEnd;


  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebNTPConf load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void handleControl(){

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleControl Start");

  String AUTO;       AUTO += FPSTR(AUTOP);
  String ON;         ON += FPSTR(ONP);
  String OFF;        OFF += FPSTR(OFFP);

  String last_light_state = lightState;
  String last_light_state2 = lightState2;

  if (WebServer.args() > 0 ) {
    for ( size_t i = 0; i < WebServer.args(); i++ ) {
      if (WebServer.argName(i) == "1" && WebServer.arg(i) == "1") {
        if (cycleEnd[atoi(JConf.light_pin)] != 0){
          lightState = OFF;
        } else {
          lightState = ON;
        }
      }
      if (WebServer.argName(i) == "1" && WebServer.arg(i) == "2") {
        lightState = OFF;
        LightControl();
        lightState = AUTO;
      }

      if (WebServer.argName(i) == "2" && WebServer.arg(i) == "1") {
        if (cycleEnd[atoi(JConf.light2_pin)] != 0){
          lightState2 = OFF;
        } else {
          lightState2 = ON;
        }
      }
      if (WebServer.argName(i) == "2" && WebServer.arg(i) == "2") {
        lightState2 = OFF;
        LightControl();
        lightState2 = AUTO;
      }
      if (last_light_state != lightState || last_light_state2 != lightState2){
        LightControl();
        if (atoi(JConf.mqtt_enable) == 1) {
          MqttPubLightState();
        }
      }
    }
  }

  WebServer.send ( 200, "text/html", "OK");

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleControl load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebPinControl(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebPinControl Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);

  String javaScriptPinControl;               javaScriptPinControl += FPSTR(javaScriptPinControlP);

  String pinControl = headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + javaScriptPinControl + containerEnd + siteEnd;

  WebServer.send ( 200, "text/html", pinControl);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebPinControl load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebPinControlStatus(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebPinControlStatus Start");

  LightControl();

  bool pinState;
  bool pinState2;

  String ClassInfo;       ClassInfo += FPSTR(ClassInfoP);
  String ClassDanger;     ClassDanger += FPSTR(ClassDangerP);
  String ClassDefault;    ClassDefault += FPSTR(ClassDefaultP);
  String ClassSuccess;    ClassSuccess += FPSTR(ClassSuccessP);
  String AUTO;            AUTO += FPSTR(AUTOP);
  String ON;              ON += FPSTR(ONP);
  String OFF;             OFF += FPSTR(OFFP);

  if (cycleEnd[atoi(JConf.light_pin)] != 0){
    pinState = true;
  } else {
    pinState = false;
  }

  if (cycleEnd[atoi(JConf.light2_pin)] != 0){
    pinState2 = true;
  } else {
    pinState2 = false;
  }

  String mode;
  if (lightState == AUTO){
    mode = ClassSuccess;
  } else if (lightState == ON) {
    mode = ClassInfo;
  } else {
    mode = ClassDanger;
  }

  String mode2;
  if (lightState2 == AUTO){
    mode2 = ClassSuccess;
  } else if (lightState2 == ON) {
    mode2 = ClassInfo;
  } else {
    mode2 = ClassDanger;
  }

  unsigned long timeOff = 0;
  if (millis() - lightOffTimer < atoi(JConf.lightoff_delay) * 60 * 1000){
    timeOff = atoi(JConf.lightoff_delay) * 60 * 1000 - (millis() - lightOffTimer);
    timeOff = timeOff/1000;
  }

  unsigned long timeOff2 = 0;
  if (millis() - lightOffTimer2 < atoi(JConf.light2off_delay) * 60 * 1000){
    timeOff2 = atoi(JConf.light2off_delay) * 60 * 1000 - (millis() - lightOffTimer2);
    timeOff2 = timeOff2/1000;
  }

  String data;    data += FPSTR(div1P);

  if (atoi(JConf.motion_sensor_enable) == 1){
    data+=String(F("<td class='active'><h4>Timer</h4></td>"));
  }
  data+=FPSTR(div2P);

  if (lightState == AUTO) { data+=ClassDefault; } else if (pinState) { data+=ClassDanger; } else { data+=ClassInfo; }
  data+=String(F("' value='"));
  if (pinState) { data+=String(F("Turn Off")); } else { data+=String(F("Turn On")); }
  data+=String(F("'></div></td>"));

  if (atoi(JConf.motion_sensor_enable) == 1){
    data+=String(F("<td class='active'><div onclick='Auto1();'><input id='Auto' type='submit' class='btn btn-"));
    if (lightState == AUTO) { data+=ClassDanger; } else { data+=ClassDefault; }
    data+=String(F("' value='Auto'></div></td>"));
  } else {
    data+=String(F("<td class='active'></td>"));
  }
  data+=String(F("<td class='"));
  if (pinState) { data+=ClassInfo; } else { data+=ClassDanger; }
  data+=String(F("'><h4>"));
  if (pinState) { data+=ON; } else { data+=OFF; }
  data+=String(F("</h4></td><td class='"));
  data+=mode;    
  data+=String(F("'><h4>"));
  data+=lightState;
  data+=String(F("</h4></td>"));

  if (atoi(JConf.motion_sensor_enable) == 1){
    if (lightState == AUTO && pinState == true){
      data+=String(F("<td class='active'><h4>"));
      data+=String(timeOff);
      data+=String(F("</h4></td>"));
    } else {
      data+=String(F("<td class='active'></td>"));
    }
  }
  data+=String(F("</tr>"));


  data+=String(F("<tr><td class='active'><h4>Light2</h4></td><td class='active'><div onclick='Pin2();'><input id='OnOff2' type='submit' class='btn btn-"));
  if (lightState2 == AUTO) { data+=ClassDefault; } else if (pinState2) { data+=ClassDanger; } else { data+=ClassInfo; }
  data+=String(F("' value='"));
  if (pinState2) { data+=String(F("Turn Off")); } else { data+=String(F("Turn On")); }
  data+=String(F("'></div></td>"));

  if (atoi(JConf.motion_sensor_enable) == 1){
    data+=String(F("<td class='active'><div onclick='Auto2();'><input id='Auto2' type='submit' class='btn btn-"));
    if (lightState2 == AUTO) { data+=ClassDanger; } else { data+=ClassDefault; }
    data+=String(F("' value='Auto'></div></td>"));
  } else {
    data+=String(F("<td class='active'></td>"));
  }
  data+=String(F("<td class='"));
  if (pinState2) { data+=ClassInfo; } else { data+=ClassDanger; }
  data+=String(F("'><h4>"));
  if (pinState2) { data+=ON; } else { data+=OFF; }
  data+=String(F("</h4></td><td class='"));
  data+=mode2;    
  data+=String(F("'><h4>"));
  data+=lightState2;
  data+=String(F("</h4></td>"));

  if (atoi(JConf.motion_sensor_enable) == 1){
    if (lightState2 == AUTO && pinState2 == true){
      data+=String(F("<td class='active'><h4>"));
      data+=String(timeOff2);
      data+=String(F("</h4></td>"));
    } else {
      data+=String(F("<td class='active'></td>"));
    }
  }
  data+=String(F("</tr>"));
  data+=String(F("</tbody></table></div>"));

  WebServer.send ( 200, "text/html", data);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebPinControlStatus load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



#if defined(UART_ON)
void WebAnalogUart(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebAnalogUart Start");

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerStart2;          headerStart2 += FPSTR(headerStart2P);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String javaScript;            javaScript += FPSTR(javaScriptP);
    String javaScript2;           javaScript2 += FPSTR(javaScript2P);
    String bodyAjax;              bodyAjax += FPSTR(bodyAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
    String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

    navbarNonActive += FPSTR(navbarBeginP);
    navbarNonActive += FPSTR(navbarUartP);

    String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
    String containerStart;        containerStart += FPSTR(containerStartP);
    String containerEnd;          containerEnd += FPSTR(containerEndP);
    String siteEnd;               siteEnd += FPSTR(siteEndP);
    String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
    String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);
    String panelEnd;              panelEnd += FPSTR(panelEndP);
    String panelBodySymbol;       panelBodySymbol += FPSTR(panelBodySymbolP);
    String panelBodyName;         panelBodyName += FPSTR(panelBodyNameP);
    String panelBodyValue;        panelBodyValue += FPSTR(panelBodyValueP);
    String closingAngleBracket;   closingAngleBracket += FPSTR(closingAngleBracketP);
    String panelBodyEnd;          panelBodyEnd += FPSTR(panelBodyEndP);

    String title1  = panelHeaderName + String(F("Analog Pins value"))   + panelHeaderEnd;

    String ApinValue = panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 0")) + panelBodyValue + String(F(" id='apin0Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 1")) + panelBodyValue + String(F(" id='apin1Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 2")) + panelBodyValue + String(F(" id='apin2Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 3")) + panelBodyValue + String(F(" id='apin3Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 4")) + panelBodyValue + String(F(" id='apin4Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 5")) + panelBodyValue + String(F(" id='apin5Id'")) + closingAngleBracket + panelBodyEnd;

    
    String title2 = panelHeaderName + String(F("Analog Pins delay"))  + panelHeaderEnd;

    String ApinDelay = panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 0")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin0 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 1")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin1 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 2")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin2 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 3")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin3 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 4")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin4 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 5")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin5 + panelBodyEnd;
    
    WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + javaScript + javaScript2 + bodyAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + title1 + ApinValue + panelEnd + title2 + ApinDelay + panelEnd + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebAnalogUart load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif



void WebGreenhouse(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebGreenhouse Start");

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("Подсветка"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  String payload=WebServer.arg("green_light_on");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_light_on, sizeof(JConf.green_light_on));
  }
  data += inputBodyName + String(F("Время включения")) + inputBodyPOST + String(F("green_light_on")) + inputPlaceHolder + JConf.green_light_on + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_light_off");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_light_off, sizeof(JConf.green_light_off));
  }
  data += inputBodyName + String(F("Время выключения")) + inputBodyPOST + String(F("green_light_off")) + inputPlaceHolder + JConf.green_light_off + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_light_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_light_pin, sizeof(JConf.green_light_pin));
  }
  data += inputBodyName + String(F("Пин лампы")) + inputBodyPOST + String(F("green_light_pin")) + inputPlaceHolder + JConf.green_light_pin + inputBodyClose + inputBodyCloseDiv;

  data += inputBodyEnd;

  data += String(F("<div class='page-header'><h2>"));
  data += String(F("Влажность почвы"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  payload=WebServer.arg("green_humidity_threshold_up");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_humidity_threshold_up, sizeof(JConf.green_humidity_threshold_up));
  }
  data += inputBodyName + String(F("Верхний порог")) + inputBodyPOST + String(F("green_humidity_threshold_up")) + inputPlaceHolder + JConf.green_humidity_threshold_up + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_humidity_threshold_down");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_humidity_threshold_down, sizeof(JConf.green_humidity_threshold_down));
  }
  data += inputBodyName + String(F("Нижний порог")) + inputBodyPOST + String(F("green_humidity_threshold_down")) + inputPlaceHolder + JConf.green_humidity_threshold_down + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_humidity_sensor_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_humidity_sensor_pin, sizeof(JConf.green_humidity_sensor_pin));
  }
  data += inputBodyName + String(F("Пин датчика")) + inputBodyPOST + String(F("green_humidity_sensor_pin")) + inputPlaceHolder + JConf.green_humidity_sensor_pin + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_pump_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_pump_pin, sizeof(JConf.green_pump_pin));
  }
  data += inputBodyName + String(F("Пин насоса")) + inputBodyPOST + String(F("green_pump_pin")) + inputPlaceHolder + JConf.green_pump_pin + inputBodyClose + inputBodyCloseDiv;

  data += inputBodyEnd;

  JConf.saveConfig();

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebGreenhouse load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
///////////////////////////////////   WEB PAGES  End  //////////////////////////////////////////////





void handleXML(){

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleXML Start");

  String XML;
  XML=String(F("<?xml version='1.0'?>"));
  XML+=String(F("<Donnees>")); 
  XML+=String(F("<temperature>"));
  XML+=temperatureString;
  XML+=String(F(" °C"));
  XML+=String(F("</temperature>"));
  XML+=String(F("<humidity>"));
  XML+=humidityString;
  XML+=String(F(" %"));
  XML+=String(F("</humidity>"));
  XML+=String(F("<pressure>"));
  XML+=pressureString;
  XML+=String(F(" mm"));
  XML+=String(F("</pressure>"));
  XML+=String(F("<illuminance>"));
  XML+=luxString;
  XML+=String(F(" lux"));
  XML+=String(F("</illuminance>"));

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      XML+=String(F("<pzemVoltage>"));
      XML+=pzemVoltageString;
      XML+=String(F(" V"));
      XML+=String(F("</pzemVoltage>"));

      XML+=String(F("<pzemCurrent>"));
      XML+=pzemCurrentString;
      XML+=String(F(" A"));
      XML+=String(F("</pzemCurrent>"));

      XML+=String(F("<pzemPower>"));
      XML+=pzemPowerString;
      XML+=String(F(" kW"));
      XML+=String(F("</pzemPower>"));

      XML+=String(F("<pzemEnergy>"));
      XML+=pzemEnergyString;
      XML+=String(F(" kWh"));
      XML+=String(F("</pzemEnergy>"));
    }
  #endif

  XML+=String(F("<uptime>"));
  XML+=uptimeString;
  XML+=String(F("</uptime>"));

  if (atoi(JConf.ntp_enable) == 1) {
    XML+=String(F("<ntpTime>"));
    XML+=ntpTimeString;
    XML+=String(F("</ntpTime>"));
  }

  XML+=String(F("<vcc>"));
  XML+=String(voltage_float);
  XML+=String(F(" V"));
  XML+=String(F("</vcc>"));
  XML+=String(F("<freeMemory>"));
  XML+=freeMemoryString;
  XML+=String(F("</freeMemory>"));

  #ifdef UART_ON
  for (int i = 0; i < ANALOG_PINS; i++){
    XML+=String(F("<apin"));  XML+=String(i);  XML+=String(F(">"));
    XML+=String(Uart.valueAnalogPin[i]);
    XML+=String(F("</apin")); XML+=String(i);  XML+=String(F(">"));
  }
  #endif

  XML+=String(F("</Donnees>")); 

  WebServer.send(200,"text/xml",XML);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleXML load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void WebServerInit()
{

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebServerInit Start");

  // Prepare webserver pages
  WebServer.on("/", handleRoot);
  WebServer.on("/reboot", WebReboot);
  WebServer.on("/update", WebUpdate);
  WebServer.onFileUpload(WebFileUpload);
  WebServer.on("/upload_sketch", WebUploadSketch);
  WebServer.on("/wificonf", WebWiFiConf);
  WebServer.on("/sensorsconf", WebSensorsConf);
  WebServer.on("/espconf", WebEspConf);
  WebServer.on("/mqttconf", WebMqttConf);
  WebServer.on("/ntpconf", WebNTPConf);
  WebServer.on("/control", handleControl);
  WebServer.on("/pincontrol", WebPinControl);
  WebServer.on("/controlstatus", WebPinControlStatus);
  WebServer.on("/log", handleLogConfig);
  WebServer.on("/save", handleSave);

  WebServer.on("/cm", handleConsole);
  WebServer.on("/ax", handleAjax);


#ifdef UART_ON
  WebServer.on("/analog", WebAnalogUart);
#endif

  WebServer.on("/greenhouse", WebGreenhouse);
  WebServer.on("/xml",handleXML);

/*
  WebServer.on("/upload", HTTP_GET, handle_upload);
  WebServer.on("/upload", HTTP_POST, handle_upload_post, handleFileUpload);
  WebServer.onNotFound(handleNotFound);

  if (ESP.getFlashChipRealSize() > 524288)
    httpUpdater.setup(&WebServer);
*/
  WebServer.begin();

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: WebServerInit load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}





#endif  // USE_WEBSERVER
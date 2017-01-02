#ifdef USE_WEBSERVER

//#define STR_HELPER(x) #x
//#define STR(x) STR_HELPER(x)



/////////////////////////////////////////////////////////////     HTML SNIPPLETS

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

const char ClassInfoP[] PROGMEM  = "info";
const char ClassDangerP[] PROGMEM  = "danger";
const char ClassDefaultP[] PROGMEM  = "default";
const char ClassSuccessP[] PROGMEM  = "success";

const char AUTOP[] PROGMEM  = "AUTO";
const char ONP[] PROGMEM  = "ON";
const char OFFP[] PROGMEM  = "OFF";


/******************************************************************************\
 *
 * Template
 *
\******************************************************************************/
const char HTTP_HEAD[] PROGMEM =
"<html lang='en'>"
  "<head>"
    "<title id='header'>{module_id}</title>"
    "<meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>"
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script>"
    "<script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>"
  "</head>";

const char HTTP_BODY[] PROGMEM =
  "<body>"
  "<nav class='navbar navbar-inverse'>"
    "<div class='container-fluid'>"
      "<div class='navbar-header'>"
        "<a class='navbar-brand' id='header' href='/'><span class='glyphicon glyphicon-home'></span> {module_id}</a>"
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
  "<div class='container-fluid'>"
    "<div class='row'>";

const char HTTP_PANEL_NAME[] PROGMEM =
"<div class='col-sm-5 col-md-4 col-lg-3'>"
  "<h3>{panel_name}</h3>";

const char HTTP_PANEL_NAME_12[] PROGMEM =
"<div class='col-sm-12 col-md-9 col-lg-8'>"
  "<h3>{panel_name}</h3>";

const char HTTP_PANEL_END[] PROGMEM =
    "<div class='clearfix visible-lg'></div>"
"</div>";

//Form for save config//////////////////////////////////////////////////////////
const char HTTP_FORM_START[] PROGMEM =
"<form action='save' method='POST'>"
    "<div class='panel panel-default'>"
        "<div class='panel-body'>";

const char HTTP_FORM_CHECKBOX[] PROGMEM =
"<div class='checkbox'>"
  "<label><input type='checkbox' value='1' id='{id}' name='{id}'>{name}</label>"
"</div>";

const char HTTP_FORM_INPUT_TXT[] PROGMEM =
"<div class='form-group'>"
    "<div class='input-group'>"
        "<span class='input-group-addon'>{name}</span>"
        "<input type='text'  id='{id}' name='{id}' length=32 placeholder='{value}' value='{value}' class='form-control'>"
    "</div>"
"</div>";

const char HTTP_FORM_INPUT_TXT_UNIT[] PROGMEM =
"<div class='form-group'>"
    "<div class='input-group'>"
        "<span class='input-group-addon'>{name}</span>"
        "<input type='text'  id='{id}' name='{id}' length=32 placeholder='{value}' value='{value}' class='form-control'>"
        "<span class='input-group-addon'>{unit}</span>"
    "</div>"
"</div>";

const char HTTP_FORM_INPUT_PWD[] PROGMEM =
"<div class='form-group'>"
    "<div class='input-group'>"
        "<span class='input-group-addon'>{name}</span>"
        "<input type='password'  id='{id}' name='{id}' length=32 value='********' class='form-control'>"
    "</div>"
"</div>";

const char HTTP_BTN_SAVE[] PROGMEM =
"</div>"
"<div class='panel-footer clearfix'>"
  "<div class='pull-right'><button type='submit' class='btn btn-primary'>Save</button></div>"
"</div>";

const char HTTP_FORM_END[] PROGMEM =
    "</div>"
"</form>";
//////////////////////////////////////////////////////////Form for save config//

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
const char JS_ROOT_START[] PROGMEM =
"<SCRIPT>"
"var xmlHttp=createXmlHttpObject();"
"function createXmlHttpObject(){"
  "if(window.XMLHttpRequest){"
    "xmlHttp=new XMLHttpRequest();"
  "}else{"
    "xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');"
 "}"
 "return xmlHttp;"
"}"
"function process(){"
 "if(xmlHttp.readyState==0 || xmlHttp.readyState==4){"
   "xmlHttp.open('PUT','xml',true);"
   "xmlHttp.onreadystatechange=handleServerResponse;"
   "xmlHttp.send(null);"
 "}"
 "setTimeout('process()',{update_delay});"
"}"
"function handleServerResponse(){"
 "if(xmlHttp.readyState==4 && xmlHttp.status==200){"
   "xmlResponse=xmlHttp.responseXML;";

const char JS_ROOT_XML[] PROGMEM =
"xmldoc = xmlResponse.getElementsByTagName('{id}');"
"message = xmldoc[0].firstChild.nodeValue;"
"document.getElementById('{id}Id').innerHTML=message;";

const char JS_ROOT_END[] PROGMEM =
 "}"
"}"
"</SCRIPT>";


const char HTTP_ROOT_PANEL_DIV[] PROGMEM =
    "<div class='panel panel-default'>"
        "<div class='panel-body'><span class='glyphicon glyphicon-{icon}'></span> {name}<span class='pull-right' id='{id}Id'></span></div>"
    "</div>";

const char HTTP_ROOT_PANEL_END[] PROGMEM =
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
"<div class='col-sm-5 col-md-4 col-lg-3'>"
    "<h3>Logging Configuration</h3>"
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
                "<div class='pull-right'><button type='submit' class='btn btn-primary'>Save</button></div>"
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
    "<textarea class='form-control' rows='30' readonly id='t1' name='t1' cols='50' wrap='off'></textarea>"
    "<form class='form-horizontal' role='form'  method='post' action='cm'>"
      "<br>"
      "<div>"
          "<input type='text'  id='" SUB_PREFIX "' name='" SUB_PREFIX "' length=80 placeholder='Enter command' class='form-control' autofocus>"
      "</div>"
    "</form>";

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


const char JS_PIN_CONTROL[] PROGMEM =
  "<div id='content'></div>"
  "<div id='pin1'></div>"
  "<script>"
  "function show(){"
    "$.ajax({url:'controlstatus', cache: false, success: function(html){$('#content').html(html);}});"
  "}"
  "function Pin1(){"
    "$.ajax({type:'POST', url:'control', data:'1=1', success:function(data){show();}});"
  "}"
  "function Auto1(){"
    "$.ajax({type:'POST', url:'control', data:'1=2', success:function(data){show();}});"
  "}"
  "function Pin2(){"
    "$.ajax({type:'POST', url:'control', data:'2=1', success:function(data){show();}});"
  "}"
  "function Auto2(){"
    "$.ajax({type:'POST', url:'control', data:'2=2', success:function(data){show();}});"
  "}"
  "$(document).ready(function(){show(); setInterval('show()',5000);});"
  "</script>";

const char HTTP_REBOOT[] PROGMEM =
"<div class='col-sm-5 col-md-4 col-lg-3'>"
  "<div class='page-header'>"
    "<h2>Reboot ESP</h2>"
  "</div>"
  "<div class='alert alert-info' role='alert'>"
    "<a href='#' class='alert-link'>Rebooting...</a>"
  "</div>"
"</div>";

const char HTTP_UPDATE[] PROGMEM  =
"<div class='col-sm-5 col-md-4 col-lg-3'>"
  "<h3>Update Frimware</h3>"
  "<form method='POST' action='/upload_sketch' enctype='multipart/form-data'>"
    "<p><input type='file' class='btn btn-primary' name='sketch'></p>"
    "<h3><small>Выберите файл формата *.bin</small></h3>"
    "<p><input type='submit' value='Upload' class='btn btn-danger'></p>"
  "</form>"
"</div>";

const char HTTP_SENSORS[] PROGMEM  =
"<div class='col-sm-5 col-md-4 col-lg-3'>"
    "<h3>Sensors Configuration</h3>"
    "<form action='save' method='POST'>"
        "<div class='panel panel-default'>"
            "<div class='panel-body'>"
                "<input id='w' name='w' value='2' hidden>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='bme280_enable' name='bme280_enable'>BME280</label></div>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='sht21_enable' name='sht21_enable'>SHT21</label></div>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='dht_enable' name='dht_enable'>DHT</label></div>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='ds18x20_enable' name='ds18x20_enable'>DS18X20</label></div>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='bh1750_enable' name='bh1750_enable'>BH1750</label></div>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='motion_sensor_enable' name='motion_sensor_enable'>Motion Sensor</label></div>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='pzem_enable' name='pzem_enable'>Energy Monitor</label></div>"
                "<div class='checkbox'><label><input type='checkbox' value='1' id='mhz19_enable' name='mhz19_enable'>MH-Z19</label></div>"
            "</div>"
            "<div class='panel-footer clearfix'>"
                "<div class='pull-right'><button type='submit' class='btn btn-primary'>Save</button></div>"
            "</div>"
        "</div>"
    "</form>"
    "<div class='clearfix visible-lg'></div>"
"</div>";


const char HTTP_WIFI_SETTINGS[] PROGMEM =
      "<div class='col-sm-5 col-md-4 col-lg-3'>"
          "<h3>Wi-Fi Configuration</h3>"
          "<form action='/save' method='POST'>"
              "<div class='panel panel-default'>"
                  "<div class='panel-body'>"
                      "<input id='w' name='w' value='6' hidden>"
                      "<div class='form-group'>"
                          "<div class='input-group'>"
                              "<span class='input-group-addon'>Module ID</span>"
                              "<input type='text' name='module_id' id='module_id' class='form-control'>"
                          "</div>"
                      "</div>"
                      "<div class='form-group'>"
                          "<div class='input-group'>"
                              "<span class='input-group-addon'>Wi-Fi type</span>"
                              "<select class='form-control' name='wifi_mode' id='wifi_mode'>"
                                  "<option value='AP'>Access Point</option>"
                                  "<option value='STA'>Client</option>"
                                  "<option value='AP_STA'>AP + Client</option>"
                              "</select>"
                          "</div>"
                      "</div>"
                      "<div id='ap' style='display: none;'>"
                          "<h4>AP settings</h4>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>Mode</span>"
                                  "<select class='form-control' name='wifi_phy_mode' id='wifi_phy_mode'>"
                                      "<option value='11B'>11b</option>"
                                      "<option value='11G'>11g</option>"
                                      "<option value='11N'>11n</option>"
                                  "</select>"
                              "</div>"
                          "</div>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>Channel</span>"
                                  "<select class='form-control' name='wifi_channel' id='wifi_channel'>"
                                      "<option value='1'>1</option>"
                                      "<option value='2'>2</option>"
                                      "<option value='3'>3</option>"
                                      "<option value='4'>4</option>"
                                      "<option value='5'>5</option>"
                                      "<option value='6'>6</option>"
                                      "<option value='7'>7</option>"
                                      "<option value='8'>8</option>"
                                      "<option value='9'>9</option>"
                                      "<option value='10'>10</option>"
                                      "<option value='11'>11</option>"
                                      "<option value='12'>12</option>"
                                      "<option value='13'>13</option>"
                                  "</select>"
                              "</div>"
                          "</div>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>Auth</span>"
                                  "<select class='form-control' name='wifi_auth' id='wifi_auth'>"
                                      "<option value='OPEN'>OPEN</option>"
                                      "<option value='WPA_PSK'>WPA_PSK</option>"
                                      "<option value='WPA2_PSK'>WPA2_PSK</option>"
                                      "<option value='WPA_WPA2_PSK'>WPA_WPA2_PSK</option>"
                                  "</select>"
                              "</div>"
                          "</div>"
                          "<div class='form-group' id=ap_auth_div>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>AP Password</span>"
                                  "<input type='password' name='ap_pwd' class='form-control' placeholder='********'>"
                              "</div>"
                          "</div>"
                      "</div>"
                      "<div id='sta' style='display: none;'>"
                          "<h4>Client settings</h4>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>SSID</span>"
                                  "<input type='text' name='sta_ssid' id='sta_ssid' class='form-control'>"
                              "</div>"
                          "</div>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>Password</span>"
                                  "<input type='password' name='sta_pwd' id='sta_pwd' class='form-control' placeholder='********'>"
                              "</div>"
                          "</div>"
                      "</div>"
                      "<div class='checkbox' id='static_checkbox_div' style='display: none;'>"
                          "<label><input type='checkbox' name='static_ip_enable' id='static_ip_enable'>Static IP</label>"
                      "</div>"
                      "<div id='static' style='display: none;'>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>Static IP</span>"
                                  "<input type='text' name='static_ip' id='static_ip' class='form-control'>"
                              "</div>"
                          "</div>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>Static Gateway</span>"
                                  "<input type='text' name='static_gateway' id='static_gateway' class='form-control'>"
                              "</div>"
                          "</div>"
                          "<div class='form-group'>"
                              "<div class='input-group'>"
                                  "<span class='input-group-addon'>Static Subnet</span>"
                                  "<input type='text' name='static_subnet' id='static_subnet' class='form-control'>"
                              "</div>"
                          "</div>"
                      "</div>"
                  "</div>"
                  "<div class='panel-footer clearfix'>"
                      "<div class='pull-right'><button type='submit' class='btn btn-primary' name='save_conf' value='1'>Save</button></div>"
                  "</div>"
              "</div>"
          "</form>"
      "</div>"
      "<div class='col-sm-5 col-md-4 col-lg-3'>"
          "<h3>Scan AP</h3>"
          "<div class='panel panel-default'>"
              "<div class='panel-body'>"
                  "<table class='table table-striped'>"
                      "<tbody id='scan'>"
                      "</tbody>"
                  "</table>"
                  "<div class='clearfix visible-lg'></div>"
              "</div>"
              "<div class='panel-footer clearfix'>"
                "<div class='pull-right'>"
                  "<a href='#update' class='btn btn-primary' id='rescan'>Rescan</a>"
                "</div>"
              "</div>"
          "</div>"
      "</div>"
    "</div>"
  "</body>"
"</html>";


const char JS_WIFI_SETTINGS[] PROGMEM =
"<script>"
  "function c(l){"
    "document.getElementById('sta_ssid').value=l.innerText||l.textContent;"
    "document.getElementById('sta_pwd').focus();"
  "}"
  "$(function(){"
    "$('#scan').load('/scan');"
    "var module_id = '{{module_id}}';"
    "var wifi_mode = '{{wifi_mode}}';"
    "var wifi_phy_mode = '{{wifi_phy_mode}}';"
    "var wifi_channel = '{{wifi_channel}}';"
    "var wifi_auth = '{{wifi_auth}}';"
    "var sta_ssid = '{{sta_ssid}}';"
    "var static_ip_enable = '{{static_ip_enable}}';"
    "var static_ip = '{{static_ip}}';"
    "var static_subnet = '{{static_subnet}}';"
    "var static_gateway = '{{static_gateway}}';"
    "$('#module_id').prop('value', module_id);"
    "$('#wifi_mode').prop('value', wifi_mode);"
    "$('#wifi_phy_mode').prop('value', wifi_phy_mode);"
    "$('#wifi_channel').prop('value', wifi_channel);"
    "$('#wifi_auth').prop('value', wifi_auth);"
    "$('#sta_ssid').prop('value', sta_ssid);"
    "$('#static_ip').prop('value', static_ip);"
    "$('#static_subnet').prop('value', static_subnet);"
    "$('#static_gateway').prop('value', static_gateway);"
    "if (static_ip_enable === '1') {"
      "$('#static_ip_enable').prop('checked', true);"
    "}"
    "const staticIp = () => {"
      "if ($('#wifi_mode').prop('selected', true).prop('value') === 'AP') {"
        "$('#static_checkbox_div').hide();"
        "$('#static').hide();"
        "return;"
      "} else {"
        "$('#static_checkbox_div').show();"
      "}"
      "if ( $('#static_ip_enable').prop('checked') ) {"
        "static_ip_enable = 1;"
        "$('#static').show();"
      "} else {"
        "static_ip_enable = 0;"
        "$('#static').hide();"
      "}"
      "$('#static_ip_enable').prop('value', static_ip_enable);"
    "};"
    "staticIp();"
    "$('#static_ip_enable').on('click', function(){"
      "staticIp();"
    "});"
    "const wifiMode = () => {"
      "var $selected = $('#wifi_mode').prop('selected', true);"
      "if($selected.prop('value') === 'AP') {"
        "$('#sta').hide();"
        "$('#ap').show();"
      "} else if($selected.prop('value') === 'STA') {"
        "$('#ap').hide();"
        "$('#sta').show();"
      "} else {"
        "$('#ap').show();"
        "$('#sta').show();"
      "}"
      "staticIp();"
    "};"
    "wifiMode();"
    "$('#wifi_mode').on('change', function(){"
      "wifiMode();"
    "});"
    "const apAuth = () => {"
      "var $selected = $('#wifi_auth').prop('selected', true);"
      "if($selected.prop('value') === 'OPEN') {"
        "$('#ap_auth_div').hide();"
      "} else {"
        "$('#ap_auth_div').show();"
      "}"
    "};"
    "apAuth();"
    "$('#wifi_auth').on('change', function(){"
      "apAuth();"
    "});"
    "$('#rescan').on('click', function(){"
      "$('#scan').load('/scan');"
    "});"
  "});"
"</script>";


const char JS_NTP_SETTINGS[] PROGMEM =
"<script>"
  "$(function(){"
    "var ntp_enable = '{{ntp_enable}}';"
    "if (ntp_enable === '1') {"
      "$('#ntp_enable').prop('checked', true);"
    "}"
    "const staticIp = () => {"
      "if ( $('#ntp_enable').prop('checked') ) {"
        "ntp_enable = 1;"
        "$('#ntp').show();"
      "} else {"
        "ntp_enable = 0;"
        "$('#ntp').hide();"
      "}"
      "$('#ntp_enable').prop('value', ntp_enable);"
    "};"
    "staticIp();"
    "$('#ntp_enable').on('click', function(){"
      "staticIp();"
    "});"
  "});"
"</script>";


const char JS_MQTT_SETTINGS[] PROGMEM =
"<script>"
  "$(function(){"
    "var mqtt_enable = '{{mqtt_enable}}';"
    "var mqtt_auth_enable = '{{mqtt_auth_enable}}';"
    "if (mqtt_enable === '1') {"
      "$('#mqtt_enable').prop('checked', true);"
    "}"
    "if (mqtt_auth_enable === '1') {"
      "$('#mqtt_auth_enable').prop('checked', true);"
    "}"
    "const mqtt = () => {"
      "if ( $('#mqtt_enable').prop('checked') ) {"
        "mqtt_enable = 1;"
        "$('#mqtt').show();"
      "} else {"
        "mqtt_enable = 0;"
        "$('#mqtt').hide();"
      "}"
      "$('#mqtt_enable').prop('value', mqtt_enable);"
    "};"
    "mqtt();"
    "$('#mqtt_enable').on('click', function(){"
      "mqtt();"
    "});"
    "const mqttAuth = () => {"
      "if ( $('#mqtt_auth_enable').prop('checked') ) {"
        "mqtt_auth_enable = 1;"
        "$('#mqtt_auth').show();"
      "} else {"
        "mqtt_auth_enable = 0;"
        "$('#mqtt_auth').hide();"
      "}"
      "$('#mqtt_auth_enable').prop('value', mqtt_auth_enable);"
    "};"
    "mqttAuth();"
    "$('#mqtt_auth_enable').on('click', function(){"
      "mqttAuth();"
    "});"
  "});"
"</script>";



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
  String js = FPSTR(JS_ROOT_START);
    js.replace("{update_delay}", String(atoi(JConf.get_data_delay) * 1000).c_str());



  if (atoi(JConf.bme280_enable) == 1 || atoi(JConf.sht21_enable) == 1 || atoi(JConf.dht_enable) == 1) {
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "temperature");

    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "humidity");
  }
  if (atoi(JConf.bme280_enable) == 1) {
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "pressure");
  }
  if (atoi(JConf.bh1750_enable) == 1) {
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "illuminance");
  }

  #ifdef DS18X20_ON
    if (atoi(JConf.ds18x20_enable) == 1){
      for (size_t i = 0; i < findDsSensors; i++)
      {
        String id = String(F("ds")) + String(i);
        js += FPSTR(JS_ROOT_XML);
        js.replace("{id}", id);
      }
    }
  #endif //DS18X20_ON


  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "pzemVoltage");
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "pzemCurrent");
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "pzemPower");
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "pzemEnergy");
    }
  #endif //PZEM_ON

  #ifdef MHZ19_ON
    if (atoi(JConf.mhz19_enable) == 1){
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "mhz19ppm");
    }
  #endif //MHZ19_ON

  if (atoi(JConf.ntp_enable) == 1) {
    js += FPSTR(JS_ROOT_XML);
    js.replace("{id}", "ntpTime");
  }

  js += FPSTR(JS_ROOT_XML);
  js.replace("{id}", "vcc");
  js += FPSTR(JS_ROOT_XML);
  js.replace("{id}", "uptime");
  js += FPSTR(JS_ROOT_XML);
  js.replace("{id}", "freeMemory");

  js += FPSTR(JS_ROOT_END);

//Sensors//////////////////////////////////////////////////////////////////////////////////////////////////
  String sensors = FPSTR(HTTP_PANEL_NAME);
  sensors.replace("{panel_name}", "Sensors Data");

  if (atoi(JConf.bme280_enable) == 1 || atoi(JConf.sht21_enable) == 1 || atoi(JConf.dht_enable) == 1 ){
    sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
    sensors.replace("{icon}", "fire");
    sensors.replace("{name}", "Temperature");
    sensors.replace("{id}", "temperature");

    sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
    sensors.replace("{icon}", "tint");
    sensors.replace("{name}", "Humidity");
    sensors.replace("{id}", "humidity");
  }

  #ifdef BME280_ON
    if (atoi(JConf.bme280_enable) == 1){
      sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
      sensors.replace("{icon}", "cloud");
      sensors.replace("{name}", "Pressure");
      sensors.replace("{id}", "pressure");
    }
  #endif //BME280_ON

  #ifdef DS18X20_ON
    if (atoi(JConf.ds18x20_enable) == 1){
      for (size_t i = 0; i < findDsSensors; i++)
      {
        String id = String(F("ds")) + String(i);
        String name = String(F("ds")) + String(i) + String(F(" (")) + dsData[i].addressString + String(F(")"));
        sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
        sensors.replace("{icon}", "fire");
        sensors.replace("{name}", name);
        sensors.replace("{id}", id);
      }
    }
  #endif //DS18X20_ON


  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
      sensors.replace("{icon}", "flash");
      sensors.replace("{name}", "Voltage");
      sensors.replace("{id}", "pzemVoltage");

      sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
      sensors.replace("{icon}", "flash");
      sensors.replace("{name}", "Current");
      sensors.replace("{id}", "pzemCurrent");

      sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
      sensors.replace("{icon}", "flash");
      sensors.replace("{name}", "Power");
      sensors.replace("{id}", "pzemPower");

      sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
      sensors.replace("{icon}", "flash");
      sensors.replace("{name}", "Energy");
      sensors.replace("{id}", "pzemEnergy");
    }
  #endif //PZEM_ON

  #ifdef MHZ19_ON
    if (atoi(JConf.mhz19_enable) == 1){
      sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
      sensors.replace("{icon}", "grain");
      sensors.replace("{name}", "CO2");
      sensors.replace("{id}", "mhz19ppm");
    }
  #endif //MHZ19_ON

  if (atoi(JConf.bh1750_enable) == 1){
    sensors += FPSTR(HTTP_ROOT_PANEL_DIV);
    sensors.replace("{icon}", "asterisk");
    sensors.replace("{name}", "Illuminance");
    sensors.replace("{id}", "illuminance");
  }

  sensors += FPSTR(HTTP_ROOT_PANEL_END);

//Settings/////////////////////////////////////////////////////////////////////////////////////////////////
  String settings = FPSTR(HTTP_PANEL_NAME);
  settings.replace("{panel_name}", "Settings");

  if (atoi(JConf.ntp_enable) == 1) {
    settings += FPSTR(HTTP_ROOT_PANEL_DIV);
    settings.replace("{icon}", "time");
    settings.replace("{name}", "NTP Time");
    settings.replace("{id}", "ntpTime");
  }

  settings += FPSTR(HTTP_ROOT_PANEL_DIV);
  settings.replace("{icon}", "time");
  settings.replace("{name}", "Uptime");
  settings.replace("{id}", "uptime");

  settings += FPSTR(HTTP_ROOT_PANEL_DIV);
  settings.replace("{icon}", "flash");
  settings.replace("{name}", "Voltage");
  settings.replace("{id}", "vcc");

  settings += FPSTR(HTTP_ROOT_PANEL_DIV);
  settings.replace("{icon}", "flash");
  settings.replace("{name}", "Free Memory");
  settings.replace("{id}", "freeMemory");

  settings += FPSTR(HTTP_ROOT_PANEL_END);
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

  String name = FPSTR(HTTP_PANEL_NAME_12);
  name.replace("{panel_name}", "Console");

  String page = head + body + FPSTR(JS_CONSOLE) + name + FPSTR(HTTP_CONSOLE) + FPSTR(HTTP_PANEL_END) + FPSTR(HTTP_BODY_END);

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



void handleReboot()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleReboot Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));
  head.replace("</title>", "</title><META HTTP-EQUIV='Refresh' CONTENT='20; URL=/'>");

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String page = head + body + FPSTR(HTTP_REBOOT) + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleReboot load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
  restartESP();
}



void handleUpdate()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleUpdate Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String page = head + body + FPSTR(HTTP_UPDATE) + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleUpdate load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handleUploadSketch()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleUploadSketch Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));
  head.replace("</title>", "</title><META HTTP-EQUIV='Refresh' CONTENT='20; URL=/'>");

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String data = String(F("<div class='col-sm-5 col-md-4 col-lg-3'><h3>Update Frimware</h3><div class='alert alert-success'>")) + ((Update.hasError()) ? String(F("FAIL")) : String(F("Update Frimware: OK"))) + String(F("</div></div>"));

  String page = head + body + data + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleUploadSketch load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
  restartESP();
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



void handleSensorsConfig()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleSensorsConfig Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));


  String settings = FPSTR(HTTP_SENSORS);

  if (atoi(JConf.bme280_enable) == 1){
    settings.replace("id='bme280_enable'", "checked='true' id='bme280_enable'");
  }
  if (atoi(JConf.sht21_enable) == 1){
    settings.replace("id='sht21_enable'", "checked='true' id='sht21_enable'");
  }
  if (atoi(JConf.dht_enable) == 1){
    settings.replace("id='dht_enable'", "checked='true' id='dht_enable'");
  }

  #ifdef DS18X20_ON
    if (atoi(JConf.ds18x20_enable) == 1){
      settings.replace("id='ds18x20_enable'", "checked='true' id='ds18x20_enable'");
    }
  #endif //DS18X20_ON

  if (atoi(JConf.bh1750_enable) == 1){
    settings.replace("id='bh1750_enable'", "checked='true' id='bh1750_enable'");
  }
  if (atoi(JConf.motion_sensor_enable) == 1){
    settings.replace("id='motion_sensor_enable'", "checked='true' id='motion_sensor_enable'");
  }
  if (atoi(JConf.pzem_enable) == 1){
    settings.replace("id='pzem_enable'", "checked='true' id='pzem_enable'");
  }
  if (atoi(JConf.mhz19_enable) == 1){
    settings.replace("id='mhz19_enable'", "checked='true' id='mhz19_enable'");
  }

  String page = head + body + settings + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleSensorsConfig load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handleMqttConfig()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleMqttConfig Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String js = FPSTR(JS_MQTT_SETTINGS);
  js.replace("{{mqtt_enable}}",      String(JConf.mqtt_enable));
  js.replace("{{mqtt_auth_enable}}", String(JConf.mqtt_auth_enable));

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String name = FPSTR(HTTP_PANEL_NAME);
  name.replace("{panel_name}", "MQTT Configuration");


  String form = FPSTR(HTTP_FORM_START);
  form += "<input id='w' name='w' value='1' hidden>";

  form += FPSTR(HTTP_FORM_CHECKBOX);
  form.replace("{id}", "mqtt_enable");
  form.replace("{name}", "MQTT Enable");

  form += String(F("<div id='mqtt' style='display: none;'>"));
  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "mqtt_server");
  form.replace("{name}", "Server MQTT");
  form.replace("{value}", String(JConf.mqtt_server));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "mqtt_port");
  form.replace("{name}", "Port MQTT");
  form.replace("{value}", String(JConf.mqtt_port));
  form += String(F("<hr>"));

  form += FPSTR(HTTP_FORM_CHECKBOX);
  form.replace("{id}", "mqtt_auth_enable");
  form.replace("{name}", "MQTT Authentication");

  form += String(F("<div id='mqtt_auth' style='display: none;'>"));
  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "mqtt_user");
  form.replace("{name}", "MQTT User");
  form.replace("{value}", String(JConf.mqtt_user));

  form += FPSTR(HTTP_FORM_INPUT_PWD);
  form.replace("{id}", "mqtt_pwd");
  form.replace("{name}", "MQTT Password");
  form += String(F("</div>"));
  form += String(F("<hr>"));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "mqtt_name");
  form.replace("{name}", "MQTT Postfix");
  form.replace("{value}", String(JConf.mqtt_name));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "publish_topic");
  form.replace("{name}", "Publish Topic");
  form.replace("{value}", String(JConf.publish_topic));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "subscribe_topic");
  form.replace("{name}", "Subscribe Topic");
  form.replace("{value}", String(JConf.subscribe_topic));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "publish_delay");
  form.replace("{name}", "Publish Delay");
  form.replace("{value}", String(JConf.publish_delay));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "subscribe_delay");
  form.replace("{name}", "Subscribe Delay");
  form.replace("{value}", String(JConf.subscribe_delay));

  form += String(F("</div>"));
  form += FPSTR(HTTP_BTN_SAVE);
  form += FPSTR(HTTP_FORM_END);

  String page = head + js + body + name + form + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleMqttConfig load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handleNtpConfig()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleNtpConfig Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String js = FPSTR(JS_NTP_SETTINGS);
  js.replace("{{ntp_enable}}",  String(JConf.ntp_enable));

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String name = FPSTR(HTTP_PANEL_NAME);
  name.replace("{panel_name}", "NTP Configuration");

  String form = FPSTR(HTTP_FORM_START);
  form += "<input id='w' name='w' value='4' hidden>";

  form += FPSTR(HTTP_FORM_CHECKBOX);
  form.replace("{id}", "ntp_enable");
  form.replace("{name}", "NTP Enable");

  form += String(F("<div id='ntp' style='display: none;'>"));
  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "ntp_server");
  form.replace("{name}", "Server NTP");
  form.replace("{value}", String(JConf.ntp_server));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "my_time_zone");
  form.replace("{name}", "Port NTP");
  form.replace("{value}", String(JConf.my_time_zone));
  form += String(F("</div>"));

  form += FPSTR(HTTP_BTN_SAVE);
  form += FPSTR(HTTP_FORM_END);

  String page = head + js + body + name + form + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleNtpConfig load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handlePinControl()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handlePinControl Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String page = head + body + FPSTR(JS_PIN_CONTROL) + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handlePinControl load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handleEspConfig()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleEspConfig Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String name = FPSTR(HTTP_PANEL_NAME);
  name.replace("{panel_name}", "ESP Configuration");


  String form = FPSTR(HTTP_FORM_START);
  form += String(F("<input id='w' name='w' value='5' hidden>"));
  form += String(F("<h4>Light 1</h4>"));


  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "light_pin");
  form.replace("{name}", "Pin");
  form.replace("{value}", String(JConf.light_pin));

  form += FPSTR(HTTP_FORM_INPUT_TXT_UNIT);
  form.replace("{id}", "lightoff_delay");
  form.replace("{name}", "Off Delay");
  form.replace("{value}", String(JConf.lightoff_delay));
  form.replace("{unit}", "min");

  if (atoi(JConf.bh1750_enable) == 1){
    form += FPSTR(HTTP_FORM_INPUT_TXT_UNIT);
    form.replace("{id}", "lighton_lux");
    form.replace("{name}", "On Lux");
    form.replace("{value}", String(JConf.lighton_lux));
    form.replace("{unit}", "Lux");
  }

  form += FPSTR(HTTP_FORM_CHECKBOX);
  form.replace("{id}", "light_smooth");
  form.replace("{name}", "Smooth Enable");
  if (atoi(JConf.light_smooth) == 1){
    form.replace("id='light_smooth'", "checked='true' id='light_smooth'");
  }

  form += String(F("<hr>"));
  form += String(F("<h4>Light 2</h4>"));

  form += FPSTR(HTTP_FORM_INPUT_TXT);
  form.replace("{id}", "light2_pin");
  form.replace("{name}", "Pin");
  form.replace("{value}", String(JConf.light2_pin));

  form += FPSTR(HTTP_FORM_INPUT_TXT_UNIT);
  form.replace("{id}", "light2off_delay");
  form.replace("{name}", "Off Delay");
  form.replace("{value}", String(JConf.light2off_delay));
  form.replace("{unit}", "min");

  if (atoi(JConf.bh1750_enable) == 1){
    form += FPSTR(HTTP_FORM_INPUT_TXT_UNIT);
    form.replace("{id}", "light2on_lux");
    form.replace("{name}", "On Lux");
    form.replace("{value}", String(JConf.light2on_lux));
    form.replace("{unit}", "Lux");
  }

  form += FPSTR(HTTP_FORM_CHECKBOX);
  form.replace("{id}", "light2_smooth");
  form.replace("{name}", "Smooth Enable");
  if (atoi(JConf.light2_smooth) == 1){
    form.replace("id='light2_smooth'", "checked='true' id='light2_smooth'");
  }
  form += String(F("<hr>"));

  if (atoi(JConf.motion_sensor_enable) == 1){
    form += FPSTR(HTTP_FORM_INPUT_TXT);
    form.replace("{id}", "motion_pin");
    form.replace("{name}", "Motion Pin");
    form.replace("{value}", String(JConf.motion_pin));
  }

  if (atoi(JConf.dht_enable) == 1){
    form += FPSTR(HTTP_FORM_INPUT_TXT);
    form.replace("{id}", "dht_pin");
    form.replace("{name}", "DHT Pin");
    form.replace("{value}", String(JConf.dht_pin));
  }
  form += String(F("<br>"));

  form += FPSTR(HTTP_FORM_INPUT_TXT_UNIT);
  form.replace("{id}", "get_data_delay");
  form.replace("{name}", "Update Data Delay");
  form.replace("{value}", String(JConf.get_data_delay));
  form.replace("{unit}", "sec");

  if (atoi(JConf.motion_sensor_enable) == 1){
    form += FPSTR(HTTP_FORM_INPUT_TXT_UNIT);
    form.replace("{id}", "motion_read_delay");
    form.replace("{name}", "Motion Read Delay");
    form.replace("{value}", String(JConf.motion_read_delay));
    form.replace("{unit}", "sec");
  }

  #ifdef REBOOT_ON
    form += FPSTR(HTTP_FORM_INPUT_TXT_UNIT);
    form.replace("{id}", "reboot_delay");
    form.replace("{name}", "Reboot Delay");
    form.replace("{value}", String(JConf.reboot_delay));
    form.replace("{unit}", "sec");
  #endif //REBOOT_ON

  form += FPSTR(HTTP_BTN_SAVE);
  form += FPSTR(HTTP_FORM_END);

  String page = head + body + name + form + FPSTR(HTTP_BODY_END);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleEspConfig load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handleWifiConfig()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleWifiConfig Start");

  String head = FPSTR(HTTP_HEAD);
  head.replace("{module_id}", String(JConf.module_id));

  String js = FPSTR(JS_WIFI_SETTINGS);
  js.replace("{{module_id}}",         String(JConf.module_id));
  js.replace("{{wifi_mode}}",         String(JConf.wifi_mode));
  js.replace("{{wifi_phy_mode}}",     String(JConf.wifi_phy_mode));
  js.replace("{{wifi_channel}}",      String(JConf.wifi_channel));
  js.replace("{{wifi_auth}}",         String(JConf.wifi_auth));
  js.replace("{{sta_ssid}}",          String(JConf.sta_ssid));
  js.replace("{{static_ip_enable}}",  String(JConf.static_ip_enable));
  js.replace("{{static_ip}}",         String(JConf.static_ip));
  js.replace("{{static_subnet}}",     String(JConf.static_subnet));
  js.replace("{{static_gateway}}",    String(JConf.static_gateway));


  String body = FPSTR(HTTP_BODY);
  body.replace("{module_id}", String(JConf.module_id));

  String wifi = FPSTR(HTTP_WIFI_SETTINGS);

  String page = head + js + body + wifi;

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleWifiConfig load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(page);
}



void handleWifiScan()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: handleWifiScan Start");

  scanWiFi();

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: handleWifiScan load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  showPage(network_html);
}



void handleSave()
{
  char log[LOGSZ];
  byte what = 0, restart;

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Parameter save"));

  if (strlen(WebServer.arg("w").c_str())) {
    what = atoi(WebServer.arg("w").c_str());
  }
  switch (what) {
    case 1:
      if (strstr(WebServer.arg("mqtt_enable").c_str(),"1")) {
        strlcpy(JConf.mqtt_enable, "1", sizeof(JConf.mqtt_enable));
      } else {
        strlcpy(JConf.mqtt_enable, "0", sizeof(JConf.mqtt_enable));
      }
      strlcpy(JConf.mqtt_server, (!strlen(WebServer.arg("mqtt_server").c_str())) ? JConf.mqtt_server : WebServer.arg("mqtt_server").c_str(), sizeof(JConf.mqtt_server));
      strlcpy(JConf.mqtt_port, (!strlen(WebServer.arg("mqtt_port").c_str())) ? JConf.mqtt_port : WebServer.arg("mqtt_port").c_str(), sizeof(JConf.mqtt_port));

      if (strstr(WebServer.arg("mqtt_auth_enable").c_str(),"1")) {
        strlcpy(JConf.mqtt_auth_enable, "1", sizeof(JConf.mqtt_auth_enable));
      } else {
        strlcpy(JConf.mqtt_auth_enable, "0", sizeof(JConf.mqtt_auth_enable));
      }
      strlcpy(JConf.mqtt_user, (!strlen(WebServer.arg("mqtt_user").c_str())) ? JConf.mqtt_user : WebServer.arg("mqtt_user").c_str(), sizeof(JConf.mqtt_user));
      strlcpy(JConf.mqtt_pwd, (!strlen(WebServer.arg("mqtt_pwd").c_str())) ? JConf.mqtt_pwd : WebServer.arg("mqtt_pwd").c_str(), sizeof(JConf.mqtt_pwd));

      strlcpy(JConf.mqtt_name, (!strlen(WebServer.arg("mqtt_name").c_str())) ? JConf.mqtt_name : WebServer.arg("mqtt_name").c_str(), sizeof(JConf.mqtt_name));
      strlcpy(JConf.publish_topic, (!strlen(WebServer.arg("publish_topic").c_str())) ? JConf.publish_topic : WebServer.arg("publish_topic").c_str(), sizeof(JConf.publish_topic));
      strlcpy(JConf.subscribe_topic, (!strlen(WebServer.arg("subscribe_topic").c_str())) ? JConf.subscribe_topic : WebServer.arg("subscribe_topic").c_str(), sizeof(JConf.subscribe_topic));
      strlcpy(JConf.publish_delay, (!strlen(WebServer.arg("publish_delay").c_str())) ? JConf.publish_delay : WebServer.arg("publish_delay").c_str(), sizeof(JConf.publish_delay));
      strlcpy(JConf.subscribe_delay, (!strlen(WebServer.arg("subscribe_delay").c_str())) ? JConf.subscribe_delay : WebServer.arg("subscribe_delay").c_str(), sizeof(JConf.subscribe_delay));

      snprintf_P(log, sizeof(log), PSTR("HTTP: MQTT mqtt_enable %s, mqtt_server %s, mqtt_port %s, mqtt_auth_enable %s, mqtt_user %s"),
        JConf.mqtt_enable, JConf.mqtt_server, JConf.mqtt_port, JConf.mqtt_auth_enable, JConf.mqtt_user);
      addLog(LOG_LEVEL_INFO, log);

      snprintf_P(log, sizeof(log), PSTR("HTTP: MQTT mqtt_name %s, publish_topic %s, subscribe_topic %s, publish_delay %s, subscribe_delay %s"),
        JConf.mqtt_name, JConf.publish_topic, JConf.subscribe_topic, JConf.publish_delay, JConf.subscribe_delay);
      addLog(LOG_LEVEL_INFO, log);

      JConf.saveConfig();
      handleMqttConfig();
      break;
    case 2:  //Sensors Config save
      if (strstr(WebServer.arg("bme280_enable").c_str(),"1")) {
        strlcpy(JConf.bme280_enable, "1", sizeof(JConf.bme280_enable));
      } else {
        strlcpy(JConf.bme280_enable, "0", sizeof(JConf.bme280_enable));
      }
      if (strstr(WebServer.arg("sht21_enable").c_str(),"1")) {
        strlcpy(JConf.sht21_enable, "1", sizeof(JConf.sht21_enable));
      } else {
        strlcpy(JConf.sht21_enable, "0", sizeof(JConf.sht21_enable));
      }
      if (strstr(WebServer.arg("dht_enable").c_str(),"1")) {
        strlcpy(JConf.dht_enable, "1", sizeof(JConf.dht_enable));
      } else {
        strlcpy(JConf.dht_enable, "0", sizeof(JConf.dht_enable));
      }
      if (strstr(WebServer.arg("ds18x20_enable").c_str(),"1")) {
        strlcpy(JConf.ds18x20_enable, "1", sizeof(JConf.ds18x20_enable));
      } else {
        strlcpy(JConf.ds18x20_enable, "0", sizeof(JConf.ds18x20_enable));
      }
      if (strstr(WebServer.arg("bh1750_enable").c_str(),"1")) {
        strlcpy(JConf.bh1750_enable, "1", sizeof(JConf.bh1750_enable));
      } else {
        strlcpy(JConf.bh1750_enable, "0", sizeof(JConf.bh1750_enable));
      }
      if (strstr(WebServer.arg("motion_sensor_enable").c_str(),"1")) {
        strlcpy(JConf.motion_sensor_enable, "1", sizeof(JConf.motion_sensor_enable));
      } else {
        strlcpy(JConf.motion_sensor_enable, "0", sizeof(JConf.motion_sensor_enable));
      }
      if (strstr(WebServer.arg("pzem_enable").c_str(),"1")) {
        strlcpy(JConf.pzem_enable, "1", sizeof(JConf.pzem_enable));
      } else {
        strlcpy(JConf.pzem_enable, "0", sizeof(JConf.pzem_enable));
      }
      if (strstr(WebServer.arg("mhz19_enable").c_str(),"1")) {
        strlcpy(JConf.mhz19_enable, "1", sizeof(JConf.mhz19_enable));
      } else {
        strlcpy(JConf.mhz19_enable, "0", sizeof(JConf.mhz19_enable));
      }

      snprintf_P(log, sizeof(log), PSTR("HTTP: Sensors bme280_enable %s, sht21_enable %s, dht_enable %s, bh1750_enable %s, motion_sensor_enable %s, pzem_enable %s"),
        JConf.bme280_enable, JConf.sht21_enable, JConf.dht_enable, JConf.bh1750_enable, JConf.motion_sensor_enable, JConf.pzem_enable);
      addLog(LOG_LEVEL_INFO, log);

      JConf.saveConfig();
      handleSensorsConfig();
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
    case 4:  //MQTT Config save
      if (strstr(WebServer.arg("ntp_enable").c_str(),"1")) {
        strlcpy(JConf.ntp_enable, "1", sizeof(JConf.ntp_enable));
      } else {
        strlcpy(JConf.ntp_enable, "0", sizeof(JConf.ntp_enable));
      }
      strlcpy(JConf.ntp_server, (!strlen(WebServer.arg("ntp_server").c_str())) ? JConf.ntp_server : WebServer.arg("ntp_server").c_str(), sizeof(JConf.ntp_server));
      strlcpy(JConf.my_time_zone, (!strlen(WebServer.arg("my_time_zone").c_str())) ? JConf.my_time_zone : WebServer.arg("my_time_zone").c_str(), sizeof(JConf.my_time_zone));

      snprintf_P(log, sizeof(log), PSTR("HTTP: NTP ntp_enable %s, ntp_server %s, my_time_zone %s"), JConf.ntp_enable, JConf.ntp_server, JConf.my_time_zone);
      addLog(LOG_LEVEL_INFO, log);

      #ifdef NTP_ON
        NTPSettingsUpdate();
      #endif //NTP_ON

      JConf.saveConfig();
      handleNtpConfig();
      break;
    case 5:  //ESP Config save
      strlcpy(JConf.light_pin, (!strlen(WebServer.arg("light_pin").c_str())) ? JConf.light_pin : WebServer.arg("light_pin").c_str(), sizeof(JConf.light_pin));
      strlcpy(JConf.lightoff_delay, (!strlen(WebServer.arg("lightoff_delay").c_str())) ? JConf.lightoff_delay : WebServer.arg("lightoff_delay").c_str(), sizeof(JConf.lightoff_delay));
      strlcpy(JConf.lighton_lux, (!strlen(WebServer.arg("lighton_lux").c_str())) ? JConf.lighton_lux : WebServer.arg("lighton_lux").c_str(), sizeof(JConf.lighton_lux));

      if (strstr(WebServer.arg("light_smooth").c_str(),"1")) {
        strlcpy(JConf.light_smooth, "1", sizeof(JConf.light_smooth));
      } else {
        strlcpy(JConf.light_smooth, "0", sizeof(JConf.light_smooth));
      }

      strlcpy(JConf.light2_pin, (!strlen(WebServer.arg("light2_pin").c_str())) ? JConf.light2_pin : WebServer.arg("light2_pin").c_str(), sizeof(JConf.light2_pin));
      strlcpy(JConf.light2off_delay, (!strlen(WebServer.arg("light2off_delay").c_str())) ? JConf.light2off_delay : WebServer.arg("light2off_delay").c_str(), sizeof(JConf.light2off_delay));
      strlcpy(JConf.light2on_lux, (!strlen(WebServer.arg("light2on_lux").c_str())) ? JConf.light2on_lux : WebServer.arg("light2on_lux").c_str(), sizeof(JConf.light2on_lux));

      if (strstr(WebServer.arg("light2_smooth").c_str(),"1")) {
        strlcpy(JConf.light2_smooth, "1", sizeof(JConf.light2_smooth));
      } else {
        strlcpy(JConf.light2_smooth, "0", sizeof(JConf.light2_smooth));
      }

      strlcpy(JConf.motion_pin, (!strlen(WebServer.arg("motion_pin").c_str())) ? JConf.motion_pin : WebServer.arg("motion_pin").c_str(), sizeof(JConf.motion_pin));
      strlcpy(JConf.dht_pin, (!strlen(WebServer.arg("dht_pin").c_str())) ? JConf.dht_pin : WebServer.arg("dht_pin").c_str(), sizeof(JConf.dht_pin));
      strlcpy(JConf.get_data_delay, (!strlen(WebServer.arg("get_data_delay").c_str())) ? JConf.get_data_delay : WebServer.arg("get_data_delay").c_str(), sizeof(JConf.get_data_delay));
      strlcpy(JConf.motion_read_delay, (!strlen(WebServer.arg("motion_read_delay").c_str())) ? JConf.motion_read_delay : WebServer.arg("motion_read_delay").c_str(), sizeof(JConf.motion_read_delay));
      strlcpy(JConf.reboot_delay, (!strlen(WebServer.arg("reboot_delay").c_str())) ? JConf.reboot_delay : WebServer.arg("reboot_delay").c_str(), sizeof(JConf.reboot_delay));

      JConf.saveConfig();
      handleEspConfig();
      break;
    case 6:  //WiFi Config save
      strlcpy(JConf.module_id, (!strlen(WebServer.arg("module_id").c_str())) ? JConf.module_id : WebServer.arg("module_id").c_str(), sizeof(JConf.module_id));
      strlcpy(JConf.wifi_mode, (!strlen(WebServer.arg("wifi_mode").c_str())) ? JConf.wifi_mode : WebServer.arg("wifi_mode").c_str(), sizeof(JConf.wifi_mode));
      strlcpy(JConf.wifi_phy_mode, (!strlen(WebServer.arg("wifi_phy_mode").c_str())) ? JConf.wifi_phy_mode : WebServer.arg("wifi_phy_mode").c_str(), sizeof(JConf.wifi_phy_mode));
      strlcpy(JConf.wifi_channel, (!strlen(WebServer.arg("wifi_channel").c_str())) ? JConf.wifi_channel : WebServer.arg("wifi_channel").c_str(), sizeof(JConf.wifi_channel));
      strlcpy(JConf.wifi_auth, (!strlen(WebServer.arg("wifi_auth").c_str())) ? JConf.wifi_auth : WebServer.arg("wifi_auth").c_str(), sizeof(JConf.wifi_auth));
      strlcpy(JConf.ap_pwd, (!strlen(WebServer.arg("ap_pwd").c_str())) ? JConf.ap_pwd : WebServer.arg("ap_pwd").c_str(), sizeof(JConf.ap_pwd));
      strlcpy(JConf.sta_ssid, (!strlen(WebServer.arg("sta_ssid").c_str())) ? JConf.sta_ssid : WebServer.arg("sta_ssid").c_str(), sizeof(JConf.sta_ssid));
      strlcpy(JConf.sta_pwd, (!strlen(WebServer.arg("sta_pwd").c_str())) ? JConf.sta_pwd : WebServer.arg("sta_pwd").c_str(), sizeof(JConf.sta_pwd));

      if (strstr(WebServer.arg("static_ip_enable").c_str(),"1")) {
        strlcpy(JConf.static_ip_enable, "1", sizeof(JConf.static_ip_enable));
      } else {
        strlcpy(JConf.static_ip_enable, "0", sizeof(JConf.static_ip_enable));
      }

      strlcpy(JConf.static_ip, (!strlen(WebServer.arg("static_ip").c_str())) ? JConf.static_ip : WebServer.arg("static_ip").c_str(), sizeof(JConf.static_ip));
      strlcpy(JConf.static_subnet, (!strlen(WebServer.arg("static_subnet").c_str())) ? JConf.static_subnet : WebServer.arg("static_subnet").c_str(), sizeof(JConf.static_subnet));
      strlcpy(JConf.static_gateway, (!strlen(WebServer.arg("static_gateway").c_str())) ? JConf.static_gateway : WebServer.arg("static_gateway").c_str(), sizeof(JConf.static_gateway));

/*
      snprintf_P(log, sizeof(log), PSTR(
        "module_id %s, wifi_mode %s, wifi_phy_mode %s, wifi_channel %s, wifi_auth %s, sta_ssid %s"),
        JConf.module_id, JConf.wifi_mode, JConf.wifi_phy_mode, JConf.wifi_channel, JConf.wifi_auth, JConf.sta_ssid);
      addLog(LOG_LEVEL_INFO, log);
      snprintf_P(log, sizeof(log), PSTR(
        "static_ip_enable %s, static_ip %s, static_subnet %s, static_gateway %s"),
        JConf.static_ip_enable, JConf.static_ip, JConf.static_subnet, JConf.static_gateway);
      addLog(LOG_LEVEL_INFO, log);
*/
      JConf.saveConfig();
      handleWifiConfig();
      break;
  }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////   WEB PAGES  Start  //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
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
        if (fading[0].cycleEnd != 0){
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
        if (fading[1].cycleEnd != 0){
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

  if (fading[0].cycleEnd != 0){
    pinState = true;
  } else {
    pinState = false;
  }

  if (fading[1].cycleEnd != 0){
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

  #ifdef MHZ19_ON
    if (atoi(JConf.mhz19_enable) == 1){
      XML+=String(F("<mhz19ppm>"));
      XML+=mhz19PpmString;
      XML+=String(F(" PPM"));
      XML+=String(F("</mhz19ppm>"));
    }
  #endif

  #ifdef DS18X20_ON
    if (atoi(JConf.ds18x20_enable) == 1){
      for (size_t i = 0; i < findDsSensors; i++)
      {
        XML+=String(F("<ds")) + String(i) +  String(F(">"));
        XML+=dsData[i].dsTemp;
        XML+=String(F(" C"));
        XML+=String(F("</ds")) + String(i) +  String(F(">"));
      }
    }
  #endif //DS18X20_ON


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



void WebServerInit() {
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: WebServerInit Start");
  // Prepare webserver pages
  WebServer.on("/", handleRoot);
  WebServer.on("/reboot", handleReboot);
  WebServer.on("/update", handleUpdate);
  WebServer.onFileUpload(WebFileUpload);
  WebServer.on("/upload_sketch", handleUploadSketch);
  WebServer.on("/wificonf", handleWifiConfig);
  WebServer.on("/scan", handleWifiScan);
  WebServer.on("/sensorsconf", handleSensorsConfig);
  WebServer.on("/espconf", handleEspConfig);
  WebServer.on("/mqttconf", handleMqttConfig);
  WebServer.on("/ntpconf", handleNtpConfig);
  WebServer.on("/control", handleControl);
  WebServer.on("/pincontrol", handlePinControl);
  WebServer.on("/controlstatus", WebPinControlStatus);
  WebServer.on("/log", handleLogConfig);
  WebServer.on("/save", handleSave);

  WebServer.on("/cm", handleConsole);
  WebServer.on("/ax", handleAjax);

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

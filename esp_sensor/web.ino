#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


const char headerP[] PROGMEM = 
"<html lang='en'><head><title>{module_id}</title><meta charset='utf-8'"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"<link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>"
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js'></script>"
"<script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script></head>"
"<body>"
"<nav class='navbar navbar-inverse'>"
  "<div class='container-fluid'>"
    "<div class='navbar-header'>"
      "<a class='navbar-brand' href='/'>{module_id}</a>"
    "</div>"
    "<div>"
      "<ul class='nav navbar-nav'>"
        "<li><a href='/'><span class='glyphicon glyphicon-dashboard'></span> Status</a></li>"
        "<li><a href='/pincontrol'><span class='glyphicon glyphicon-tasks'></span> Control Pins</a></li>"
        "<li class='dropdown'><a class='dropdown-toggle' data-toggle='dropdown' href='#'>"
        "<span class='glyphicon glyphicon-cog'></span> Configure<span class='caret'></span></a>"
          "<ul class='dropdown-menu'>"
            "<li><a href='/wificonf'>Wi-Fi</a></li>"
            "<li><a href='/sensorsconf'>Sensors</a></li>"
            "<li><a href='/espconf'>ESP</a></li>"
            "<li><a href='/mqttconf'>MQTT</a></li>"
            "<li><a href='/ntpconf'>NTP time</a></li>"
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



const char containerP[] PROGMEM =  
        "<div class='clearfix visible-lg'>"
        "</div>"
      "</div>"
    "</div>"
  "</body>"
"</html>";



const char HTTP_FORM_LOG[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Logging parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='3' hidden><input id='r' name='r' value='0' hidden>"
  "<br/><b>Serial log level</b> (" STR(SERIAL_LOG_LEVEL) ")<br/><select id='ls' name='ls'>"
  "<option{a0value='0'>0 None</option>"
  "<option{a1value='1'>1 Error</option>"
  "<option{a2value='2'>2 Info</option>"
  "<option{a3value='3'>3 Debug</option>"
  "<option{a4value='4'>4 More debug</option>"
  "</select></br>"
  "<br/><b>Web log level</b> (" STR(WEB_LOG_LEVEL) ")<br/><select id='lw' name='lw'>"
  "<option{b0value='0'>0 None</option>"
  "<option{b1value='1'>1 Error</option>"
  "<option{b2value='2'>2 Info</option>"
  "<option{b3value='3'>3 Debug</option>"
  "<option{b4value='4'>4 More debug</option>"
  "</select></br>"
  "<br/><b>Syslog level</b> (" STR(SYS_LOG_LEVEL) ")<br/><select id='ll' name='ll'>"
  "<option{c0value='0'>0 None</option>"
  "<option{c1value='1'>1 Error</option>"
  "<option{c2value='2'>2 Info</option>"
  "<option{c3value='3'>3 Debug</option>"
  "<option{c4value='4'>4 More debug</option>"
  "</select></br>"
  "<br/><b>Syslog host</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' length=32 placeholder='" SYS_LOG_HOST "' value='{l2}'><br/>"
  "<br/><b>Syslog port</b> (" STR(SYS_LOG_PORT) ")<br/><input id='lp' name='lp' length=5 placeholder='" STR(SYS_LOG_PORT) "' value='{l3}'><br/>"
  "<br/><b>Telemetric period</b> (" STR(TELE_PERIOD) ")<br/><input id='lt' name='lt' length=4 placeholder='" STR(TELE_PERIOD) "' value='{l4}'><br/>";

const char HTTP_FORM_END[] PROGMEM =
  "<br/><button type='submit'>Save</button></form></fieldset>";


const char HTTP_BTN_CONF[] PROGMEM =
  "<br/><br/><form action='/cn' method='post'><button>Configuration menu</button></form>";


void handleTest()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: handleTest"));

  String page = FPSTR(headerP);
  page.replace("{module_id}", String(JConf.module_id));
  page += FPSTR(containerP);
  showPage(page);
}



void handleLog()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Log config"));

  String page = FPSTR(headerP);
  page.replace("{module_id}", String(JConf.module_id));
  page += FPSTR(HTTP_FORM_LOG);
  for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
    page.replace("{a" + String(i), (i == sysCfg.seriallog_level) ? " selected " : " ");
    page.replace("{b" + String(i), (i == sysCfg.weblog_level) ? " selected " : " ");
    page.replace("{c" + String(i), (i == sysCfg.syslog_level) ? " selected " : " ");
  }  
  page.replace("{l2}", String(sysCfg.syslog_host));
  page.replace("{l3}", String(sysCfg.syslog_port));
  page.replace("{l4}", String(sysCfg.tele_period));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(containerP);
  showPage(page);
}




void showPage(String &page)
{
  WebServer.send(200, "text/html", page);
}
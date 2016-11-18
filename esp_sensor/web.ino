//#define STR_HELPER(x) #x
//#define STR(x) STR_HELPER(x)


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



const char containerP[] PROGMEM =  
        "<div class='clearfix visible-lg'>"
        "</div>"
      "</div>"
    "</div>"
  "</body>"
"</html>";



const char HTTP_FORM_LOG[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Logging parameters&nbsp;</b></legend><form method='post' action='save'>"
  "<input id='w' name='w' value='3' hidden><input id='r' name='r' value='0' hidden>"
  "<br/><b>Serial log level</b> ( {serial_log_level} )<br/><select id='ls' name='ls'>"
  "<option{a0value='0'>0 None</option>"
  "<option{a1value='1'>1 Error</option>"
  "<option{a2value='2'>2 Info</option>"
  "<option{a3value='3'>3 Debug</option>"
  "<option{a4value='4'>4 More debug</option>"
  "</select></br>"
  "<br/><b>Web log level</b> ( {web_log_level} )<br/><select id='lw' name='lw'>"
  "<option{b0value='0'>0 None</option>"
  "<option{b1value='1'>1 Error</option>"
  "<option{b2value='2'>2 Info</option>"
  "<option{b3value='3'>3 Debug</option>"
  "<option{b4value='4'>4 More debug</option>"
  "</select></br>"
  "<br/><b>Syslog level</b> ( {sys_log_level} )<br/><select id='ll' name='ll'>"
  "<option{c0value='0'>0 None</option>"
  "<option{c1value='1'>1 Error</option>"
  "<option{c2value='2'>2 Info</option>"
  "<option{c3value='3'>3 Debug</option>"
  "<option{c4value='4'>4 More debug</option>"
  "</select></br>"
  "<br/><b>Syslog host</b> ( {sys_log_host} )<br/><input id='lh' name='lh' length=32 placeholder=' {sys_log_host} ' value='{sys_log_host}'><br/>"
  "<br/><b>Syslog port</b> ( {sys_log_port} )<br/><input id='lp' name='lp' length=5 placeholder=' {sys_log_port} ' value='{sys_log_port}'><br/>";

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
  page.replace("{serial_log_level}", String(JConf.serial_log_level));
  page.replace("{web_log_level}", String(JConf.web_log_level));
  page.replace("{sys_log_level}", String(JConf.sys_log_level));
  page.replace("{sys_log_host}", String(JConf.sys_log_host));
  page.replace("{sys_log_port}", String(JConf.sys_log_port));
  for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
    page.replace("{a" + String(i), (i == atoi(JConf.serial_log_level)) ? " selected " : " ");
    page.replace("{b" + String(i), (i == atoi(JConf.web_log_level)) ? " selected " : " ");
    page.replace("{c" + String(i), (i == atoi(JConf.sys_log_level)) ? " selected " : " ");
  }  
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(containerP);
  showPage(page);
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
  case 3:
    strlcpy(JConf.serial_log_level, (!strlen(WebServer.arg("ls").c_str())) ? JConf.serial_log_level : WebServer.arg("ls").c_str(), sizeof(JConf.serial_log_level));
    strlcpy(JConf.web_log_level, (!strlen(WebServer.arg("lw").c_str())) ? JConf.web_log_level : WebServer.arg("lw").c_str(), sizeof(JConf.web_log_level));
    strlcpy(JConf.sys_log_level, (!strlen(WebServer.arg("ll").c_str())) ? JConf.sys_log_level : WebServer.arg("ll").c_str(), sizeof(JConf.sys_log_level));
    strlcpy(JConf.sys_log_host, (!strlen(WebServer.arg("lh").c_str())) ? JConf.sys_log_host : WebServer.arg("lh").c_str(), sizeof(JConf.sys_log_host));
    strlcpy(JConf.sys_log_port, (!strlen(WebServer.arg("lp").c_str())) ? JConf.sys_log_port : WebServer.arg("lp").c_str(), sizeof(JConf.sys_log_port));

    snprintf_P(log, sizeof(log), PSTR("HTTP: Logging Seriallog %s, Weblog %s, Syslog %s, Host %s, Port %s"),
      JConf.serial_log_level, JConf.web_log_level, JConf.sys_log_level, JConf.sys_log_host, JConf.sys_log_port);
    addLog(LOG_LEVEL_INFO, log);
    JConf.saveConfig();
    handleLog();
    break;
  }
}










void showPage(String &page)
{
  WebServer.send(200, "text/html", page);
}
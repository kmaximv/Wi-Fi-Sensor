

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
"</nav>";


const char containerP[] PROGMEM =  

    "<div class='container'>"
      "<div class='row'>"
        "<div class='clearfix visible-lg'>"
        "</div>"
      "</div>"
    "</div>"
  "</body>"
"</html>";


void handleTest()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: handleTest"));

  String page = FPSTR(headerP);
  page.replace("{module_id}", String(JConf.module_id));
  page += FPSTR(containerP);
  showPage(page);
}


void showPage(String &page)
{
  WebServer.send(200, "text/html", page);
}

#include "config.h"
#include "common_def.h"

#include "config_json_stream_parser.h"

#include <ESP8266httpUpdate.h>

//ConfigListener * config_lstnr;

//Device_config * config;

bool updateFirmware()
{

    bool status = false ;
   t_httpUpdate_return ret = (t_httpUpdate_return) ESPhttpUpdate.update(
       php_upgrade_server
       , php_upgrade_server_port
       , String(php_upgrade_server_file_target)+String("/firmware.bin")
       , __VERSION__);
switch(ret) 
{
    case HTTP_UPDATE_FAILED: 
        Serial.println("[update] Update failed.");
        syslog_debug("[update] Update no Update.");
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Update.");
        syslog_debug("[update] Update no Update.");

        break;
    case HTTP_UPDATE_OK:
        // Send msg to update the db for update was done.

        status = true;

        Serial.println("[update] Update ok."); // may not called we reboot the ESP
        syslog_debug("[update] Update ok.");

        break;
}

return status;

}

bool processConfig()
{
  
   bool status = false;
   ConfigListener * config_lstnr = getJsonConfigListenerPtr();

   Device_config * config = config_lstnr->getDeviceConfigPtr();

   // timed

   if(FW_UPDATE_AVAILABLE==config->whether_update_available[0])
   {
       status = updateFirmware();
   }

   if(status == true)
   {
       status = updateCodeUpdateStatus();
   }

   // check others
   // e.g. buzzer and leds

return status;

}
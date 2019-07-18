
#include "config.h"
#include "common_def.h"

#include "config_json_stream_parser.h"

#include <ESP8266httpUpdate.h>

void updateFirmware()
{
   t_httpUpdate_return ret = (t_httpUpdate_return)0;// ESPhttpUpdate.update("192.168.0.2", 80, "/esp/update/arduino.php", "optional current version string here");
switch(ret) 
{
    case HTTP_UPDATE_FAILED: 
        Serial.println("[update] Update failed.");
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Update.");
        break;
    case HTTP_UPDATE_OK:
        // Send msg to update the db for update was done.
        Serial.println("[update] Update ok."); // may not called we reboot the ESP
        break;
}



}

void processConfig()
{
  
   ConfigListener * config_lstnr = getJsonConfigListenerPtr();

   Device_config * config = config_lstnr->getDeviceConfigPtr();

   if(FW_UPDATE_AVAILABLE==config->whether_update_available[0])
   {
       updateFirmware();
   }


}
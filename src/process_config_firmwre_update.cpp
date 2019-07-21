
#include "config.h"
#include "common_def.h"

#include "config_json_stream_parser.h"

#include <ESP8266httpUpdate.h>

//ConfigListener * config_lstnr;

//Device_config * config;

bool updateFirmware()
{

    bool status = false;
    String path_to_firmware = String(php_upgrade_server_file_target) + String("/firmware.bin");

    ESPhttpUpdate.rebootOnUpdate(false);

    unsigned long time_spent_to_update = millis();

    t_httpUpdate_return ret = (t_httpUpdate_return)ESPhttpUpdate.update(
        php_upgrade_server, php_upgrade_server_port, path_to_firmware, _VER_);

    time_spent_to_update = millis() - time_spent_to_update;

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        sprintf(getPrintBuffer(), "%s", path_to_firmware.c_str());
        Serial.println(getPrintBuffer());
        syslog_debug(getPrintBuffer());

        sprintf(getPrintBuffer(), "HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        Serial.println(getPrintBuffer());
        syslog_debug(getPrintBuffer());

        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Update.");
        syslog_debug("[update] Update no Update.");

        break;
    case HTTP_UPDATE_OK:
        // Send msg to update the db for update was done.

        status = true;

        sprintf(getPrintBuffer(), "[update] Update ok. - time spent on update %d", time_spent_to_update);

        Serial.println(getPrintBuffer()); // may not called we reboot the ESP
        syslog_debug(getPrintBuffer());

        break;
    }

    return status;
}

bool processConfig()
{

    bool status = false;
    ConfigListener *config_lstnr = getJsonConfigListenerPtr();

    Device_config *config = config_lstnr->getDeviceConfigPtr();

    // timed

    if (FW_UPDATE_AVAILABLE == config->whether_update_available[0])
    {
        sprintf(getPrintBuffer(), "code update in progress (can take upto 5 minutes max)...");
        Serial.println(getPrintBuffer());
        syslog_debug(getPrintBuffer());
        delay(10);
        yield();

        status = updateFirmware();
    }

    if (status == true)
    {
        status = updateCodeUpdateStatus();
    }

    // check others
    // e.g. buzzer and leds

    return status;
}
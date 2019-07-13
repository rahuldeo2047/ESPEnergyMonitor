#include <FS.h> //this needs to be first, or it all crashes and burns...

#include <Arduino.h>
#include "config.h"

#include "JsonStreamingParser.h"
#include "JsonListener.h"
#include "config_json_stream_parser.h"

///extern "C"{
#include "common_def.h"

// MAX_PRINT_BUFFER_SIZE
//char print_buffer[];// = {0}; //externed from config.h

//};

#include <ESP8266WiFi.h>


JsonStreamingParser parser;
ConfigListener listener;
 

bool whether_post_wifi_connect_setup_done_json_test;

/*
void syslog_debug(char* msg){logger.debug(msg);}
void syslog_info(char* msg){logger.info(msg);}
void syslog_warn(char* msg){logger.warn(msg);}
void syslog_error(char* msg){logger.error(msg);}
 */
void setup()
{
    Serial.begin(115200);

    wifimanager_setup();

    Serial.print("Version: ");
    Serial.println(_VER_);
    syslog_info("Version");
    syslog_info(_VER_);

    String mac_str = (WiFi.macAddress());
    mac_str.replace(":", "");
    strncpy(DEVICE_ID_STR, mac_str.c_str(), 13);

    String device_id_based_ssid = "Device hotspot can be EEWD_" + String(DEVICE_ID_STR);
    Serial.println(device_id_based_ssid);
    syslog_info((char *)device_id_based_ssid.c_str());

    //syslog_debug

    parser.setListener(&listener);
    // put your setup code here, to run once:
    char json[] = "{\"a\":3, \"b\":{\"c\":\"d\"}}";
    for (int i = 0; i < sizeof(json); i++)
    {
        parser.parse(json[i]);
    }
}



void loop() 
{
    if (false == whether_post_wifi_connect_setup_done_json_test)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      setup_OTA();
      //rd_setup(HOST_NAME);
      whether_post_wifi_connect_setup_done_json_test = true;
    }
  }
  if (true == whether_post_wifi_connect_setup_done_json_test)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      if (loop_OTA())
      {
        return;
      }
      //rd_loop();
    }
  }
}
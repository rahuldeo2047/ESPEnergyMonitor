
//#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h> 

#include "config.h"

#include "Syslog.h"
 
#include "common_def.h"
 

//WiFiUDP udp;

//+String(" ")+String(DEVICE_DEVELOPMENT_TYPE)).c_str()
static Syslog logger(SYSLOG_DEVICE_NAME,"" , SYSLOG_SERVER_ADDRESS);//""192.168.43.200");
// void setup() {
//   Serial.begin(115200); 
// }

//#define syslog_(type_of_msg) void syslog_##type_of_msg(char* msg) { logger.##type_of_msg(msg);}

// void syslog_debug(char* msg);
// void syslog_info(char* msg);
// void syslog_warn(char* msg);
// void syslog_error(char* msg);

 void syslog_debug(char* msg){logger.debug(msg);}
 void syslog_info(char* msg){logger.info(msg);}
 void syslog_warn(char* msg){logger.warn(msg);}
 void syslog_error(char* msg){logger.error(msg);}

// void loop() {
//   logger..debug("running loop");
//   delay(2000);
// }


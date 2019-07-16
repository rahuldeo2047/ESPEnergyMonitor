#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include "config_json_stream_parser.h"

#include <ESP8266WiFi.h>

void wifimanager_setup();

void syslog_debug(char* msg);
void syslog_info(char* msg);
void syslog_warn(char* msg);
void syslog_error(char* msg);

bool mpu_setup();
float mpu_getTempFiltered();
float mpu_getTemp();
float mpu_getAccelFftMagFiltered();
float mpu_getAccelFftMag();
void mpu_resetSampleTimer();

void Irms_setup();
void Irms_resetSampleTimer();
bool Irms_loop(); // It is not measuring status
float Irms_getCurr();
float Irms_getFilteredCurr();

void setup_php_server();

void setup_OTA();

bool loop_OTA();

void mpu_loop();

bool loop_php_server(unsigned long _php_sr, unsigned long _php_uptm, float _php_temp_f, float _php_temp_r, float _php_current_f, float _php_current_r, float _php_accel_f, float _php_accel_r);

ConfigListener * getJsonConfigListenerPtr();

inline String getDeviceMacIdStr()
{
    String mac_str = (WiFi.macAddress());
    mac_str.replace(":", "");
    return mac_str;
}

#endif //COMMON_DEF_H

#include <Arduino.h>
#include "config.h"
#include "data.h"
#include "version.h"

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

HTTPClient httpClient;

void phpgsheet_Init(gScript_motor_status status, float Irms, gScript_type type )
{


  String url ="/logger.php?";
  String urlFinal = url
  + "type="+ type_str[type]
  + "&ellapsed_time=" + String(millis())
  + "&current_factor=" + String(Irms)
  + "&motor_status="+motor_sataus_str[status];

  if(powerOn==type)
  {
    urlFinal += "&version="+String(_VER_);
  }

  Serial.println(urlFinal);


  //httpClient.setReuse(true);
  httpClient.begin(PHP_HOST,80,urlFinal); //HTTP
  httpClient.setUserAgent("ESP8266 watermotor meter");
  //Serial.print("[HTTP] GET...\n");
  int httpCode = httpClient.GET();

  // httpCode will be negative on error
  if(httpCode > 0)
  {
    //HTTP header has been send and Server response header has been handled
    //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    Serial.printf("[HTTP] GET... code: [%d] %s\n", httpCode, httpClient.errorToString(httpCode).c_str());


    //file found at server
    //if(httpCode == HTTP_CODE_OK)
    {
      String payload = httpClient.getString();
      Serial.println(payload);
    }

  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
  }


  httpClient.end();

}

void phpgsheet_Loop(gScript_motor_status status, float Irms, gScript_type type, unsigned long dt )
{
  if(
    (googleSpreadSheetHBKeepReadyUpdateTime > (googleSpreadSheetUpdateTime_hb_keepready))// beready 5 second earlier
    ||
    (hb != type)
    //(googleSpreadSheetMotorStatusUpdateTime > (googleSpreadSheetUpdateTime_motorstatus-5000))// beready 5 second earlier
  )
  {
    googleSpreadSheetHBKeepReadyUpdateTime = 0;
    //int rem = ( millis() / 1000 )%60 ;//seconds;
    //googleSpreadSheetUpdateTime_hb_keepready -= ((rem-5)*1000);
    //googlespreadsheet_keepready();
  }

  if(
    (googleSpreadSheetHBUpdateTime > googleSpreadSheetUpdateTime_hb)
    ||
    (hb != type)
  )
  {

    googleSpreadSheetHBUpdateTime = 0;
    googleSpreadSheetHBKeepReadyUpdateTime = 0;

    Serial.print(millis());
    Serial.print(" : sending data to gScript for type ");
    Serial.println(type_str[type]);

    //googlespreadsheet_keepready();
    //
    String url ="/logger.php?";
    String urlFinal = url
    + "type="+ type_str[type]
    + "&ellapsed_time=" + String(dt)
    + "&current_factor=" + String(Irms)
    + "&motor_status="+motor_sataus_str[status];

    urlFinal += "&version="+String(_VER_);

    Serial.println(urlFinal);


    //httpClient.setReuse(true);
    httpClient.begin(PHP_HOST,80,urlFinal); //HTTP
    httpClient.setUserAgent("ESP8266 watermotor meter");
    //Serial.print("[HTTP] GET...\n");
    int httpCode = httpClient.GET();

    // httpCode will be negative on error
    if(httpCode > 0)
    {
      //HTTP header has been send and Server response header has been handled
      //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      Serial.printf("[HTTP] GET... code: [%d] %s\n", httpCode, httpClient.errorToString(httpCode).c_str());


      //file found at server
      //if(httpCode == HTTP_CODE_OK)
      {
        String payload = httpClient.getString();
        Serial.println(payload);
      }

    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
    }


    httpClient.end();
  }

}

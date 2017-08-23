
#include <Arduino.h>
#include "config.h"
#include "data.h"
//#include "version.h"

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

HTTPClient httpClient;

HTTPClient client;

void checkInternetConnection()
{
  // In google script when motor status is UNKNOWN
  // sheet will not be updated

  //while(!client.connected())

  Serial.print("Checking internet connection ... ");

  int httpCode = -1;
  String url ="/logger.php?";
  String urlFinal = url
  + "type="+ type_str[test]
  + "&ellapsed_time=0"
  + "&current_factor=0"
  + "&motor_status="+motor_sataus_str[UNKNOWN];

  urlFinal += "&version="+String(_VER_);

  //client.connect(PHP_HOST_1, 80);

  do
  {
    client.begin(PHP_HOST_1,80,urlFinal); //HTTP
    client.setTimeout(5000);

    client.setUserAgent("ESP8266 watermotor meter");
    //Serial.print("[HTTP] GET...\n");
    httpCode = client.GET();

    if(httpCode > 0)
    {
      Serial.printf("\n[HTTP] GET... code: [%d] %s\n", httpCode, client.errorToString(httpCode).c_str());

      if(httpCode == HTTP_CODE_OK)
      {
        String payload = client.getString();
        Serial.println(payload);
      }
      else
      {
        Serial.print(".");
        delay(5000);
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: [%d] %s\n", httpCode, client.errorToString(httpCode).c_str());
      Serial.print(".");
      delay(5000);
    }

  }
  while((httpCode != HTTP_CODE_OK));

  client.end();

  Serial.print(" ... to the internet.\n");

}

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

  static bool toggleHost = true;
  //httpClient.setReuse(true);
  if(toggleHost)
  {
    toggleHost = false;
    if(false == httpClient.connected())
    {
      httpClient.begin(PHP_HOST,80,urlFinal); //HTTP
    }
    Serial.print(PHP_HOST);
  }
  else
  {
    toggleHost = true;
    if(false == httpClient.connected())
    {
      httpClient.begin(PHP_HOST_1,80,urlFinal); //HTTP
    }
    Serial.print(PHP_HOST_1);
  }

  Serial.println(urlFinal);

  //httpClient.setReuse(true);

  httpClient.setTimeout(5000);

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
    Serial.printf("[HTTP] GET... failed, error: [%d] %s\n", httpCode, httpClient.errorToString(httpCode).c_str());
  }


  //httpClient.end();

}

bool phpgsheet_Loop(gScript_motor_status status, float Irms, gScript_type type, unsigned long dt )
{

  static int hasFailedSentCount = 0;

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
    //timesync();
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
    //timesync();

    String url ="/logger.php?";
    String urlFinal = url
    + "type="+ type_str[type]
    + "&ellapsed_time=" + String(dt)
    + "&current_factor=" + String(Irms)
    + "&motor_status="+motor_sataus_str[status];

    urlFinal += "&version="+String(_VER_);

    static bool toggleHost = true;
    //httpClient.setReuse(true);
    if(toggleHost)
    {
      toggleHost = false;
      if(false == httpClient.connected())
      {
        httpClient.end();
        httpClient.begin(PHP_HOST,80,urlFinal); //HTTP
      }
      Serial.print(PHP_HOST);
    }
    else
    {
      toggleHost = true;
      if(false == httpClient.connected())
      {
        httpClient.end();
        httpClient.begin(PHP_HOST,80,urlFinal); //HTTP
      }
      Serial.print(PHP_HOST);
    }

    Serial.println(urlFinal);

    //httpClient.setReuse(true);
    //httpClient.begin(PHP_HOST,80,urlFinal); //HTTP
    httpClient.setTimeout(9000);
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
      if(httpCode == HTTP_CODE_OK)
      {
        hasFailedSentCount = 0;
      }

      String payload = httpClient.getString();
      Serial.println(payload);

    }
    else
    {
      hasFailedSentCount++;
      Serial.printf("[HTTP] GET... failed, error: [%d] %s\n", httpCode, httpClient.errorToString(httpCode).c_str());
    }


    //httpClient.end();
  }

  return hasFailedSentCount;

}

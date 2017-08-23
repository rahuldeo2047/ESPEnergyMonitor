
#include <Arduino.h>
#include "config.h"
#include "data.h"
//#include "version.h"

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecure.h>

#include "elapsedMillis.h"

elapsedMillis telegramhbUpdateTime;

HTTPClient httpClientTelegramBot;
WiFiClientSecure client;
const char* fingerprintT = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";


String createTelegramChatAction(String _BotToken, String action, String chatID)
{
  String url = String("/bot")+String(_BotToken)+String("/sendChatAction?action=")+String(action)+String("&chat_id=")+String(chatID);
  return url;
  //var response = UrlFetchApp.fetch(url, {'muteHttpExceptions': true});

}

bool sendActionFromClient()
{

  if(telegramhbUpdateTime > 10000)
  {
    telegramhbUpdateTime = 0;

    String url = createTelegramChatAction(BOTtoken, "typing", telegramGroupID);
    Serial.print(TELEGRAM_HOST);
    Serial.print(url);

    if (!client.connect(TELEGRAM_HOST, 443))
    {
      Serial.println("connection failed");
      return false;
    }

    if (client.verify(fingerprintT, TELEGRAM_HOST))
    {
      Serial.println("certificate matches");
    }
    else
    {
      Serial.println("certificate doesn't match");
    }

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
    "Host: " + TELEGRAM_HOST + "\r\n" +
    "User-Agent: ESP8266\r\n" +
    "Connection: close\r\n\r\n");

    Serial.println("request sent");
String line = "";
    while (client.connected() || client.available())
    {
      line += client.readStringUntil('\n');
      line += "\n\r";
      //if (line == "\r")
      {
        Serial.println("headers received");
        break;
      }
    }

    //String line = client.readStringUntil('\n');

    // if (line.startsWith("{\"state\":\"success\""))
    // {
    //   Serial.println("esp8266/Arduino CI successfull!");
    // }
    // else
    // {
    //   Serial.println("esp8266/Arduino CI has failed");
    // }

    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(line);
    Serial.println("==========");
    Serial.println("closing connection");

    return true;

  }
}

bool sendAction()
{

  if(telegramhbUpdateTime > 10000)
  {
    telegramhbUpdateTime = 0;

    String url = createTelegramChatAction(BOTtoken, "typing", telegramGroupID);
    Serial.print(TELEGRAM_HOST);
    Serial.print(url);


    httpClientTelegramBot.begin(String(TELEGRAM_HOST)+String(url));
    httpClientTelegramBot.setTimeout(5000);

    httpClientTelegramBot.setUserAgent("ESP8266 watermotor meter");
    //Serial.print("[HTTP] GET...\n");
    int httpCode = httpClientTelegramBot.GET();

    // httpCode will be negative on error
    if(httpCode > 0)
    {
      //HTTP header has been send and Server response header has been handled
      //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      Serial.printf("[HTTP TELEGRAM] GET... code: [%d] %s\n", httpCode, httpClient.errorToString(httpCode).c_str());


      //file found at server
      //if(httpCode == HTTP_CODE_OK)
      {
        String payload = httpClientTelegramBot.getString();
        Serial.println(payload);
      }

    }
    else
    {
      Serial.printf("[HTTP TELEGRAM] GET... failed, error: [%d] %s\n", httpCode, httpClient.errorToString(httpCode).c_str());
      httpClientTelegramBot.end();
      return false;
    }

    return true;
  }
  return false;
}

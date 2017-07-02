
#include <Arduino.h>
#include "config.h"
#include "data.h"

#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>

//HTTPSRedirect wClientSecure(GScript_httpsPort);

#include "elapsedMillis.h"

elapsedMillis googleSpreadSheetUpdateTime;

int googleSpreadSheetUpdateTime_update_time = 60000; //600000; //10 minutes


const char * type_str[]
{
  "hb",
  "powerOn",
  "reConnected",
  "motorStats",
  "test"
};

//const char* host = "script.google.com";
//const char* googleRedirHost = "script.googleusercontent.com";
//const char *GScriptId = "AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ";

//const int httpsPort = 443;

// http://askubuntu.com/questions/156620/how-to-verify-the-ssl-fingerprint-by-command-line-wget-curl/
// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout
// www.grc.com doesn't seem to get the right fingerprint
// SHA1 fingerprint of the certificate
//const char* fingerprint = "94 2F 19 F7 A8 B4 5B 09 90 34 36 B2 2A C4 7F 17 06 AC 6A 2E";
const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";
const char* fingerprint2 = "94 64 D8 75 DE 5D 3A E6 3B A7 B6 15 52 72 CC 51 7A BA 2B BE";

//const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";

// On a Linux system with OpenSSL installed, get the SHA1 fingerprint for the destination and redirect hosts:
//   echo -n | openssl s_client -connect script.google.com:443 2>/dev/null | openssl x509  -noout -fingerprint | cut -f2 -d'='
//   echo -n | openssl s_client -connect script.googleusercontent.com:443 2>/dev/null | openssl x509  -noout -fingerprint | cut -f2 -d'='
char const * const dstFingerprint = "C7:4A:32:BC:A0:30:E6:A5:63:D1:8B:F4:2E:AC:19:89:81:20:96:BB";
char const * const redirFingerprint = "E6:88:19:5A:3B:53:09:43:DB:15:56:81:7C:43:30:6D:3E:9D:2F:DE";

HTTPSRedirect wClientSecure(GScript_httpsPort);

void googlespreadsheet_Init(char * status_str, float Irms, gScript_type type )
{

  while(!wClientSecure.connect(GScript_host, GScript_httpsPort))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println(" Connected to the internet.");

  String url = String("/macros/s/") + GScript_Id + "/exec?";
  String urlFinal = url
  + "type="+ type_str[type]
  + "&ellapsed_time=" + String(millis()/1000)
  + "&current_factor=" + String(Irms)
  + "&motor_status="+status_str;

  Serial.println(urlFinal);

  if(!wClientSecure.connected())
  {
    if(!wClientSecure.connect(GScript_host, GScript_httpsPort))
    {
      return;
    }
  }

  wClientSecure.printRedir(urlFinal, GScript_host, GScript_googleRedirHost);



  //String dstPath = "/macros/s/"+String(GScript_Id)+"/exec?status=OFF&currentFactor=3.4&currentFactorAverage=3.4";
  //char const * const dstPath = "/macros/s/google_random_path__replace_with_yours_see_documentation/exec";  // ** UPDATE ME **

  //
  //Send updates to g script for sheet
  //
  //

  // if(googleSpreadSheetUpdateTime > googleSpreadSheetUpdateTime_update_time)
  // {
  //   googleSpreadSheetUpdateTime = 0;
  //
  //   //HTTPSRedirect wClientSecure(GScript_httpsPort);
  //
  //   String state = "OFF";
  //   if(whetherMotorIsOn)
  //   {
  //     state = "ON";
  //   }
  //
  //   String url = String("/macros/s/") + GScript_Id + "/exec?";
  //String urlFinal = url + "status=" + state + "&currentFactor=" + String(Irms) + "&currentFactorAverage=" + String(IrmsAverage);

  // if(!wClientSecure.connected())
  // {
  //   if(!wClientSecure.connect(GScript_host, GScript_httpsPort))
  //   {
  //     return;
  //   }
  // }
  //
  // wClientSecure.printRedir(urlFinal, GScript_host, GScript_googleRedirHost);

  //}
}


void googlespreadsheet_Loop(char * status_str, float Irms, gScript_type type )
{

  if(googleSpreadSheetUpdateTime > googleSpreadSheetUpdateTime_update_time)
  {
    googleSpreadSheetUpdateTime = 0;
    while(!wClientSecure.connect(GScript_host, GScript_httpsPort))
    {
      Serial.print(".");
      delay(1000);
    }

    //Serial.println(" Connected to the internet.");

    String url = String("/macros/s/") + GScript_Id + "/exec?";
    String urlFinal = url
    + "type="+ type_str[type]
    + "&ellapsed_time=" + String(millis()/1000)
    + "&current_factor=" + String(Irms)
    + "&motor_status="+status_str;

    if(!wClientSecure.connected())
    {
      if(!wClientSecure.connect(GScript_host, GScript_httpsPort))
      {
        return;
      }
    }

    wClientSecure.printRedir(urlFinal, GScript_host, GScript_googleRedirHost);
  }

}

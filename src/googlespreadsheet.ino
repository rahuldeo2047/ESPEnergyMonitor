
#include <Arduino.h>
#include "config.h"
#include "data.h"
//#include "version.h"

#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>

//HTTPSRedirect wClientSecure(GScript_httpsPort);

#include "elapsedMillis.h"

elapsedMillis googleSpreadSheetMotorStatusUpdateTime;
elapsedMillis googleSpreadSheetHBUpdateTime;
elapsedMillis googleSpreadSheetHBKeepReadyUpdateTime;

//int googleSpreadSheetUpdateTime_motorstatus = 60000; //600000; //to be sent only on change not 10 minutes
int googleSpreadSheetUpdateTime_hb = 60000*10; // fixed number //600000; //10 minutes

int googleSpreadSheetUpdateTime_hb_keepready = googleSpreadSheetUpdateTime_hb-5000; //5 seconds before


const char * type_str[]
{
  "hb",
  "powerOn",
  "reConnected",
  "motorStats",
  "test"
};

const char * motor_sataus_str[]
{
  "ON",
  "OFF",
  "UNKNOWN"
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

void timesync()
{
  int rem = googleSpreadSheetUpdateTime_hb/1000;
  // time sync to millis() no work here it can be bypassed
  unsigned long one_interval = 0;//(millis()/1000)%rem;
  do {
    one_interval = ((millis())/1000)%rem;
    ESP.wdtFeed();

    //googlespreadsheet_keepready();

    //double Irms = -1.0;
    //bool state = curretSample_Loop(&Irms); // this might make it miss the %

    /* code */
  } while(one_interval != 0);

}

void googlespreadsheet_Init(gScript_motor_status status, float Irms, gScript_type type )
{

  while(!wClientSecure.connect(GScript_host, GScript_httpsPort))
  {
    Serial.print(".");

    delay(1000);

    // double Irms = -1.0;
    // bool state = curretSample_Loop(&Irms);
    // state = curretSample_Loop(&Irms);
    // state = curretSample_Loop(&Irms);
    // state = curretSample_Loop(&Irms);
    // state = curretSample_Loop(&Irms);
    // state = curretSample_Loop(&Irms);
    // state = curretSample_Loop(&Irms);

  }

  Serial.println(" Connected to the internet.");

  String url = String("/macros/s/") + GScript_Id + GSCRIPT_DEPLOY_TYPE; //"/exec?";
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

  if(!wClientSecure.connected())
  {
    if(!wClientSecure.connect(GScript_host, GScript_httpsPort))
    {
      return;
    }
  }

  wClientSecure.printRedir(urlFinal, GScript_host, GScript_googleRedirHost);

  //googleSpreadSheetHBKeepReadyUpdateTime = 40;
  //googleSpreadSheetHBUpdateTime = 45;
}

int googlespreadsheet_keepready()
{
  int ret_status = -1;

  if(!wClientSecure.connected())
  {
    Serial.print(millis());
    Serial.print(" : Trying to connect to gScript ...");
    do {
      ret_status = wClientSecure.connect(GScript_host, GScript_httpsPort);
      Serial.print(".");

      delay(500);
      // double Irms = -1.0;
      // bool state = curretSample_Loop(&Irms);
      // state = curretSample_Loop(&Irms);
      // state = curretSample_Loop(&Irms);
      // state = curretSample_Loop(&Irms);

    } while(!wClientSecure.connected());
    //Serial.println();

    Serial.print(" connected, sync with millis() ...");
    timesync();
    Serial.println(" Done");
  }
  else
  {
    timesync();
  }

  return wClientSecure.connected();

}

void googlespreadsheet_Loop(gScript_motor_status status, float Irms, gScript_type type, unsigned long dt )
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
    googlespreadsheet_keepready();
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

    googlespreadsheet_keepready();

    String url = String("/macros/s/") + GScript_Id + GSCRIPT_DEPLOY_TYPE ;//"/exec?";
    String urlFinal = url
    + "type="+ type_str[type]
    + "&ellapsed_time=" + String(dt)
    + "&current_factor=" + String(Irms)
    + "&motor_status="+motor_sataus_str[status];

    urlFinal += "&version="+String(_VER_);


    wClientSecure.printRedir(urlFinal, GScript_host, GScript_googleRedirHost);

  }

}

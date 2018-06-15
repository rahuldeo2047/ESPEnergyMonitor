


// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
//#include "version.h"
#include <ESP8266WiFi.h>

#include "RemoteDebug.h"
RemoteDebug Debug;

#include <MedianFilter.h>
MedianFilter samples_1(121, 30);
MedianFilter samples_2(121, 2000);

//#include <ESP8266TrueRandom.h>

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

#include "elapsedMillis.h"

const char * HOST_NAME="remotedebug-air_conditioner_energy";

elapsedSeconds checkThingSpeakTime, checkTelnetTime, checkPrintTime;

unsigned long last_time_thingspoke, last_time_telnet_talked ;
const int updateTelnetInterval = 1;// * 1000;

WiFiClient client;


bool whether_post_wifi_connect_setup_done;

void setup()
{
  Serial.begin(115200);
  WiFi.disconnect();
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  pinMode(LED_BUILTIN, OUTPUT);

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Wifi connection OK ");
  Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());


  whether_post_wifi_connect_setup_done = false;

  //Serial.printf("Version %s\n",_VER_);
  Serial.printf("Build at %s %s\n", __DATE__, __TIME__);
  Serial.print("MAC: "); Serial.println(WiFi.macAddress());

  double Irms = currentSample_Init();

  bool state ;

  state = curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);

}

double approxRollingAverage (double avg, double new_sample) {

  avg -= avg / MOVING_AVERAGE_COUNT;
  avg += new_sample / MOVING_AVERAGE_COUNT;

  return avg;
}

bool last_state = false;
void loop()
{

   double Irms,  Irms_filtered ;
   float  temp,  temp_filtered; // readTemp();

    if(false==whether_post_wifi_connect_setup_done)
    {
      if(WiFi.status() == WL_CONNECTED)
      {
        setup_OTA();
        rd_setup(HOST_NAME);
        whether_post_wifi_connect_setup_done = true;
      }
    }
    if(true==whether_post_wifi_connect_setup_done)
    {
      if(WiFi.status() == WL_CONNECTED)
      {
        if(loop_OTA())
        {
          return;
        }
        rd_loop();
      }
    }


    bool state = curretSample_Loop(&Irms);
    state = curretSample_Loop(&Irms);
    state = curretSample_Loop(&Irms);
    state = curretSample_Loop(&Irms);

    //temp = readTemp();
    //samples_2.in((int)((float)temp*1000.0));
    //temp_filtered = ((float)samples_2.out())*0.001;

    //delay(1);
    yield();
    samples_1.in((int)((float)Irms*1000.0));
    Irms_filtered = ((float)samples_1.out())*0.001;

    //if(checkPrintTime>0)
    {
      checkPrintTime = 0;
      Serial.printf("* Irms: %f (%f) A and Temp: %f (VERBOSE)\n", Irms, Irms_filtered, temp);
    }



if(true==whether_post_wifi_connect_setup_done)
{
 if(checkTelnetTime>=updateTelnetInterval)
 {
   checkTelnetTime = 0;
   //last_time_telnet_talked = millis();
   DEBUG_V("* Irms: %f (%f) A and Temp: %f (VERBOSE)\n", Irms, Irms_filtered, temp);
 }
}
 if(checkThingSpeakTime>updateThingSpeakInterval) // && samples.getCount() == samples.getSize())
 {
   checkThingSpeakTime = 0;
  //last_time_thingspoke = millis();
    Serial.println("data sending time");

    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(250);
    }

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("\nWifi connection OK ");
    Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());

    if (client.connect(server,80))
    {

      String tsData = apiWritekey;
      tsData +="&field1=";
      tsData += String(Irms_filtered); //
      tsData +="&field2=";
      tsData += String(temp_filtered); //
      tsData +="&field3=";
      tsData += String(millis()*0.001);
      tsData +="&field4=";
      tsData += String(Irms);
      tsData +="&field5=";
      tsData += String(temp); //
      tsData += "\r\n\r\n";

      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+String(apiWritekey)+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(tsData.length());
      client.print("\n\n");
      client.print(tsData);
      Serial.println("ThingSpeak data sent");
      delay(250);

    }
    else
    {

    }
    client.stop();
   }
   else
   {
     //WiFi.begin(ssid, password);
   }
 }




}

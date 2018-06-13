


// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
//#include "version.h"
#include <ESP8266WiFi.h>

#include "RemoteDebug.h"
RemoteDebug Debug;

//#include <ESP8266TrueRandom.h>

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

#include "elapsedMillis.h"

const char * HOST_NAME="remotedebug-air_conditioner_energy";

elapsedSeconds testMotorStatusChange;

unsigned long last_time_thingspoke, last_time_telnet_talked ;
const int updateTelnetInterval = 1 * 1000;

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

  whether_post_wifi_connect_setup_done = false;

  //Serial.printf("Version %s\n",_VER_);
  Serial.printf("Build at %s %s\n", __DATE__, __TIME__);
  Serial.print("MAC: "); Serial.println(WiFi.macAddress());

  double Irms = currentSample_Init();

  bool state ;

  state =  curretSample_Loop(&Irms);
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

   double Irms ;

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




 if(millis()-last_time_telnet_talked>updateTelnetInterval)
 {
   last_time_telnet_talked = millis();
   DEBUG_V("* Irms: %f A (VERBOSE)\n", Irms);
}

 if(millis()-last_time_thingspoke>updateThingSpeakInterval) // && samples.getCount() == samples.getSize())
 {

  last_time_thingspoke = millis();

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Wifi connection OK ");
    Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());

    if (client.connect(server,80))
    {

      String tsData = apiWritekey;
      tsData +="&field1=";
      tsData += String(Irms); // filtered time_us
      tsData +="&field2=";
      tsData += String(millis()*0.001);
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
      Serial.print("ThingSpeak data sent");

    }
    else
    {

    }
    client.stop();
   }
 }




}

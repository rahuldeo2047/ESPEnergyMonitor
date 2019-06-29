#include <FS.h>                   //this needs to be first, or it all crashes and burns...


// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
//#include "version.h"
#include <ESP8266WiFi.h>

#include "RemoteDebug.h"
RemoteDebug Debug;



#ifdef ESP8266
extern "C"
{
#include "user_interface.h"
}
#endif

#include "elapsedMillis.h"

const char *HOST_NAME = "remotedebug-air_conditioner_energy";

elapsedSeconds checkTelnetTime, checkPrintTime;
elapsedMillis checkThingSpeakTime;
unsigned long last_time_thingspoke, last_time_telnet_talked;
const int updateTelnetInterval = 1; // * 1000;

//WiFiClient client;

IPAddress local_IP(192, 168, 43, 152);
IPAddress gateway(192, 168, 43, 255);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8); // Google DNS

bool whether_post_wifi_connect_setup_done;

// //=======================================================================
void ICACHE_RAM_ATTR onTimerISR()
{
  //handleClients();
  timer1_write(5000); //12us??
}

void setup()
{
  Serial.begin(115200);

  
  //WiFi.disconnect();
  //delay(10);
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  //WiFi.config(local_IP, gateway, subnet, dns);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to configured wifi...");

  wifimanager_setup();

   
  mpu_setup();

  pinMode(LED_BUILTIN, OUTPUT);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   Serial.print(".");
  //   delay(1000);
  // }

  // Serial.print("Wifi connection OK ");
  // Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());

  whether_post_wifi_connect_setup_done = false;

  //setup_server();
  // handleClients();

  setup_php_server();

  //Serial.printf("Version %s\n",_VER_);
  Serial.printf("Build at %s %s\n", __DATE__, __TIME__);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  String ssid = "Device hotspot can be EEWD_" + String(WiFi.macAddress());
  ssid.replace(":", "");
  Serial.println(ssid);

  Irms_setup(); 

  // For complete sampling
      // Without this the first sample after this point is incomplete.
  Irms_resetSampleTimer();
  mpu_resetSampleTimer();

  // timer1_attachInterrupt(onTimerISR);
  // timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE); // TIM_LOOP
  // timer1_write(5000); // 1ms hope fully //120000 us

  //timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  //timer1_write(5000000);//1 second
}

 

bool last_state = false;
unsigned long sr, ts_acc;
void loop()
{

  unsigned long ts = millis(), dt_acc;


  double Irms = 0, Irms_filtered = 0;
  float temp = 0, temp_filtered = 0; // readTemp();
  float acc = 0, acc_filtered = 0;

  acc = 0;
  

  if (false == whether_post_wifi_connect_setup_done)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      setup_OTA();
      rd_setup(HOST_NAME);
      whether_post_wifi_connect_setup_done = true;
    }
  }
  if (true == whether_post_wifi_connect_setup_done)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      if (loop_OTA())
      {
        return;
      }
      rd_loop();
    }
  }

  ts = millis();
   
  mpu_loop();
 
  
  temp = mpu_getTemp(); 
  acc = mpu_getAccelFftMag(); 
  temp_filtered = mpu_getTempFiltered();
  acc_filtered = mpu_getAccelFftMagFiltered();
  

  bool state = Irms_loop(); // It is not measuring status

  Irms = Irms_getCurr();
 
  Irms_filtered = Irms_getFilteredCurr();
 

// Irms, Irms_filtered, temp, temp_filtered, acc, acc_filtered

  ts = millis() - ts;

  //if(checkPrintTime>0)
  //{
  //  checkPrintTime = 0;
  //  Serial.printf("* %d %d %f (%f) A, %f (%f) dC, %f (%f) (%d) G(VERBOSE)\n", millis(), ts, Irms, Irms_filtered, temp, temp_filtered, acc, acc_filtered, dt_acc);
  //}

  if (true == whether_post_wifi_connect_setup_done)
  {
    if (checkTelnetTime >= updateTelnetInterval)
    {
      checkTelnetTime = 0;
      //last_time_telnet_talked = millis();
      DEBUG_V("* %f (%f) A, %f (%f) dC, %f (%f) G(VERBOSE)\n", Irms, Irms_filtered, temp, temp_filtered, acc, acc_filtered);
    }
  }
  if (checkThingSpeakTime > updateThingSpeakInterval) // && samples.getCount() == samples.getSize())
  {
    checkThingSpeakTime = 0;
    //last_time_thingspoke = millis();
    Serial.println("data sending time");

    long time_wifi_check = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(250);
       
      if (millis()- time_wifi_check> 60000)
      {
        Serial.println("Resetting the device as not connecting to configured wifi settings...");
        delay(2000);
        Serial.println("Repeat: Resetting the device as not connecting to configured wifi settings...");
        delay(2000);
        Serial.println("Repeat: Resetting the device as not connecting to configured wifi settings...");
        delay(2000);
        ESP.reset();
      }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      sr++;
      loop_php_server(sr, millis(), temp_filtered, temp, Irms_filtered, Irms, acc_filtered, acc);

      Serial.print("\nWifi connection OK ");
      Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());

      // For complete sampling
      // Without this the first sample after this point is incomplete.
      Irms_resetSampleTimer();
      mpu_resetSampleTimer();
      
      return;

      // Working example
      // if (client.connect(server, 80))
      // {

      //   String tsData = apiWritekey;
      //   tsData += "&field1=";
      //   tsData += String(Irms_filtered); //
      //   tsData += "&field2=";
      //   tsData += String(temp_filtered); //
      //   tsData += "&field3=";
      //   tsData += String(acc_filtered);
      //   tsData += "&field4=";
      //   tsData += String(millis() * 0.001);
      //   tsData += "&field5=";
      //   tsData += String(Irms);
      //   tsData += "&field6=";
      //   tsData += String(temp); //
      //   tsData += "&field7=";
      //   tsData += String(acc); //
      //   tsData += "\r\n\r\n";

      //   client.print("POST /update HTTP/1.1\n");
      //   client.print("Host: api.thingspeak.com\n");
      //   client.print("Connection: close\n");
      //   client.print("X-THINGSPEAKAPIKEY: " + String(apiWritekey) + "\n");
      //   client.print("Content-Type: application/x-www-form-urlencoded\n");
      //   client.print("Content-Length: ");
      //   client.print(tsData.length());
      //   client.print("\n\n");
      //   client.print(tsData);
      //   Serial.println("ThingSpeak data sent");
      //   delay(250);
      // }
      // else
      // {
      // }
      //client.stop();
    }
    else
    {
      //WiFi.begin(ssid, password);
    }
  }
}

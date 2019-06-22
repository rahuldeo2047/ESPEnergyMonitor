

// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
//#include "version.h"
#include <ESP8266WiFi.h>

#include "RemoteDebug.h"
RemoteDebug Debug;

#include <MedianFilter.h>
MedianFilter samples_1(121, 30);
MedianFilter samples_2(21, 10000); // devide by 10000 as targetting 1.000
MedianFilter samples_3(21, 3500);  // devide by 100 as targetting 35.0

//#include <ESP8266TrueRandom.h>

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

WiFiClient client;

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
  WiFi.disconnect();
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(local_IP, gateway, subnet, dns);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  setup_mpu();

  pinMode(LED_BUILTIN, OUTPUT);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Wifi connection OK ");
  Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());

  whether_post_wifi_connect_setup_done = false;

  //setup_server();
  // handleClients();

  setup_php_server();

  //Serial.printf("Version %s\n",_VER_);
  Serial.printf("Build at %s %s\n", __DATE__, __TIME__);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  double Irms = currentSample_Init();

  bool state;

  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);

  // timer1_attachInterrupt(onTimerISR);
  // timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE); // TIM_LOOP
  // timer1_write(5000); // 1ms hope fully //120000 us

  //timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  //timer1_write(5000000);//1 second
}

double approxRollingAverage(double avg, double new_sample)
{

  avg -= avg / MOVING_AVERAGE_COUNT;
  avg += new_sample / MOVING_AVERAGE_COUNT;

  return avg;
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

  //ts = millis();

  bool state = curretSample_Loop(&Irms);
  //handleClients();
  state = curretSample_Loop(&Irms);
  //handleClients();
  state = curretSample_Loop(&Irms);
  //handleClients();
  state = curretSample_Loop(&Irms);
  //handleClients();
  //ts = millis()-ts;
  bool status = loop_mpu();

  if (status = true)
  {
    temp = getTemp();
    dt_acc = millis() - ts_acc;
    acc = getAccMag();
    ts_acc = millis();
  }

  yield();
  samples_2.in((int)((float)acc * 10000.0));
  acc_filtered = ((float)samples_2.out()) * 0.0001;

  samples_3.in((int)((float)temp * 100.0));
  temp_filtered = ((float)samples_3.out()) * 0.01;

  //temp = readTemp();
  //samples_2.in((int)((float)temp*1000.0));
  //temp_filtered = ((float)samples_2.out())*0.001;

  //delay(1);
  yield();
  samples_1.in((int)((float)Irms * 1000.0));
  Irms_filtered = ((float)samples_1.out()) * 0.001;

  //loop_server(sr, millis(), temp_filtered, temp, Irms_filtered, Irms, acc_filtered, acc);
  //loop_php_server(sr, millis(), temp_filtered, temp, Irms_filtered, Irms, acc_filtered, acc);

  //ts = micros64();

  //handleClients();

  ts = millis() - ts;

  //if(checkPrintTime>0)
  {
    checkPrintTime = 0;
    Serial.printf("* %d %d %f (%f) A, %f (%f) dC, %f (%f) (%d) G(VERBOSE)\n", millis(), ts, Irms, Irms_filtered, temp, temp_filtered, acc, acc_filtered, dt_acc);
  }

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

    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(250);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      sr++;
      loop_php_server(sr, millis(), temp_filtered, temp, Irms_filtered, Irms, acc_filtered, acc);

      Serial.print("\nWifi connection OK ");
      Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());
      return;

      if (client.connect(server, 80))
      {

        String tsData = apiWritekey;
        tsData += "&field1=";
        tsData += String(Irms_filtered); //
        tsData += "&field2=";
        tsData += String(temp_filtered); //
        tsData += "&field3=";
        tsData += String(acc_filtered);
        tsData += "&field4=";
        tsData += String(millis() * 0.001);
        tsData += "&field5=";
        tsData += String(Irms);
        tsData += "&field6=";
        tsData += String(temp); //
        tsData += "&field7=";
        tsData += String(acc); //
        tsData += "\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + String(apiWritekey) + "\n");
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

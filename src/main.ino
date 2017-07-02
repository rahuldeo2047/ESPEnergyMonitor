


// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
#include "version.h"
#include "data.h"
#include <ESP8266WiFi.h>

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

#include "elapsedMillis.h"

elapsedSeconds wifiDisconnectionTime;

elapsedSeconds motorStatusChange;

bool isWifiConnected = false;

unsigned long lastDisconnectionTs = 0;
unsigned long lastMotorStatusChangeTs = 0;

void wifi_setup()
{

  //
  //Connect to WiFi
  //
  //

  Serial.printf("Connecting to %s ", WIFISSID);
  WiFi.begin(WIFISSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  isWifiConnected = true;

  Serial.print(" connected to wifi.");//\nChecking internet connectivity ..");

}

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Ready");
  delay(1000);

  double Irms = currentSample_Init();

  wifi_setup();

  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  bool state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  
  gScript_motor_status status_gScript = UNKNOWN;

  if(state)
  {
    status_gScript = ON;
  }
  else
  {
    status_gScript = OFF;
  }

  googlespreadsheet_Init(status_gScript, Irms, powerOn);


  Serial.print(" connected, sync with millis()...");
  timesync();
  Serial.println("Done");

}

double approxRollingAverage (double avg, double new_sample) {

  avg -= avg / MOVING_AVERAGE_COUNT;
  avg += new_sample / MOVING_AVERAGE_COUNT;

  return avg;
}

bool last_state = false;
void loop()
{
  //return;

  // Only valid if we are sending data within 10 seconds
  if (WiFi.status() != WL_CONNECTED)
  {

    if(isWifiConnected)// && (WiFi.status() != WL_CONNECTED))
    {

      lastDisconnectionTs = millis();
      Serial.print(millis());
      Serial.print(" : ");
      Serial.println("WiFi connectivity failed");


      isWifiConnected = false;

      wifiDisconnectionTime = 0;
    }
    Serial.print(".");
    //discCnt++;
    delay(500);
    return;
  }


  //
  // All power state calculation
  //
  //

  double Irms = -1.0;
  bool state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  gScript_motor_status status_gScript = UNKNOWN;

  if(state)
  {
    status_gScript = ON;
    lastMotorStatusChangeTs = millis();
  }
  else
  {
    status_gScript = OFF;
    lastMotorStatusChangeTs = millis();
  }

  //////
  //////
  //////
  //////

  if(!isWifiConnected)
  {
    Serial.println();

    googlespreadsheet_keepready();

    isWifiConnected = true; // set it true as googlespreadsheet_keepready() will make it online

    googlespreadsheet_Loop(status_gScript, Irms, reConnected, millis()-lastDisconnectionTs );
  }

  // Motor status
  if(last_state != state)
  {
    if(motorStatusChange > 10000)
    {
      motorStatusChange = 0;
      Serial.println("State changes");
      state = curretSample_Loop(&Irms);
      googlespreadsheet_Loop(status_gScript, Irms, motorStats, millis()-lastMotorStatusChangeTs);
      last_state = state;
    }
  }

  // hb

  googlespreadsheet_Loop(status_gScript, Irms, hb, millis());


}




// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
#include "version.h"
#include "data.h"
#include <ESP8266WiFi.h>

#include <ESP8266TrueRandom.h>

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

#include "elapsedMillis.h"


elapsedSeconds testMotorStatusChange;
unsigned long testTime = 0;

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

gScript_motor_status motor_status_gScript = UNKNOWN;
gScript_motor_status last_motor_status_gScript = UNKNOWN;

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


  bool state ;

  #if !defined(TEST_MODE)

  //=  -1;//
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);
  state =  curretSample_Loop(&Irms);

  #else
  //  if(testMotorStatusChange > testTime)
  {
    testMotorStatusChange = 0;
    testTime = 0;//ESP8266TrueRandom.random(11,19);

    bool status;
    //gScript_type = hb;
    Irms = ESP8266TrueRandom.random(10,600)/100.0;

    if(Irms>current_factor_threshold_for_motor_ON)
    {
      status = true ; //signifies motor is ON
    }

    if(Irms<current_factor_threshold_for_motor_ON)
    {

      status = false ; //signifies motor is OFF
    }

    state = status;

    //state = (bool)ESP8266TrueRandom.randomBit();


    Serial.print("Random test ts:");
    Serial.print(testTime+millis());
    Serial.print(" test Irms: ");
    Serial.println(Irms);
  }

  #endif // !defined(TEST_MODE)



  motor_status_gScript = UNKNOWN;

  if(state)
  {
    motor_status_gScript = ON;
  }
  else
  {
    motor_status_gScript = OFF;
  }

  //googlespreadsheet_Init(motor_status_gScript, Irms, powerOn);
  phpgsheet_Init(motor_status_gScript, Irms, powerOn);
  last_motor_status_gScript = motor_status_gScript;


  Serial.print(" connected, sync with millis()...");
  timesync();
  Serial.println("Done");

  motor_status_gScript = UNKNOWN;
  last_motor_status_gScript = UNKNOWN;

//while(1)
//ESP.wdtFeed();
}

double approxRollingAverage (double avg, double new_sample) {

  avg -= avg / MOVING_AVERAGE_COUNT;
  avg += new_sample / MOVING_AVERAGE_COUNT;

  return avg;
}

bool last_state = false;
void loop()
{
  gScript_type gScript_type = unknown;
  //gScript_motor_status motor_status_gScript = UNKNOWN;
  unsigned long ts = 0;
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

      //wifiDisconnectionTime = 0;
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

  // these are static only for test purpose

  #if !defined(TEST_MODE)

  double Irms = -1.0;
  bool state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);
  state = curretSample_Loop(&Irms);

  #else

  static double Irms = -1.0;
  static bool state = 0;

  if(
    (testMotorStatusChange > testTime)
    ||
    ( UNKNOWN == motor_status_gScript )
  )
  {
    testMotorStatusChange = 0;
    testTime = ESP8266TrueRandom.random(11,60*60*2);

    bool status;
    //gScript_type = hb;
    Irms = ESP8266TrueRandom.random(10,600)/100.0;

    if(Irms>current_factor_threshold_for_motor_ON)
    {
      status = true ; //signifies motor is ON
    }

    if(Irms<current_factor_threshold_for_motor_ON)
    {

      status = false ; //signifies motor is OFF
    }

    state = status;

    //state = (bool)ESP8266TrueRandom.randomBit();


    Serial.print("Random test ts:");
    Serial.print(testTime*1000+millis());
    Serial.print(" test Irms: ");
    Serial.println(Irms);
  }

  #endif // !defined(TEST_MODE)

  if(!isWifiConnected)
  {
    Serial.println();

    //googlespreadsheet_keepready();

    isWifiConnected = true; // set it true as googlespreadsheet_keepready() will make it online

    if(gScript_type == unknown)
    {
      gScript_type = reConnected;
      ts = millis()-lastDisconnectionTs;
      //googlespreadsheet_Loop(motor_status_gScript, Irms, gScript_type, ts);
      phpgsheet_Loop(motor_status_gScript, Irms, gScript_type, ts);
    }

    //googlespreadsheet_Loop(motor_status_gScript, Irms, gScript_type, ts );
  }

  // Motor status

  if( UNKNOWN == motor_status_gScript )
  {

    lastMotorStatusChangeTs = millis();

  }

  motor_status_gScript = state==true ? ON : OFF;

  if(last_motor_status_gScript != motor_status_gScript)
  {
    //if(motorStatusChange > 10000)
    //{
    //motorStatusChange = 0;
    //if(gScript_type == unknown)

    last_motor_status_gScript = motor_status_gScript;

    //Serial.println("State changes");

    if( (gScript_type == unknown) || (gScript_type == reConnected) )
    {
      gScript_type = motorStats;
      ts = millis()-lastMotorStatusChangeTs;

      Serial.print("Motor ts:");
      Serial.println(ts);

      lastMotorStatusChangeTs = millis();
      //googlespreadsheet_Loop(motor_status_gScript, Irms, gScript_type, ts);
      phpgsheet_Loop(motor_status_gScript, Irms, gScript_type, ts);
    }

    //}
  }

  // hb
  if( (gScript_type == unknown) || (gScript_type == motorStats) )
  {
    gScript_type = hb;
    ts = millis() ;

    //googlespreadsheet_Loop(motor_status_gScript, Irms, gScript_type, ts);
    phpgsheet_Loop(motor_status_gScript, Irms, gScript_type, ts);
  }



}

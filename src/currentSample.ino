
#include <Arduino.h>
#include "config.h"
#include "version.h"


#include "elapsedMillis.h"

elapsedMillis analogSampleUpdateTime;

#include "EmonLib.h"

EnergyMonitor emon1;                   // Create an instances

const float current_factor_threshold_for_motor_ON = (1.0);


float currentSample_Init()
{

  emon1.current(A0, 60.6);// 111.1);             // Current: input pin, calibration.

  emon1.calcIrms(1480); // stabilize the reading
  emon1.calcIrms(1480);
  emon1.calcIrms(1480);
  emon1.calcIrms(1480);
  return emon1.calcIrms(1480);

}

boolean curretSample_Loop(double * Irms)
{
  //
  //Read analog data as per
  // https://learn.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino
  //

  bool status = false;

  if(analogSampleUpdateTime > 20)
  {
    analogSampleUpdateTime = 0;

    *Irms = emon1.calcIrms(1480);  // Calculate Irms only

    // if(testMotorStatusChange > testTime)
    // {
    //   testMotorStatusChange = 0;
    //   testTime = ESP8266TrueRandom.random(10000,20000);
    //
    //   //gScript_type = hb;
    //   *Irms = ESP8266TrueRandom.random(10,600)/100.0;
    //
    //
    //   Serial.print("Random test ts:");
    //   Serial.print(testTime);
    //   Serial.print(" test Irms: ");
    //   Serial.println(*Irms);
    // }

    if(*Irms>current_factor_threshold_for_motor_ON)  // probably ignore accounting anything below 200mA
    {

      if(*Irms>current_factor_threshold_for_motor_ON)
      {
        status = true ; //signifies motor is ON
      }

    }

    if(*Irms<current_factor_threshold_for_motor_ON)
    {

      status = false ; //signifies motor is OFF
    }

    Serial.print(millis());
    Serial.print(" : ");
    Serial.println(*Irms);
  }

  return status;

}

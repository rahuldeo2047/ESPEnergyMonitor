


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
  if(state)
  {
    googlespreadsheet_Init("ON", Irms, powerOn);
  }
  else
  {
    googlespreadsheet_Init("OFF", Irms, powerOn);
  }

}

double approxRollingAverage (double avg, double new_sample) {

  avg -= avg / MOVING_AVERAGE_COUNT;
  avg += new_sample / MOVING_AVERAGE_COUNT;

  return avg;
}

void loop()
{
  //return;
  //
  //Read analog data as per
  // https://learn.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino
  //
  //double Irms = 0.0;
  //bool status = curretSample_Loop(double * Irms)


  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Internet connectivity failed");

    //hasSentLast = false;

    //lastDisconnectionTs = millis();
    //discCnt++;
    delay(5000);
    return;
  }

  double Irms = -1.0;
  bool state =  curretSample_Loop(&Irms);
  if(state)
  {
    googlespreadsheet_Loop("ON", Irms, motorStats);
  }
  else
  {
    googlespreadsheet_Loop("OFF", Irms, motorStats);
  }

}


// // This is the main method where data gets pushed to the Google sheet
// void postData(String state, float Irms){
//     if (!client.connected()){
//             Serial.println(“Connecting to client again…”);
//             client.connect(host, httpsPort);
//     }
//     String urlFinal = url + “tag=” + tag + “&value=” + String(value);
//     client.printRedir(urlFinal, GScript_host, GScript_googleRedirHost);
// }

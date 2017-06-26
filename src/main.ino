


// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
#include "version.h"



#include "elapsedMillis.h"

elapsedMillis telegramBotMsgUpdateTime;
elapsedMillis telegramBotHeartbeatTime;
elapsedMillis analogSampleUpdateTime;
elapsedMillis googleSpreadSheetUpdateTime;

#include "EmonLib.h"                   // Include Emon Library
#include "LinkedList.h"

EnergyMonitor emon1;                   // Create an instances


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//const char * host = "google.co.in";
//const int httpPort = 80;
//WiFiClient wClient;
unsigned long lastDisconnectionTs = 0;
uint8_t lastWifiConnectivityStatus = 0;


#include <ESP8266TelegramBOT.h>

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 5000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

int Bot_hb = (3600000-5000); //mean time between scan messages
long Bot_hb_lasttime;   //last time messages' scan has been done


#include "HTTPSRedirect.h"
#include <WiFiClientSecure.h>

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

#if !defined(ESP_WATCHDOG_FOR_WIFICLIENTSECURE)

// ~/.platformio/packages/framework-arduinoespressif8266/libraries
// /ESP8266WiFi/src/WiFiClientSecure.cpp
//
// #ifdef ESP8266
// extern "C" {
//  #include "user_interface.h"
//  #define ESP_WATCHDOG_FOR_WIFICLIENTSECURE
// }
// #endif
// #error need watchdog feed in


#endif
HTTPSRedirect wClientSecure(GScript_httpsPort);

int googleSpreadSheetUpdateTime_update_time = 10000; //600000; //10 minutes

const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";

bool hasSentLast = false;

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

  Serial.print(" connected to wifi.\nChecking internet connectivity ..");


  //
  //Check net connectivity a dirty way
  //
  //

  bot.begin();      // launch Bot functionalities

  while(!bot.sendMessage(telegramGroupID, "Hi I am Powered ON and connected to the internet Just NOW ( "
  +String(millis()/1000.0) +" seconds ) ", "")) // Test
  {
    delay(5000);
    Serial.print(".");
    //
  }
  hasSentLast = true;
  Serial.println(" checked all fine.\nChecking GScript connectivity ..");

  //
  //Check google scipt connection
  //
  //

  // Use HTTPSRedirect class to create TLS connection
  HTTPSRedirect wClientSecure(GScript_httpsPort);
  Serial.print("Connecting to ");
  Serial.println(GScript_host);

  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = wClientSecure.connect(GScript_host, GScript_httpsPort);
    if (retval == 1) {
       flag = true;
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }

  Serial.flush();
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(GScript_host);
    Serial.println("Exiting...");
    return;
  }

  Serial.flush();
  if (wClientSecure.verify(fingerprint, GScript_host)) {
    Serial.println("Certificate match.");
  } else {
    Serial.println("Certificate mis-match");
  }

  // int retval = 0 ;//wClientSecure.connect(GScript_host, GScript_httpsPort);
  //
  // while(!retval)
  // {
  //   Serial.println(__LINE__);
  //   retval = wClientSecure.connect(GScript_host, GScript_httpsPort);
  //   Serial.println(__LINE__);
  //
  //   elapsedMillis nonblockedEllapsecheck;
  //
  //   while(nonblockedEllapsecheck < 1000)
  //   {
  //     ESP.wdtFeed();
  //     //nonblockedEllapsecheck = 0;
  //   }
  //   Serial.println(__LINE__);
  //
  //   //delay(5000);
  //   Serial.print(".");
  // }

  Serial.println(" checked all fine.");



  // Data will still be pushed even certification don’t match.
  if (wClientSecure.verify(fingerprint, GScript_host)) {
    Serial.println("Certificate match.");
  } else {
    Serial.println("Certificate mis-match");
  }

}



/********************************************
* EchoMessages - function to Echo messages *
********************************************/
bool autoStatus = true;
bool whetherMotorIsOn = false;
bool whetherMotorIsOnLast = true;
bool Started = false;
String teleBotMsgID = telegramGroupID ;// group id //"55129840"; // my telegram
//lastWifiConnectivityStatus = false;
unsigned long lastMotorStartedTimeStamp = 0;
void Bot_EchoMessages(bool status, double data) {
  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)      {
    bot.message[i][5]=bot.message[i][5].substring(1,bot.message[i][5].length());

    Serial.print("BOT>MSG:");
    Serial.println(bot.message[i][5]);

    if (bot.message[i][5] == "autowatermotorstatus") {

      Serial.println("BOT : CMD rec : autowatermotorstatus");
      //digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      autoStatus = true==autoStatus?false:true;

      if(true == autoStatus)
      {
        bot.sendMessage(bot.message[i][4], "On change status will be sent automatically", "");
      }
      else
      {
        bot.sendMessage(bot.message[i][4], "On change status has been disabled", "");
      }
    }


    if (bot.message[i][5] == "watermotorstatus") {
      Serial.println("BOT : CMD rec : watermotorstatus");
      //digitalWrite(13, LOW);    // turn the LED off (LOW is the voltage level)
      if(true == status)
      {
        bot.sendMessage(bot.message[i][4], String("Water meter is ON with current factor "+String(data)+"mA"), "");
      }
      else
      {
        bot.sendMessage(bot.message[i][4], String("Water meter is OFF with current factor "+String(data)+"mA"), "");
      }
    }


    if (bot.message[i][5] == "uptime") {
      Serial.println("BOT : CMD rec : uptime");
      //float ts = millis();
      String duration  = timeFormat(millis()); //= timeScale(&ts);

      bot.sendMessage(bot.message[i][4], String("uptime : \u2705 "+duration), "");

    }


    if (bot.message[i][5] == "version") {
      Serial.println("BOT : CMD rec : version");

      bot.sendMessage(bot.message[i][4], (_VER_), "");

    }


    if (bot.message[i][5] == "start") {
      Serial.println("BOT : Started");
      String wellcome = "Wellcome from Water-Electric-Motor, your personal Bot";
      String wellcome1 = "/watermotorstatus : to get motor runnning status";
      String wellcome2 = "/autowatermotorstatus : to  get motor runnning status automatically";
      bot.sendMessage(bot.message[i][4], wellcome, "");
      bot.sendMessage(bot.message[i][4], wellcome1, "");
      bot.sendMessage(bot.message[i][4], wellcome2, "");
      Started = true;
      teleBotMsgID = bot.message[i][4];
    }

    teleBotMsgID = bot.message[i][4];

  }
  bot.message[0][0] = "";   // All messages have been replied - reset new messages
}


String timeFormat(unsigned long ts)
{
  unsigned long now = ts/1000.0;
  int seconds = now%60;
  int minutes = (now/60)%60;
  int hours   = (now/3600)%24;
  int days    = (now/(3600*24)); // Not changing to month further
  return String(
    String(days) + " d, "
    + String(hours) + " h, "
    + String(minutes) + " m, "
    + String(seconds) + " s"
  );
}

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Ready");
  delay(1000);


  wifi_setup();

  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  bot.begin();      // launch Bot functionalities

  emon1.current(A0, 60.6);// 111.1);             // Current: input pin, calibration.

  emon1.calcIrms(1480);
  emon1.calcIrms(1480);
  emon1.calcIrms(1480);

  //user
  //bot.sendMessage("55129840", "Hi I am Powered ON and connected to the internet Just NOW.", ""); // Test

  //group
  //bot.sendMessage("-237644374", "Hi I am Powered ON and connected to the internet Just NOW.", ""); // Test
  // emon1.calcIrms(1480);
  // emon1.calcIrms(1480);
  // emon1.calcIrms(1480);
  // emon1.calcIrms(1480);
  // Remove few initial fuctuations

}

double approxRollingAverage (double avg, double new_sample) {

    avg -= avg / MOVING_AVERAGE_COUNT;
    avg += new_sample / MOVING_AVERAGE_COUNT;

    return avg;
}

void loop()
{

  //
  //Read analog data as per
  // https://learn.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino
  //

  const  double alpha = 0.98;
  double Irms = 0;
  double power = 0;
  static double IrmsAverage = 0;

  if(analogSampleUpdateTime > 20)
  {
    analogSampleUpdateTime = 0;
    Irms = emon1.calcIrms(1480 );  // Calculate Irms only

    IrmsAverage = approxRollingAverage(IrmsAverage, Irms);

    power = Irms*230.0;

    if(Irms>0.20)  // probably ignore accounting anything below 200mA
    {

      if(Irms>2.0)
      {
        whetherMotorIsOn = true;
      }

    }

    if(Irms<0.20)
    {
      whetherMotorIsOn = false;
    }

    Serial.println(Irms);
  }

  //
  //Get telegram updates
  // https://core.telegram.org/bots/api
  //

  if(telegramBotMsgUpdateTime > Bot_mtbs)
  {
    static int discCnt = 0;
    telegramBotMsgUpdateTime = 0;

    //
    // Keep on checking is WiFi is connected
    //
    //

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Internet connectivity failed");

      hasSentLast = false;

      lastDisconnectionTs = millis();
      discCnt++;
      delay(5000);
      return;
    }

    //
    //If WiFi is connected check for internet
    //
    //
    if(!hasSentLast)
    {

      hasSentLast = bot.sendMessage(telegramDebugID
        , String( "Hi I am connected back to the internet after ( "
        + String( (discCnt*5000.0)/1000.0 )
        + " plus ) "
        + String( (millis() - lastDisconnectionTs ) /1000.0)
        +" seconds ago."), ""); // Test

        if(!hasSentLast)
        {
          delay(5000);
          return;
        }

      }


      bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
      Bot_EchoMessages(whetherMotorIsOn, Irms);   // reply to message with Echo

      //
      //Check if autoStatus sending is 'ON' or /start msg has been sent here
      //
      //

      if(autoStatus || Started)
      {
        //
        //If motor running status changed send msg automatically by default
        //
        //

        if(whetherMotorIsOn != whetherMotorIsOnLast)
        {
          whetherMotorIsOnLast = whetherMotorIsOn;
          String duration = " ";
          String str = " ";
          float timediff =  millis()-lastMotorStartedTimeStamp ;
          lastMotorStartedTimeStamp = millis();
          duration = String(timediff);
          duration += " ms ";
          bool lastTimeMultiplierChecked = true;


          if(whetherMotorIsOn)
          {
            str = "Water motor is ON now. It was 'off' for ";
          }
          else
          {
            str = "Water motor is OFF now. It was 'on' (or another state) for ";
          }

          duration = timeFormat(timediff); //timeScale(&timediff);

          Serial.print("timediff ms : ");
          Serial.print(timediff);


          if(whetherMotorIsOn)
          {
            bot.sendMessage(teleBotMsgID, String(str+duration), "");
          }
          else
          {
            bot.sendMessage(teleBotMsgID, String(str+duration), "");
          }

        }

        //
        //SEND HB MSG TO a debug telegram user
        //
        //

        if(telegramBotHeartbeatTime > Bot_hb) {
          telegramBotHeartbeatTime = 0;
          //if (millis() > Bot_hb_lasttime + Bot_hb)  {
          //float ts = millis();
          String duration = timeFormat(millis()); // timeScale(&ts);

          bot.sendMessage(telegramDebugID, String("hb \u2764\ufe0f : "+ duration +" "+Irms+" mA."), "");

          Bot_hb_lasttime = millis();
        }


        //
        //Send updates to g script for sheet
        //
        //

        if(googleSpreadSheetUpdateTime > googleSpreadSheetUpdateTime_update_time)
        {
          googleSpreadSheetUpdateTime = 0;

          //HTTPSRedirect wClientSecure(GScript_httpsPort);

          String state = "OFF";
          if(whetherMotorIsOn)
          {
            state = "ON";
          }

          String url = String("/macros/s/") + GScript_Id + "/exec?";
          String urlFinal = url + "status=" + state + "&currentFactor=" + String(Irms) + "&currentFactorAverage=" + String(IrmsAverage);

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

      Bot_lasttime = millis();
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

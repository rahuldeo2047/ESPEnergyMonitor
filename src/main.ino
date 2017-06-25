


// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>

#include "EmonLib.h"                   // Include Emon Library
#include "LinkedList.h"
#include "config.h"

//#if !defined(__AVR_ATmega2560__)

//#error jhhgkj
//#


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ESP8266TelegramBOT.h>

#define BOTtoken "444154317:AAFuKAx319tadCnWxHv9hdA0MiDbPLHxoj8"  //token of TestBOT
#define BOTname "Water-Electric-Motor"
#define BOTusername "WaterElectricMotorBot"

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 5000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done


EnergyMonitor emon1;                   // Create an instances


//unsigned long
double totalPower = 0;

const char * host = "google.co.in";
const int httpPort = 80;
WiFiClient wClient;
unsigned long lastDisconnectionTs = 0;
uint8_t lastWifiConnectivityStatus = 0;

bool hasSentLast = false;
//WiFiUDP Udp;

//unsigned int localUdpPort = 10000;
//char incomingPacket[255] = {0};

void wifi_setup()
{

  Serial.printf("Connecting to %s ", WIFISSID);
  WiFi.begin(WIFISSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print(" connected to wifi.\nChecking internet connectivity ..");


  //wClient.connect(host, httpPort);

  while(!bot.sendMessage("-237644374", "Hi I am Powered ON and connected to the internet Just NOW ( "
  +String(millis()/1000.0) +" seconds ) ", "")) // Test
  {
    delay(5000);
    Serial.print(".");
    //
  }
  //wClient.stop();
  hasSentLast = true;
  Serial.println(" checked all fine.");

  //Udp.begin(localUdpPort);
  //Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

}



/********************************************
* EchoMessages - function to Echo messages *
********************************************/
bool autoStatus = true;
bool whetherMotorIsOn = false;
bool whetherMotorIsOnLast = true;
bool Started = false;
String teleBotMsgID = "-237644374" ;// group id //"55129840"; // my telegram
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
      float ts = millis();
      String duration = timeScale(&ts);

      bot.sendMessage(bot.message[i][4], String("uptime : "+duration), "");
      //digitalWrite(13, LOW);    // turn the LED off (LOW is the voltage level)
      // if(true == status)
      // {
      //   bot.sendMessage(bot.message[i][4], "Water meter is ON", "");
      // }
      // else
      // {
      //   bot.sendMessage(bot.message[i][4], "Water meter is OFF", "");
      // }
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


String timeScale(float *timediff)
{
  //float timediff = *_timediff;
  String durationStr = "";
  bool lastTimeMultiplierChecked = true;

  if(*timediff >= 1000 && lastTimeMultiplierChecked)
  {

    *timediff /= 1000; // seconds
    durationStr = String(*timediff);
    durationStr += " s ";

    //Serial.print(" timediff seconds: ");
    //Serial.print(timediff);
  }
  else
  {
    lastTimeMultiplierChecked = false;
  }

  if(*timediff >= 60 && lastTimeMultiplierChecked)
  {
    *timediff /= 60; // minutes
    durationStr = String(*timediff);
    durationStr += " m ";

    //Serial.print(" timediff minutes: ");
    //Serial.print(timediff);

  }
  else
  {
    lastTimeMultiplierChecked = false;
  }



  if(*timediff >= 60 && lastTimeMultiplierChecked)
  {
    *timediff /= 60; // hours
    durationStr = String(*timediff);
    durationStr += " h ";

    // Serial.print(" timediff hours: ");
    // Serial.print(timediff);
  }
  else
  {
    lastTimeMultiplierChecked = false;
  }


  if(*timediff >= 24 && lastTimeMultiplierChecked)
  {
    *timediff /= 24; // days
    durationStr = String(*timediff);
    durationStr += " d ";

    // Serial.print(" timediff days: ");
    // Serial.print(timediff);
  }
  else
  {
    lastTimeMultiplierChecked = false; // :P
  }

  return durationStr;


}

void _Bot_EchoMessages() {
  Serial.println("MSG :");
  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)      {

    Serial.print(bot.message[i][0]);
    Serial.print(" > ");
    Serial.print(bot.message[i][1]);
    Serial.print(" > ");
    Serial.print(bot.message[i][2]);
    Serial.print(" > ");
    Serial.print(bot.message[i][3]);
    Serial.print(" > ");
    Serial.print(bot.message[i][4]);
    Serial.print(" > ");
    Serial.println(bot.message[i][5]);

    bot.sendMessage(bot.message[i][4], bot.message[i][5], "");
  }
  bot.message[0][0] = "";   // All messages have been replied - reset new messages
}


//#endif // !defined(__AVR_ATmega2560__)


void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Ready");
  delay(1000);

  //#if !defined(__AVR_ATmega2560__)

  wifi_setup();

  //Serial.println("");
  //Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  bot.begin();      // launch Bot functionalities

  //#endif // !defined(__AVR_ATmega2560__)

  emon1.current(A0, 60.6);// 111.1);             // Current: input pin, calibration.

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


float j = 0;

void loop()
{


  static unsigned long lastTime = 0;

  const  double alpha = 0.98;
  static double totalDeltaTime = 0;
  static double totalTime = 0;
  static double IrmskWh = 0;
  double Irms = 0;
  double power = 0;
  double deltaTime = 0;
  static double IrmsAvg = 0;

  totalTime = (double)(millis())/3600000.0;///(1000*60*60);

  //emon1.analogDataList->clear();

  Irms = emon1.calcIrms(1480 );  // Calculate Irms only
  //int sampleCount = 1000;

  //Serial.print("0000000000000000");
  //Serial.println("00");//emon1.analogDataList->size());

  // while(--sampleCount > 0)
  // {
  //
  //   Serial.println(emon1.analogDataList->shift());
  // }

  power = Irms*230.0;

  if(Irms>0.20)  // probably ignore accounting anything below 200mA
  {

    if(Irms>2.0)
    {
      whetherMotorIsOn = true;
    }

    deltaTime = ( totalTime - lastTime );

    totalDeltaTime += deltaTime; // required due to if(Irms>0.20)

    IrmskWh += (power * totalDeltaTime) / 3600000.0 / 1000.0;

  }

  if(Irms<0.20)
  {
    whetherMotorIsOn = false;
  }

  Serial.println(Irms);

  lastTime = totalTime;

  ///#if !defined(__AVR_ATmega2560__)

  static int discCnt = 0;

  if (millis() > Bot_lasttime + Bot_mtbs)  {

    // check internet connectivity

    if (WiFi.status() != WL_CONNECTED) //!wClient.connect(host, httpPort))
    {
      Serial.println("Internet connectivity failed");
      //wClient.stop();
      hasSentLast = false;

      // if(lastWifiConnectivityStatus != wClient.connected()) // since passed the last return status has to return connected status here
      // {
      lastDisconnectionTs = millis();
      //   lastWifiConnectivityStatus = wClient.connected();
      // }
      discCnt++;
      delay(5000);
      return;
    }
    //wClient.stop();

    // while (WiFi.status() != WL_CONNECTED)
    // {
    //   delay(500);
    //   Serial.print(".");
    // }
    //
    // Serial.print(" connected to wifi.\nChecking internet connectivity ..");



    if(!hasSentLast)
    {

      hasSentLast = bot.sendMessage("-237644374"
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

      //lastDisconnectionTs = millis();
    }


    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
    Bot_EchoMessages(whetherMotorIsOn, Irms);   // reply to message with Echo

    if(autoStatus || Started)
    {
      // char str[50];
      //
      // if(Irms>0.20)  // probably ignore accounting anything below 200mA
      // {
      //   sprintf(str, "Data : %f", Irms);
      // }
      // else
      // {
      //   sprintf(str, "Data : 0.0", Irms);

      // }

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

        duration = timeScale(&timediff);

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

        // if(timediff >= 1000 && lastTimeMultiplierChecked)
        // {
        //
        //   timediff /= 1000; // seconds
        //   duration = String(timediff);
        //   duration += " s ";
        //
        //   Serial.print(" timediff seconds: ");
        //   Serial.print(timediff);
        // }
        // else
        // {
        //   lastTimeMultiplierChecked = false;
        // }
        //
        // if(timediff >= 60 && lastTimeMultiplierChecked)
        // {
        //   timediff /= 60; // minutes
        //   duration = String(timediff);
        //   duration += " m ";
        //
        //   Serial.print(" timediff minutes: ");
        //   Serial.print(timediff);
        //
        // }
        // else
        // {
        //   lastTimeMultiplierChecked = false;
        // }
        //
        //
        //
        // if(timediff >= 60 && lastTimeMultiplierChecked)
        // {
        //   timediff /= 60; // hours
        //   duration = String(timediff);
        //   duration += " h ";
        //
        //   Serial.print(" timediff hours: ");
        //   Serial.print(timediff);
        // }
        // else
        // {
        //   lastTimeMultiplierChecked = false;
        // }
        //
        //
        // if(timediff >= 24 && lastTimeMultiplierChecked)
        // {
        //   timediff /= 24; // days
        //   duration = String(timediff);
        //   duration += " d ";
        //
        //   Serial.print(" timediff days: ");
        //   Serial.print(timediff);
        // }
        // else
        // {
        //   lastTimeMultiplierChecked = false; // :P
        // }
        //


      }
    }

    Bot_lasttime = millis();
  }

  //#endif // !defined(__AVR_ATmega2560__)





}

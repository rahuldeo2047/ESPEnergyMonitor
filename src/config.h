#ifndef CONFIG_H
#define CONFIG_H

#define WIFISSID "HHH7351HHH"// Put your WiFi SSID here
#define PASSWORD "hh1537hhh" // Put your Wifi Password hereconst char *GScriptId = “—Your-Google-Script-ID–=”;

const char* apiWritekey = "U6208ODGB0Q9SKJG";
const char* ssid = "JioFi3_3FA858";
const char* password = "mnajk1h6tz" ;

const char* server = "api.thingspeak.com";
float resolution=3.3/1023;

   // GET https://api.thingspeak.com /update?api_key=K3CC6GPXNVQULRRX&field1=0&field1=0&field1=0
  const int updateThingSpeakInterval = 20 ;//* 1000; // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)





//#define TEST_MODE

#define TELEGRAM_HOST "api.telegram.org"
#define testBOTtoken "392509569:AAHNVA79fG9qsVQ9dgSS2GMXrZ5SLejEs_s"
#define testBOTname "testBot"
#define testBOTusername "test_watermotorstatus_bot"

#define BOTtoken "444154317:AAFuKAx319tadCnWxHv9hdA0MiDbPLHxoj8"  //token of TestBOT
//#define BOTtoken (testBOTtoken)

#define BOTname "Water-Electric-Motor"
#define BOTusername "WaterElectricMotorBot"
#define telegramDebugID  "55129840"
#define telegramGroupID  "-237644374"

// comment if GSCIPT BASED CONNECTION IS NEEDED
#define PHP_HOST_BASED_CONNECTION



////////////////////////////////////////

// https://in.000webhost.com/cpanel-login?from=panel
// ID rahuldeo2047@gmail.com
// PSSS whattokeep
// index.php https://tricentennial-accum.000webhostapp.com/
// logger.php https://tricentennial-accum.000webhostapp.com/logger.php?type=powerOn&ellapsed_time=33575&current_factor=0.13&motor_status=OFF&version=v0.0.1-2-gef42b98

#define PHP_HOST "tricentennial-accum.000webhostapp.com" //"watermotor-datalogger.rf.gd" //logger.php?"

#define PHP_HOST_1  "watermotordatalogger.000webhostapp.com"

////////////////////////////////////////
#define GSCRIPT_DEPLOY_TYPE ("/dev?")  //("/exec?")
//https://script.google.com/macros/s/AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg/exec
#define GScript_Id  "AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg"
                                   //"AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ"
//https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec
//https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec

#define GScript_host "script.google.com"
#define GScript_googleRedirHost "script.googleusercontent.com"
#define GScript_httpsPort ( 443 )

#define MOVING_AVERAGE_COUNT (10);

#define current_factor_threshold_for_motor_ON (1.0)

#define current_factor_threshold_for_motor_ON_No_Load (4.8)

#define DISCONNECTION_TIME (60000)

//#define WIFISSID "D_ATOM_1_2"// Put your WiFi SSID here
//#define PASSWORD "D_ATOM_1" // Put your Wifi Password here

#endif // CONFIG_H

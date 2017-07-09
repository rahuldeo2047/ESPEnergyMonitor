#ifndef CONFIG_H
#define CONFIG_H

#define WIFISSID "HHH7351HHH"// Put your WiFi SSID here
#define PASSWORD "hh1537hhh" // Put your Wifi Password hereconst char *GScriptId = “—Your-Google-Script-ID–=”;


#define testBOTtoken "392509569:AAHNVA79fG9qsVQ9dgSS2GMXrZ5SLejEs_s"
#define testBOTname "testBot"
#define testBOTusername "test_watermotorstatus_bot"

//#define BOTtoken "444154317:AAFuKAx319tadCnWxHv9hdA0MiDbPLHxoj8"  //token of TestBOT
#define BOTtoken (testBOTtoken)

#define BOTname (testBOTname) //"Water-Electric-Motor"
#define BOTusername (testBOTusername) //"WaterElectricMotorBot"
#define telegramDebugID  "55129840"
#define telegramGroupID  "55129840" //"-237644374"


#define GSCRIPT_DEPLOY_TYPE ("/dev?")  //("/exec?")
//https://script.google.com/macros/s/AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg/exec
#define GScript_Id  "AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg"
                                   //"AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ"
//https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec
//https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec

#define GScript_host "script.google.com"
#define GScript_googleRedirHost "script.googleusercontent.com"
#define GScript_httpsPort ( 443 )

#define MOVING_AVERAGE_COUNT (100);

#define current_factor_threshold_for_motor_ON (1.0)


//#define WIFISSID "D_ATOM_1_2"// Put your WiFi SSID here
//#define PASSWORD "D_ATOM_1" // Put your Wifi Password here

#endif // CONFIG_H

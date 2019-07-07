#ifndef CONFIG_H
#define CONFIG_H

#define STR(x) #x
#define XSTR(x) STR(x) 
#define _VER_ XSTR(VER)

#define STRCAT(A, B) A B

// Four char 
// Development, Observation, Production, Mass-Deployable 
#define DEVICE_DEVELOPMENT_TYPE "DEVT" //  ("OBSN") ("PROD") ("DEPL")
#define UPDATE_PATH "/devices/updates/"
// Above will be web folder names

char DEVICE_ID[6]={0/* The mac ID */};
char DEVICE_ID_STR[13]={0/* The mac ID */};


// data structure for the device configuraiton
struct Device_config
{
 int config_id;
 char device_code_to_update_to[32]; // v000.000.000-000-gf676f80
 char device_code_type[8];
 char server_host_address_data[128]; // e.g. device1-eews.000webhostapp.com
 int server_host_port_data;
 char device_min_code_version[32]; // 000.000.000-000 or full v000.000.000-000-gf676f80
 char device_max_code_version[32]; // 000.000.000-000 or full v000.000.000-000-gf676f80
 int available_sensor_count;
 unsigned char sensor_current_enabled; // bool
 unsigned char sensor_temperature_enabled; // bool
 unsigned char sensor_vibration_enabled; // bool
 float sensor_current_threshold_normal;
 float sensor_current_threshold_alert;
 float sensor_current_threshold_warning;
 float sensor_current_threshold_critical;
 float sensor_vibration_threshold_normal;
 float sensor_vibration_threshold_alert;
 float sensor_vibration_threshold_warning;
 float sensor_vibration_threshold_critical;
 float sensor_temperature_threshold_normal;
 float sensor_temperature_threshold_alert;
 float sensor_temperature_threshold_warning;
 float sensor_temperature_threshold_critical;
 long sensor_current_sample_time_duration;
 long sensor_vibration_sample_time_duration;
 long sensor_temperature_sample_time_duration;
 long sensor_current_total_time_duration;
 long sensor_vibration_total_time_duration;
 long sensor_temperature_total_time_duration;
 long sensor_current_buzzer_light_notify_level;
 long sensor_temerature_buzzer_light_notify_level;
 long sensor_vibration_buzzer_light_notify_level;
 long development_print_level; // bit-field
}Device_config;

// 
// For HTTP web update following three variables should be used for the selection of upload
// _VER_, DEVICE_DEVELOPMENT_TYPE, DEVICE_ID
// The device ID becomes optional in case of _VER_ and DEVICE_DEVELOPMENT_TYPE

#define WIFISSID "HHA2204HH" // Put your WiFi SSID here
#define PASSWORD "hha2204hh" // Put your Wifi Password hereconst char *GScriptId = “—Your-Google-Script-ID–=”;


#define DISABLED (0)
#define ENABLED (1)

//#define TEMPERATURE_SUB_DEVICE (ENABLE)
#define CURRENT_SUB_DEVICE (DISABLED)
#define VIBRATION_SUB_DEVICE (ENABLED)

const char *apiWritekey = "U6208ODGB0Q9SKJG";
const char *ssid = WIFISSID;     //"JioFi3_3FA858";
const char *password = PASSWORD; //"mnajk1h6tz" ;

const char *server = "api.thingspeak.com";
float resolution = 3.3 / 1023;

// GET https://api.thingspeak.com /update?api_key=K3CC6GPXNVQULRRX&field1=0&field1=0&field1=0
const int updateThingSpeakInterval = 10000; //* 1000; // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

/// Local php server

const char *php_server = "device1-eews.000webhostapp.com" ;//"192.168.43.227";
const uint16_t php_server_port = 80; //8000;
const char *php_server_file_target = "/devices/insert_new_data.php?";
//const char *php_server_response = "New record created successfully.";

const char *php_config_server = "device1-eews.000webhostapp.com" ;//"192.168.43.227";
const uint16_t php_config_server_port = 80; //8000;
const char *php_config_server_file_target = "/devices/get_config.php?";
//const char *php_server_response = "New record created successfully.";

const char *php_upgrade_server = "device1-eews.000webhostapp.com" ;//"192.168.43.227";
const uint16_t php_upgrade_server_port = 80; //8000;
const char *php_upgrade_server_file_target = STRCAT(UPDATE_PATH, DEVICE_DEVELOPMENT_TYPE);

//#define TEST_MODE

#define TELEGRAM_HOST "api.telegram.org"
#define testBOTtoken "392509569:AAHNVA79fG9qsVQ9dgSS2GMXrZ5SLejEs_s"
#define testBOTname "testBot"
#define testBOTusername "test_watermotorstatus_bot"

#define BOTtoken "444154317:AAFuKAx319tadCnWxHv9hdA0MiDbPLHxoj8" //token of TestBOT
//#define BOTtoken (testBOTtoken)

#define BOTname "Water-Electric-Motor"
#define BOTusername "WaterElectricMotorBot"
#define telegramDebugID "55129840"
#define telegramGroupID "-237644374"

// comment if GSCIPT BASED CONNECTION IS NEEDED
#define PHP_HOST_BASED_CONNECTION

////////////////////////////////////////

// https://in.000webhost.com/cpanel-login?from=panel
// ID rahuldeo2047@gmail.com
// PSSS whattokeep
// index.php https://tricentennial-accum.000webhostapp.com/
// logger.php https://tricentennial-accum.000webhostapp.com/logger.php?type=powerOn&ellapsed_time=33575&current_factor=0.13&motor_status=OFF&version=v0.0.1-2-gef42b98

#define PHP_HOST "tricentennial-accum.000webhostapp.com" //"watermotor-datalogger.rf.gd" //logger.php?"

#define PHP_HOST_1 "watermotordatalogger.000webhostapp.com"

////////////////////////////////////////
#define GSCRIPT_DEPLOY_TYPE ("/dev?") //("/exec?")
//https://script.google.com/macros/s/AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg/exec
#define GScript_Id "AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg"
//"AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ"
//https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec
//https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec

#define GScript_host "script.google.com"
#define GScript_googleRedirHost "script.googleusercontent.com"
#define GScript_httpsPort (443)

#define MOVING_AVERAGE_COUNT (10);

#define current_factor_threshold_for_motor_ON (1.0)

#define current_factor_threshold_for_motor_ON_No_Load (4.8)

#define DISCONNECTION_TIME (60000)

//#define WIFISSID "D_ATOM_1_2"// Put your WiFi SSID here
//#define PASSWORD "D_ATOM_1" // Put your Wifi Password here

#endif // CONFIG_H

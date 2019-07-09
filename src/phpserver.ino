
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include "config.h"

#include "ArduinoJson.h"

float php_tmp_f, php_tmp_r, php_current_f, php_current_r, php_accel_f, php_accel_r;
unsigned long php_sr_ser, php_uptm;

WiFiClient http_wificlient;

//HTTPClient http;

void setup_php_server()
{
    //WiFiClient client;

    //if (http.begin(client, php_server))
    {
    }
}

/* [
   {
      "config_id":"0",
      "device_code_to_update_to":"v0.0.3-3-g53a0111",
      "device_code_type":"DEVT",
      "server_host_address_data":"device1-eews.000webhostapp.com",
      "server_host_port_data":"80",
      "device_min_code_version":"0.0.3-3",
      "device_max_code_version":"0.0.3-5",
      "available_sensor_count":"3",
      "sensor_current_enabled":"0",
      "sensor_temperature_enabled":"1",
      "sensor_vibration_enabled":"1",
      "sensor_current_threshold_normal":"1",
      "sensor_current_threshold_alert":"8",
      "sensor_current_threshold_warning":"16",
      "sensor_current_threshold_critical":"20",
      "sensor_vibration_threshold_normal":"0.1",
      "sensor_vibration_threshold_alert":"1",
      "sensor_vibration_threshold_warning":"3",
      "sensor_vibration_threshold_critical":"5",
      "sensor_temperature_threshold_normal":"25",
      "sensor_temperature_threshold_alert":"35",
      "sensor_temperature_threshold_warning":"40",
      "sensor_temperature_threshold_critical":"55",
      "sensor_current_sample_time_duration":"400000",
      "sensor_vibration_sample_time_duration":"80000",
      "sensor_temperature_sample_time_duration":"0",
      "sensor_current_total_time_duration":"1000",
      "sensor_vibration_total_time_duration":"3300",
      "sensor_temperature_total_time_duration":"0",
      "sensor_current_buzzer_light_notify_level":"255",
      "sensor_temperature_buzzer_light_notify_level":"255",
      "sensor_vibration_buzzer_light_notify_level":"255",
      "development_print_level":"0"
   }
]
*/

bool readDeviceConfig(struct Device_config *_device_config_data)
{
    // Compute optimal size of the JSON buffer according to what we need to parse.
    // See https://bblanchon.github.io/ArduinoJson/assistant/
    const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) +
                              2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) +
                              JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 390;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject &root = jsonBuffer.parseObject(http_wificlient);

    if (!root.success())
    {
        Serial.println("JSON parsing failed!");
        return false;
    }

    // Here were copy the strings we're interested in using to your struct data
    _device_config_data->config_id = root["config_id"];

    strncpy(_device_config_data->device_code_to_update_to, root["device_code_to_update_to"], sizeof(_device_config_data->device_code_to_update_to));
    strncpy(_device_config_data->device_code_type, root["device_code_type"], sizeof(_device_config_data->device_code_type));
    strncpy(_device_config_data->server_host_address_data, root["server_host_address_data"], sizeof(_device_config_data->server_host_address_data));

    _device_config_data->server_host_port_data = root["server_host_port_data"];

    strncpy(_device_config_data->device_min_code_version, root["device_min_code_version"], sizeof(_device_config_data->device_min_code_version));
    strncpy(_device_config_data->device_max_code_version, root["device_max_code_version"], sizeof(_device_config_data->device_max_code_version));

    _device_config_data->available_sensor_count = root["available_sensor_count"];
    _device_config_data->sensor_current_enabled = root["sensor_current_enabled"];
    _device_config_data->sensor_temperature_enabled = root["sensor_temperature_enabled"];
    _device_config_data->sensor_vibration_enabled = root["sensor_vibration_enabled"];

    _device_config_data->sensor_current_threshold_normal = root["sensor_current_threshold_normal"];
    _device_config_data->sensor_current_threshold_alert = root["sensor_current_threshold_alert"];
    _device_config_data->sensor_current_threshold_warning = root["sensor_current_threshold_warning"];
    _device_config_data->sensor_current_threshold_critical = root["sensor_current_threshold_critical"];
    _device_config_data->sensor_vibration_threshold_normal = root["sensor_vibration_threshold_normal"];
    _device_config_data->sensor_vibration_threshold_alert = root["sensor_vibration_threshold_alert"];
    _device_config_data->sensor_vibration_threshold_warning = root["sensor_vibration_threshold_warning"];
    _device_config_data->sensor_vibration_threshold_critical = root["sensor_vibration_threshold_critical"];
    _device_config_data->sensor_temperature_threshold_normal = root["sensor_temperature_threshold_normal"];
    _device_config_data->sensor_temperature_threshold_alert = root["sensor_temperature_threshold_alert"];
    _device_config_data->sensor_temperature_threshold_warning = root["sensor_temperature_threshold_warning"];
    _device_config_data->sensor_temperature_threshold_critical = root["sensor_temperature_threshold_critical"];

    _device_config_data->sensor_current_sample_time_duration = root["sensor_current_sample_time_duration"];
    _device_config_data->sensor_vibration_sample_time_duration = root["sensor_vibration_sample_time_duration"];
    _device_config_data->sensor_temperature_sample_time_duration = root["sensor_temperature_sample_time_duration"];
    _device_config_data->sensor_current_total_time_duration = root["sensor_current_total_time_duration"];
    _device_config_data->sensor_vibration_total_time_duration = root["sensor_vibration_total_time_duration"];
    _device_config_data->sensor_temperature_total_time_duration = root["sensor_temperature_total_time_duration"];

    _device_config_data->sensor_current_buzzer_light_notify_level = root["sensor_current_buzzer_light_notify_level"];
    _device_config_data->sensor_temerature_buzzer_light_notify_level = root["sensor_temerature_buzzer_light_notify_level"];
    _device_config_data->sensor_vibration_buzzer_light_notify_level = root["sensor_vibration_buzzer_light_notify_level"];

    _device_config_data->development_print_level = root["development_print_level"]; // bit-field

    // It's not mandatory to make a copy, you could just use the pointers
    // Since, they are pointing inside the "content" buffer, so you need to make
    // sure it's still in memory when you read the string

    return true;
}

// Print the data extracted from the JSON
void printDeviceConfig(struct Device_config *_device_config_data)
{
    Serial.print("config_id=");
    Serial.println(_device_config_data->config_id);

    Serial.print("device_code_to_update_to");
    Serial.println(_device_config_data->device_code_to_update_to);
    Serial.print("device_code_type");
    Serial.println(_device_config_data->device_code_type);
    Serial.print("server_host_address_data");
    Serial.println(_device_config_data->server_host_address_data);

    Serial.print("server_host_port_data");
    Serial.println(_device_config_data->server_host_port_data);

    Serial.print("device_min_code_version");
    Serial.println(_device_config_data->device_min_code_version);
    Serial.print("device_max_code_version");
    Serial.println(_device_config_data->device_max_code_version);

    Serial.print("available_sensor_count");
    Serial.println(_device_config_data->available_sensor_count);
    Serial.print("sensor_current_enabled");
    Serial.println(_device_config_data->sensor_current_enabled);
    Serial.print("sensor_temperature_enabled");
    Serial.println(_device_config_data->sensor_temperature_enabled);
    Serial.print("sensor_vibration_enabled");
    Serial.println(_device_config_data->sensor_vibration_enabled);

    Serial.print("sensor_current_threshold_normal");
    Serial.println(_device_config_data->sensor_current_threshold_normal);
    Serial.print("sensor_current_threshold_alert");
    Serial.println(_device_config_data->sensor_current_threshold_alert);
    Serial.print("sensor_current_threshold_warning");
    Serial.println(_device_config_data->sensor_current_threshold_warning);
    Serial.print("sensor_current_threshold_critical");
    Serial.println(_device_config_data->sensor_current_threshold_critical);
    Serial.print("sensor_vibration_threshold_normal");
    Serial.println(_device_config_data->sensor_vibration_threshold_normal);
    Serial.print("sensor_vibration_threshold_alert");
    Serial.println(_device_config_data->sensor_vibration_threshold_alert);
    Serial.print("sensor_vibration_threshold_warning");
    Serial.println(_device_config_data->sensor_vibration_threshold_warning);
    Serial.print("sensor_vibration_threshold_critical");
    Serial.println(_device_config_data->sensor_vibration_threshold_critical);
    Serial.print("sensor_temperature_threshold_normal");
    Serial.println(_device_config_data->sensor_temperature_threshold_normal);
    Serial.print("sensor_temperature_threshold_alert");
    Serial.println(_device_config_data->sensor_temperature_threshold_alert);
    Serial.print("sensor_temperature_threshold_warning");
    Serial.println(_device_config_data->sensor_temperature_threshold_warning);
    Serial.print("sensor_temperature_threshold_critical");
    Serial.println(_device_config_data->sensor_temperature_threshold_critical);

    Serial.print("sensor_current_sample_time_duration");
    Serial.println(_device_config_data->sensor_current_sample_time_duration);
    Serial.print("sensor_vibration_sample_time_duration");
    Serial.println(_device_config_data->sensor_vibration_sample_time_duration);
    Serial.print("sensor_temperature_sample_time_duration");
    Serial.println(_device_config_data->sensor_temperature_sample_time_duration);
    Serial.print("sensor_current_total_time_duration");
    Serial.println(_device_config_data->sensor_current_total_time_duration);
    Serial.print("sensor_vibration_total_time_duration");
    Serial.println(_device_config_data->sensor_vibration_total_time_duration);
    Serial.print("sensor_temperature_total_time_duration");
    Serial.println(_device_config_data->sensor_temperature_total_time_duration);

    Serial.print("sensor_current_buzzer_light_notify_level");
    Serial.println(_device_config_data->sensor_current_buzzer_light_notify_level);
    Serial.print("sensor_temerature_buzzer_light_notify_level");
    Serial.println(_device_config_data->sensor_temerature_buzzer_light_notify_level);
    Serial.print("sensor_vibration_buzzer_light_notify_level");
    Serial.println(_device_config_data->sensor_vibration_buzzer_light_notify_level);

    Serial.print("development_print_level");
    Serial.println(_device_config_data->development_print_level); // bit-field
}

//POST /test/demo_form.php HTTP/1.1
//Host: w3schools.com
//name1=value1&name2=value2

bool sendToServer(String data_str, const char *_php_server, const char *_php_server_port, const char *_php_server_file_target);

// device_code_type
// congif_id
// code_version my_version
struct Device_config g_device_config_data;

bool getDeviceConfig(int config_id, struct Device_config *_config)
{
    // +"&device_code_type="+String(DEVICE_DEVELOPMENT_TYPE);
    // +"&device_code_version="+String(_VER_);
    // +"&device_id=device_id_"+String(DEVICE_ID_STR);

    String data_str = "device_code_type=" + String(DEVICE_DEVELOPMENT_TYPE) + "&config_id=" + String(config_id) + "&device_code_version=" + _VER_;
    bool status = sendToServer(data_str, php_config_server, php_config_server_port, php_config_server_file_target);
    if (status == true)
    {
        status = readDeviceConfig(&g_device_config_data);
    }

    if (status == true)
    {
        *_config = g_device_config_data;
        printDeviceConfig(&g_device_config_data);
    }

    return status;
}

bool sendToServer(String data_str, const char *_php_server, uint16_t _php_server_port, const char *_php_server_file_target)
{
    bool status = true;
    long total_time_taken = 0;
    long ts_wait_for_client = millis();
    http_wificlient.setTimeout(1000);
    if (!http_wificlient.connect(_php_server, _php_server_port))
    {
        Serial.println("connection failed");
        status = false;
        return status;
    }

    Serial.print("'connect' time taken ");
    total_time_taken = millis() - ts_wait_for_client;
    Serial.println(millis() - ts_wait_for_client);
    ts_wait_for_client = millis();

    http_wificlient.setTimeout(50);
    String getStr = "GET " + String(_php_server_file_target) + data_str + " HTTP/1.1\r\n";
    getStr += "Host: " + String(_php_server) + "\r\n\r\n";

    // Send request to the server:
    http_wificlient.print(getStr);

    Serial.println("===========================");
    Serial.println(getStr);
    Serial.println("===========================");

    Serial.print("'send' time taken ");
    total_time_taken += millis() - ts_wait_for_client;
    Serial.println(millis() - ts_wait_for_client);
    ts_wait_for_client = millis();

    Serial.print("Wating ... ");

    ts_wait_for_client = millis();
    while (!http_wificlient.available())
    {
        if (millis() - ts_wait_for_client > 10000)
        {
            Serial.println(" timed out.");
            status = false;
            http_wificlient.stop();
            return status;
            break;
        }
    }

    Serial.print("'wait' time taken ");
    total_time_taken += millis() - ts_wait_for_client;
    Serial.println(millis() - ts_wait_for_client);
    ts_wait_for_client = millis();

    char endOfHeaders[] = "\r\n\r\n";
    bool ok = http_wificlient.find(endOfHeaders);

    Serial.println();
    Serial.print("Data from server: ");

    //Serial.println(http_wificlient.readString());
    // TODO: check for validity
    // call either readReponseContent(...) or read as above

    Serial.print("'read' time taken ");
    total_time_taken += millis() - ts_wait_for_client;
    Serial.println(millis() - ts_wait_for_client);

    Serial.print("'total' time taken ");
    Serial.println(total_time_taken);

    return status;
}

bool whether_new_code_available = false;

bool getUpdateAvailable()
{
    return whether_new_code_available;
}

bool readCodeUpdateStatus(struct Device_update_info *_device_update_info)
{
    // Compute optimal size of the JSON buffer according to what we need to parse.
    // See https://bblanchon.github.io/ArduinoJson/assistant/
    const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) +
                              2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) +
                              JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 390;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject &root = jsonBuffer.parseObject(http_wificlient);

    if (!root.success())
    {
        Serial.println("JSON parsing failed!");
        return false;
    }

    struct Device_update_info device_update_info;

    // This can use too much of data on internet download part

    device_update_info.device_code_to_update_to;
    device_update_info.config_id = root["config_id"];
    device_update_info.whether_update_available = root["whether_update_available"];
    strncpy(device_update_info.device_code_version, root["device_code_version"], sizeof(device_update_info.device_code_version));
    strncpy(device_update_info.host_server_address, root["host_server_address"], sizeof(device_update_info.host_server_address));
    device_update_info.host_server_port = root["host_server_port"];
    strncpy(device_update_info.query_path, root["query_path"], sizeof(device_update_info.query_path));
    strncpy(device_update_info.query_path_with_versioned_file, root["query_path_with_versioned_file"], sizeof(device_update_info.query_path_with_versioned_file));

    *_device_update_info = device_update_info;

    return true;
}

bool sendDataToServer(String data_str, struct Device_config *_config = NULL)
{
    bool status = sendToServer(data_str, php_server, php_server_port, php_server_file_target);

    whether_new_code_available = false;

    if (status == true)
    {
        status = readDeviceConfig(&g_device_config_data);
    }

    if (status == true)
    {
        *_config = g_device_config_data;
        printDeviceConfig(&g_device_config_data);
    }
    return status;
}

bool loop_php_server(unsigned long _php_sr, unsigned long _php_uptm, float _php_temp_f, float _php_temp_r, float _php_current_f, float _php_current_r, float _php_accel_f, float _php_accel_r)
{
    //WiFiClient client;

    HTTPClient http;

    php_sr_ser = _php_sr;
    php_uptm = _php_uptm;
    php_tmp_f = _php_temp_f;
    php_tmp_r = _php_temp_r;
    php_current_f = _php_current_f;
    php_current_r = _php_current_r;
    php_accel_f = _php_accel_f;
    php_accel_r = _php_accel_r;

    String query_str = "sr=" + String(php_sr_ser) + "&dt=0" + "&time=0000-00-00T00:00:00" + "&uptm=" + String(php_uptm) + "&temp_filter=" + String(php_tmp_f) + "&temp_raw=" + String(php_tmp_r) + "&curr_filter=" + String(php_current_f) + "&curr_raw=" + String(php_current_r) + "&accel_filter=" + String(php_accel_f) + "&accel_raw=" + String(php_accel_r) 
    + "&device_code_type=" + String(DEVICE_DEVELOPMENT_TYPE) 
    + "&device_code_version=" + String(_VER_) 
    + "&config_id=" + String(g_device_config_data.config_id) 
    + "&device_id=device_id_" + String(DEVICE_ID_STR);

    return sendDataToServer(query_str);
    
}

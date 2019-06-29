
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include "config.h"

float php_tmp_f, php_tmp_r, php_current_f, php_current_r, php_accel_f, php_accel_r;
unsigned long php_sr_ser, php_uptm;

//HTTPClient http;

void setup_php_server()
{
    //WiFiClient client;

    //if (http.begin(client, php_server))
    {
    }
}

//POST /test/demo_form.php HTTP/1.1
//Host: w3schools.com
//name1=value1&name2=value2



WiFiClient http_wificlient;
bool sendToServer(String data_str);

bool sendDeviceId()
{
    String data_str = String(DEVICE_ID_STR);
    return sendToServer(data_str);
}


bool sendToServer(String data_str)
{
    bool status = true;
    long total_time_taken = 0;
    long ts_wait_for_client = millis();
    http_wificlient.setTimeout(1000);
    if (!http_wificlient.connect(php_server, php_server_port))
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
    String getStr = "GET " + String(php_server_file_target) + data_str + " HTTP/1.1\r\n";
    getStr += "Host: " + String(php_server) + "\r\n\r\n";
    
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

    Serial.println();
    Serial.print("Data from server: ");
    Serial.println(http_wificlient.readString());
    Serial.print("'read' time taken ");
    total_time_taken += millis() - ts_wait_for_client;
    Serial.println(millis() - ts_wait_for_client);

    Serial.print("'total' time taken "); 
    Serial.println(total_time_taken);

    return status;

}

void loop_php_server(unsigned long _php_sr, unsigned long _php_uptm, float _php_temp_f, float _php_temp_r, float _php_current_f, float _php_current_r, float _php_accel_f, float _php_accel_r)
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

    String query_str = "sr=" + String(php_sr_ser) + "&dt=0" + "&time=0000-00-00T00:00:00" + "&uptm=" + String(php_uptm) + "&temp_filter=" + String(php_tmp_f) + "&temp_raw=" + String(php_tmp_r) + "&curr_filter=" + String(php_current_f) + "&curr_raw=" + String(php_current_r) + "&accel_filter=" + String(php_accel_f) + "&accel_raw=" + String(php_accel_r);
   // +"&device_id=device_id_"+String(DEVICE_ID_STR);

    sendToServer(query_str);
    return;

    String complete_url = String(php_server) + String(":") + String(php_server_port) + String(php_server_file_target) + String(query_str);

    Serial.println(complete_url);

    //HTTPClient http;    //Declare object of class HTTPClient

    //    http.begin("http://192.168.1.88:8085/hello");      //Specify request destination
    //    http.addHeader("Content-Type", "text/plain");  //Specify content-type header

    //    int httpCode = http.POST("Message from ESP8266");   //Send the request
    //    String payload = http.getString();                  //Get the response payload

    //    Serial.println(httpCode);   //Print HTTP return code
    //    Serial.println(payload);    //Print request response payload

    //    http.end();  //Close connection

    uint32_t ts_php_send_time = millis();

    http.setReuse(true);
    http.setUserAgent("ESP8266 IOT #1");
    http.setTimeout(50);

    //if(http.connected())
    {
        //Specify request destination
        bool status_php_server; //= http.begin(client, php_server, php_server_port, String(php_server_file_target) + String(query_str), false);
        status_php_server = http.begin(complete_url);
        if (status_php_server == true)
        {

            //http.addHeader("Content-Type", "text/plain");

            int httpCode = http.POST("");
            //.GET();         //Send the request
            String payload = http.getString(); //Get the response payload

            ts_php_send_time = millis() - ts_php_send_time;

            Serial.print("Time spent to send the msg ");
            Serial.println(ts_php_send_time);

            Serial.println(HTTPClient::errorToString(httpCode)); //Print HTTP return code
            Serial.println(payload);                             //Print request response payload

            // http.end(); //Close connection
        }
    }
}

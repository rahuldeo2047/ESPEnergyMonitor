
#include "config_json_stream_parser.h"
#include "JsonListener.h"


#include "common_def.h"

#include "config.h"

//char print_buffer[256] = {0}; //externed from config.h


void ConfigListener::whitespace(char c) {
  Serial.println("whitespace");
  syslog_debug("whitespace");
}

void ConfigListener::startDocument() {
  Serial.println("start document");
}

void ConfigListener::key(String key) {
  Serial.println("key: " + key);
}

void ConfigListener::value(String value) {
  Serial.println("value: " + value);
}

void ConfigListener::endArray() {
  Serial.println("end array. ");
}

void ConfigListener::endObject() {
  Serial.println("end object. ");
}

void ConfigListener::endDocument() {
  Serial.println("end document. ");
}

void ConfigListener::startArray() {
   Serial.println("start array. ");
}

void ConfigListener::startObject() {
   Serial.println("start object. ");
}

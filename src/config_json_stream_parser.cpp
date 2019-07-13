
#include "config_json_stream_parser.h"
#include "JsonListener.h"

#include "common_def.h"

#include "config.h"

//char print_buffer[256] = {0}; //externed from config.h

void ConfigListener::print(char *msg)
{
  Serial.println(msg);
  syslog_debug(msg);
}


void ConfigListener::whitespace(char c)
{
  sprintf(getPrintBuffer(), "whitespace %c", c);
  this->print(getPrintBuffer());
}

void ConfigListener::startDocument()
{
  sprintf(getPrintBuffer(), "start document");
  this->print(getPrintBuffer());
}

void ConfigListener::key(String key)
{
  this->currentKey = key;
  sprintf(getPrintBuffer(), "key: %s", key.c_str());
  this->print(getPrintBuffer());
}

void ConfigListener::value(String value)
{
  int i, index = -1;

  for(i = 0 ; i < Device_config_enum::vSize; i++)
  {
    String check_key = Device_config_ToString(i);
    if(this->currentKey == check_key)
    {
      index = i;
      break;
    } 
  } 
  sprintf(getPrintBuffer(), " key: %s value: %s", Device_config_ToString(index), value.c_str());
  this->print(getPrintBuffer());


}

void ConfigListener::endArray()
{
  sprintf(getPrintBuffer(), "end array. ");
  this->print(getPrintBuffer());
}

void ConfigListener::endObject()
{
  sprintf(getPrintBuffer(), "end object. ");
  this->print(getPrintBuffer());
}

void ConfigListener::endDocument()
{
  sprintf(getPrintBuffer(), "end document. ");
  this->print(getPrintBuffer());
}

void ConfigListener::startArray()
{
  sprintf(getPrintBuffer(), "start array. ");
  this->print(getPrintBuffer());
}

void ConfigListener::startObject()
{
  sprintf(getPrintBuffer(), "start object. ");
  this->print(getPrintBuffer());
}

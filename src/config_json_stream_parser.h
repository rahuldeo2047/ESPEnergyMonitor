#include "JsonListener.h"

#ifndef CONF_JSON_H
#define CONF_JSON_H



class ConfigListener: public JsonListener {

  public:
    virtual void whitespace(char c);
  
    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();
    
    private:
    void print(char * msg);
    String currentKey = "";
};

#endif //CONF_JSON_H

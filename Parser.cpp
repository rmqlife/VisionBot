#include "Parser.h"
int Parser::get(char c)
{
    // not reading
    if (!reading){
        if (c==begin){
            reading = true;
            len = 0;
            }
        return 0;
    }    
    // reading
    //meet the end, data is available
    if (c==end)
    {
        reading = false;
        return 1;
    }    
    // normal data
    data[len] = c;
    len ++;
    if (len>max_len){
        reading = false;
    }
    
    return 0;
}

Parser::Parser(char b, char e, int len): begin(b), end(e), max_len(len)
{
    reading = false;
}

int Parser::getSerial()
{
  char raw=0;
  if (Serial.available() > 0) {
    raw=(char)Serial.read();
  } else if (Serial1.available() > 0) {
    raw=(char)Serial1.read();;
  } else if (Serial2.available()>0){
    raw=(char)Serial2.read();
  } else if (Serial3.available()>0){
    raw=(char)Serial3.read();
  } else 
    return -1;
  
  return get(raw);
}

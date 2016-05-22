#ifndef PARSER_H
#define PARSER_H
#include "Arduino.h"
class Parser{
  bool reading;
  int max_len;
  char begin;
  char end;
public:
  int len = 0;
  char data[10];  
  // 1: data is available, 0: data is not available
  int get(char c);  
  // init
  Parser(char b = 'b', char e = 'e', int len = 5);
  // get from serial
  int getSerial();
};
#endif

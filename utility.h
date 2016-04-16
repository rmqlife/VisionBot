#ifndef UTILITY_H
#define UTITILY_H
#include "arduino.h"

#define MAX_TOLERANT_STARTUP_MILLIS 500
#define RPM_EQUALS_STOP 10
// time circles in millisecond
#define CIRCLE_SPEED 200
#define CIRCLE_GYRO 50
#define CIRCLE_CHECK 70

#define KEEP_STATUS 2
#define TURN_DEGREE 4
#define FIND_DIRECTION 3
#define TEMP_STATUS 5

#define FEEDBACK_TOLERATE 0.05 
#define DEGREE_TOLERATE 5
float feedbackFreq(float rpm,float feedback,float freq);

float degreeAdd(float da,float db);

#endif

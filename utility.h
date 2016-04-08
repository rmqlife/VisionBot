#ifndef UTILITY_H
#define UTITILY_H
#include "arduino.h"

#define MAX_TOLERANT_STARTUP_MILLIS 500
#define RPM_EQUALS_STOP 50
// time circles in millisecond
#define CIRCLE_SPEED 200
#define CIRCLE_GYRO 100
#define CIRCLE_CHECK 70

#define KEEP_STATUS 2
#define FEEDBACK_CONTROL 4
#define FIND_DIRECTION 3
#define FEEDBACK_TOLERATE 0.05 

float feedbackFreq(float rpm,float feedback,float freq);

#endif

#include "utility.h"

float feedbackFreq(float rpm,float feedback,float freq)
{
  if (abs(rpm - feedback) > rpm * FEEDBACK_TOLERATE) //can not tolerate, need to update
  {
    if (feedback>0.1){// can not divide by 0
      float freqUpdate = freq * rpm / feedback;
      if (freqUpdate < 1.0){
        return freqUpdate;
      }
    }
    return 1.0;
  }
  return freq;
}




#include "hw_config.h"

extern __IO uint8_t PrevXferComplete;

void EP1_IN_Callback(void)
{
  PrevXferComplete = 1; 
}

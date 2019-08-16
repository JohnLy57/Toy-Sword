#include "pwm.h"
int fadein;
// Fade a led on.
void pwm(PwmOut led) { 
    if (fadein){
        led = led + 0.01;
        if (led>=1.0){
            fadein = !fadein;
        }
    }
    else {
        led = led - 0.01;
        if (led<=0.20){
          fadein = !fadein;  
        }
    }
}

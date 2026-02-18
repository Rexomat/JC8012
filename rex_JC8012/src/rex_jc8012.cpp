
#include "common_config.h"

// ESP32P4 JC8012 Test Program
void setup()
{
    rex_lvgl_init();
    rex_lvgl_mainapp();
}

uint16_t ox,oy,x=0,y=0;

void loop()
{
    getTouchXY(&x,&y);
    if(ox != x || oy != y) { Serial.printf("X:%d-Y:%d ",x,y); ox=x; oy=y; }
    delay(5);
}
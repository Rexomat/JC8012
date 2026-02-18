
// ESP32P4 JC8012 Test Program
#define USE_IC8012  1
#include "Arduino.h"
#include "lvgl_mipi/lcd/esp_lcd_jd9365.h"

bool getTouchXY(uint16_t *x, uint16_t *y);
void rex_lvgl_init();
void rex_lvgl_mainapp();
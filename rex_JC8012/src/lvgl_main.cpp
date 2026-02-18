
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/i2c_master.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_heap_caps.h"
#include "esp_ldo_regulator.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_cache.h"
#include "esp_heap_caps.h"
#include "esp_private/esp_cache_private.h"

#include "lvgl_mipi/touch/esp_lcd_touch_gsl3680.h"
#include "lvgl_mipi/lcd/esp_lcd_jd9365.h"
#include "lvgl_port_v9_jd9365.h"

#include "demos/lv_demos.h"
#include "driver/ppa.h"
// #include "images/bg240_320.h"




static void textarea_event_handler(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target_obj(e);
    LV_UNUSED(ta);
    LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}


#define BANNER_H 20

lv_obj_t *rex_lvgl_display_banner(char *text, int posx, int posy, int wid, int hei, lv_palette_t color, lv_palette_t bgcolor)
{

    static lv_style_t std_banner_style;
    lv_style_init(&std_banner_style);
    lv_style_set_radius(&std_banner_style, 5);
    lv_style_set_bg_opa(&std_banner_style, LV_OPA_COVER);
    lv_style_set_bg_color(&std_banner_style, lv_palette_lighten(LV_PALETTE_GREY, 2));
    lv_style_set_border_width(&std_banner_style, 2);
    lv_style_set_border_color(&std_banner_style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_pad_all(&std_banner_style, 2);
    lv_style_set_text_color(&std_banner_style, lv_palette_main(LV_PALETTE_DEEP_ORANGE));
    lv_style_set_text_letter_space(&std_banner_style, 2);
    lv_style_set_text_line_space(&std_banner_style, 2);
    lv_style_set_text_decor(&std_banner_style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_align(&std_banner_style, LV_TEXT_ALIGN_CENTER); // Position des textes im Feld
    lv_style_set_width(&std_banner_style, wid);
    lv_style_set_height(&std_banner_style, BANNER_H);
    lv_style_set_x(&std_banner_style, posx);
    lv_style_set_y(&std_banner_style, posy);
    /*Create an object with the new style*/
    lv_obj_t *obj = lv_label_create(lv_screen_active());
    lv_obj_add_style(obj, &std_banner_style, 0);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, 0);
    lv_obj_align(obj, LV_ALIGN_TOP_LEFT, posx, posy); // Position des Feldes auf dem Bildschirm
    lv_label_set_text(obj, text);

    return obj;
}

lv_obj_t *Monitor_obj = NULL, *windowsText_obj = NULL;

void lv_textconsole(void) /// Eingabefenster mit anscreen Keyboard
{
    rex_lvgl_display_banner("Texteditor", 200, 150, 150, 25, LV_PALETTE_BLUE, LV_PALETTE_LIGHT_GREEN);

    lv_obj_t *tb = lv_textarea_create(lv_screen_active());
    lv_obj_align(tb, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_x(tb, 20);
    lv_obj_set_y(tb, 170);
    lv_obj_set_width(tb, 380);
    lv_obj_set_height(tb, 150);
    lv_obj_set_style_bg_color(tb, lv_palette_lighten(LV_PALETTE_ORANGE, 1), 10);
    lv_obj_add_event_cb(tb, textarea_event_handler, LV_EVENT_READY, tb);
    lv_obj_add_state(tb, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
    windowsText_obj = tb;

    /*Create a keyboard to use it with an of the text areas*/
    lv_obj_t *kb = lv_keyboard_create(lv_screen_active());
    lv_obj_set_pos(kb, 10, 230);
    lv_obj_align(kb, LV_ALIGN_TOP_LEFT, 0, 330);
    lv_obj_set_style_bg_color(kb, lv_palette_main(LV_PALETTE_LIGHT_GREEN), 0);
    /*Create a text area. The keyboard will write here*/
    lv_textarea_set_placeholder_text(windowsText_obj, "Hello");
    // lv_obj_set_size(ta1, 140, 80);
    lv_keyboard_set_textarea(kb, windowsText_obj);
}


void rex_lvgl_mainapp()
{

    if (lvgl_port_lock(-1))
    {

        lv_textconsole(); /// Eingabefenster mit anscreen Keyboard
        lvgl_port_unlock();

    }
}

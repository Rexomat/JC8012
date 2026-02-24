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
#include "lvgl_mipi/lcd/esp_lcd_jd9365.h"
#include "lvgl_mipi/touch/esp_lcd_gsl3680.h"
#include "lvgl_port_v9_jd9365.h"
#include "driver/ppa.h"
#include "demos/lv_demos.h"
//#include "lvgl_mipi/touch/gsl3680_touch.h"
#include "pins_config_JC8012.h"
#include "Arduino.h"


//gsl3680_touch gsltouch = gsl3680_touch(TP_I2C_SDA, TP_I2C_SCL, TP_RST, TP_INT);


#define TAG                                 "main"


i2c_master_bus_handle_t i2c_handle = NULL; 

static const jd9365_lcd_init_cmd_t lcd_cmd[] = {
    {0xE0, (uint8_t[]){0x00}, 1, 0},
     {0xE1, (uint8_t[]){0x93}, 1, 0},
     {0xE2, (uint8_t[]){0x65}, 1, 0},
     {0xE3, (uint8_t[]){0xF8}, 1, 0},
     {0x80, (uint8_t[]){0x01}, 1, 0},
 
     {0xE0, (uint8_t[]){0x01}, 1, 0},
     {0x00, (uint8_t[]){0x00}, 1, 0},
     {0x01, (uint8_t[]){0x39}, 1, 0},
     {0x03, (uint8_t[]){0x10}, 1, 0},
     {0x04, (uint8_t[]){0x41}, 1, 0},
 
     {0x0C, (uint8_t[]){0x74}, 1, 0},
     {0x17, (uint8_t[]){0x00}, 1, 0},
     {0x18, (uint8_t[]){0xD7}, 1, 0},
     {0x19, (uint8_t[]){0x00}, 1, 0},
     {0x1A, (uint8_t[]){0x00}, 1, 0},
 
     {0x1B, (uint8_t[]){0xD7}, 1, 0},
     {0x1C, (uint8_t[]){0x00}, 1, 0},
     {0x24, (uint8_t[]){0xFE}, 1, 0},
     {0x35, (uint8_t[]){0x26}, 1, 0},
     {0x37, (uint8_t[]){0x69}, 1, 0},
 
     {0x38, (uint8_t[]){0x05}, 1, 0},
     {0x39, (uint8_t[]){0x06}, 1, 0},
     {0x3A, (uint8_t[]){0x08}, 1, 0},
     {0x3C, (uint8_t[]){0x78}, 1, 0},
     {0x3D, (uint8_t[]){0xFF}, 1, 0},
 
     {0x3E, (uint8_t[]){0xFF}, 1, 0},
     {0x3F, (uint8_t[]){0xFF}, 1, 0},
     {0x40, (uint8_t[]){0x06}, 1, 0},
     {0x41, (uint8_t[]){0xA0}, 1, 0},
     {0x43, (uint8_t[]){0x14}, 1, 0},
 
     {0x44, (uint8_t[]){0x0B}, 1, 0},
     {0x45, (uint8_t[]){0x30}, 1, 0},
     //{0x4A, (uint8_t[]){0x35}, 1, 0},//bist
     {0x4B, (uint8_t[]){0x04}, 1, 0},
     {0x55, (uint8_t[]){0x02}, 1, 0},
     {0x57, (uint8_t[]){0x89}, 1, 0},
 
     {0x59, (uint8_t[]){0x0A}, 1, 0},
     {0x5A, (uint8_t[]){0x28}, 1, 0},
 
     {0x5B, (uint8_t[]){0x15}, 1, 0},
     {0x5D, (uint8_t[]){0x50}, 1, 0},
     {0x5E, (uint8_t[]){0x37}, 1, 0},
     {0x5F, (uint8_t[]){0x29}, 1, 0},
     {0x60, (uint8_t[]){0x1E}, 1, 0},
 
     {0x61, (uint8_t[]){0x1D}, 1, 0},
     {0x62, (uint8_t[]){0x12}, 1, 0},
     {0x63, (uint8_t[]){0x1A}, 1, 0},
     {0x64, (uint8_t[]){0x08}, 1, 0},
     {0x65, (uint8_t[]){0x25}, 1, 0},
 
     {0x66, (uint8_t[]){0x26}, 1, 0},
     {0x67, (uint8_t[]){0x28}, 1, 0},
     {0x68, (uint8_t[]){0x49}, 1, 0},
     {0x69, (uint8_t[]){0x3A}, 1, 0},
     {0x6A, (uint8_t[]){0x43}, 1, 0},
 
     {0x6B, (uint8_t[]){0x3A}, 1, 0},
     {0x6C, (uint8_t[]){0x3B}, 1, 0},
     {0x6D, (uint8_t[]){0x32}, 1, 0},
     {0x6E, (uint8_t[]){0x1F}, 1, 0},
     {0x6F, (uint8_t[]){0x0E}, 1, 0},
 
     {0x70, (uint8_t[]){0x50}, 1, 0},
     {0x71, (uint8_t[]){0x37}, 1, 0},
     {0x72, (uint8_t[]){0x29}, 1, 0},
     {0x73, (uint8_t[]){0x1E}, 1, 0},
     {0x74, (uint8_t[]){0x1D}, 1, 0},
 
     {0x75, (uint8_t[]){0x12}, 1, 0},
     {0x76, (uint8_t[]){0x1A}, 1, 0},
     {0x77, (uint8_t[]){0x08}, 1, 0},
     {0x78, (uint8_t[]){0x25}, 1, 0},
     {0x79, (uint8_t[]){0x26}, 1, 0},
 
     {0x7A, (uint8_t[]){0x28}, 1, 0},
     {0x7B, (uint8_t[]){0x49}, 1, 0},
     {0x7C, (uint8_t[]){0x3A}, 1, 0},
     {0x7D, (uint8_t[]){0x43}, 1, 0},
     {0x7E, (uint8_t[]){0x3A}, 1, 0},
 
     {0x7F, (uint8_t[]){0x3B}, 1, 0},
     {0x80, (uint8_t[]){0x32}, 1, 0},
     {0x81, (uint8_t[]){0x1F}, 1, 0},
     {0x82, (uint8_t[]){0x0E}, 1, 0},
     {0xE0,(uint8_t []){0x02},1,0},
 
     {0x00,(uint8_t []){0x1F},1,0},
     {0x01,(uint8_t []){0x1F},1,0},
     {0x02,(uint8_t []){0x52},1,0},
     {0x03,(uint8_t []){0x51},1,0},
     {0x04,(uint8_t []){0x50},1,0},
 
     {0x05,(uint8_t []){0x4B},1,0},
     {0x06,(uint8_t []){0x4A},1,0},
     {0x07,(uint8_t []){0x49},1,0},
     {0x08,(uint8_t []){0x48},1,0},
     {0x09,(uint8_t []){0x47},1,0},
 
     {0x0A,(uint8_t []){0x46},1,0},
     {0x0B,(uint8_t []){0x45},1,0},
     {0x0C,(uint8_t []){0x44},1,0},
     {0x0D,(uint8_t []){0x40},1,0},
     {0x0E,(uint8_t []){0x41},1,0},
 
     {0x0F,(uint8_t []){0x1F},1,0},
     {0x10,(uint8_t []){0x1F},1,0},
     {0x11,(uint8_t []){0x1F},1,0},
     {0x12,(uint8_t []){0x1F},1,0},
     {0x13,(uint8_t []){0x1F},1,0},
 
     {0x14,(uint8_t []){0x1F},1,0},
     {0x15,(uint8_t []){0x1F},1,0},
     {0x16,(uint8_t []){0x1F},1,0},
     {0x17,(uint8_t []){0x1F},1,0},
     {0x18,(uint8_t []){0x52},1,0},
 
     {0x19,(uint8_t []){0x51},1,0},
     {0x1A,(uint8_t []){0x50},1,0},
     {0x1B,(uint8_t []){0x4B},1,0},
     {0x1C,(uint8_t []){0x4A},1,0},
     {0x1D,(uint8_t []){0x49},1,0},
 
     {0x1E,(uint8_t []){0x48},1,0},
     {0x1F,(uint8_t []){0x47},1,0},
     {0x20,(uint8_t []){0x46},1,0},
     {0x21,(uint8_t []){0x45},1,0},
     {0x22,(uint8_t []){0x44},1,0},
 
     {0x23,(uint8_t []){0x40},1,0},
     {0x24,(uint8_t []){0x41},1,0},
     {0x25,(uint8_t []){0x1F},1,0},
     {0x26,(uint8_t []){0x1F},1,0},
     {0x27,(uint8_t []){0x1F},1,0},
 
     {0x28,(uint8_t []){0x1F},1,0},
     {0x29,(uint8_t []){0x1F},1,0},
     {0x2A,(uint8_t []){0x1F},1,0},
     {0x2B,(uint8_t []){0x1F},1,0},
     {0x2C,(uint8_t []){0x1F},1,0},
 
     {0x2D,(uint8_t []){0x1F},1,0},
     {0x2E,(uint8_t []){0x52},1,0},
     {0x2F,(uint8_t []){0x40},1,0},
     {0x30,(uint8_t []){0x41},1,0},
     {0x31,(uint8_t []){0x48},1,0},
 
     {0x32,(uint8_t []){0x49},1,0},
     {0x33,(uint8_t []){0x4A},1,0},
     {0x34,(uint8_t []){0x4B},1,0},
     {0x35,(uint8_t []){0x44},1,0},
     {0x36,(uint8_t []){0x45},1,0},
 
     {0x37,(uint8_t []){0x46},1,0},
     {0x38,(uint8_t []){0x47},1,0},
     {0x39,(uint8_t []){0x51},1,0},
     {0x3A,(uint8_t []){0x50},1,0},
     {0x3B,(uint8_t []){0x1F},1,0},
 
     {0x3C,(uint8_t []){0x1F},1,0},
     {0x3D,(uint8_t []){0x1F},1,0},
     {0x3E,(uint8_t []){0x1F},1,0},
     {0x3F,(uint8_t []){0x1F},1,0},
     {0x40,(uint8_t []){0x1F},1,0},
 
     {0x41,(uint8_t []){0x1F},1,0},
     {0x42,(uint8_t []){0x1F},1,0},
     {0x43,(uint8_t []){0x1F},1,0},
     {0x44,(uint8_t []){0x52},1,0},
     {0x45,(uint8_t []){0x40},1,0},
 
     {0x46,(uint8_t []){0x41},1,0},
     {0x47,(uint8_t []){0x48},1,0},
     {0x48,(uint8_t []){0x49},1,0},
     {0x49,(uint8_t []){0x4A},1,0},
     {0x4A,(uint8_t []){0x4B},1,0},
 
     {0x4B,(uint8_t []){0x44},1,0},
     {0x4C,(uint8_t []){0x45},1,0},
     {0x4D,(uint8_t []){0x46},1,0},
     {0x4E,(uint8_t []){0x47},1,0},
     {0x4F,(uint8_t []){0x51},1,0},
 
     {0x50,(uint8_t []){0x50},1,0},
     {0x51,(uint8_t []){0x1F},1,0},
     {0x52,(uint8_t []){0x1F},1,0},
     {0x53,(uint8_t []){0x1F},1,0},
     {0x54,(uint8_t []){0x1F},1,0},
 
     {0x55,(uint8_t []){0x1F},1,0},
     {0x56,(uint8_t []){0x1F},1,0},
     {0x57,(uint8_t []){0x1F},1,0},
     {0x58,(uint8_t []){0x40},1,0},
     {0x59,(uint8_t []){0x00},1,0},
 
     {0x5A,(uint8_t []){0x00},1,0},
     {0x5B,(uint8_t []){0x10},1,0},
     {0x5C,(uint8_t []){0x05},1,0},
     {0x5D,(uint8_t []){0x50},1,0},
     {0x5E,(uint8_t []){0x01},1,0},
 
     {0x5F,(uint8_t []){0x02},1,0},
     {0x60,(uint8_t []){0x50},1,0},
     {0x61,(uint8_t []){0x06},1,0},
     {0x62,(uint8_t []){0x04},1,0},
     {0x63,(uint8_t []){0x03},1,0},
 
     {0x64,(uint8_t []){0x64},1,0},
     {0x65,(uint8_t []){0x65},1,0},
     {0x66,(uint8_t []){0x0B},1,0},
     {0x67,(uint8_t []){0x73},1,0},
     {0x68,(uint8_t []){0x07},1,0},
 
     {0x69,(uint8_t []){0x06},1,0},
     {0x6A,(uint8_t []){0x64},1,0},
     {0x6B,(uint8_t []){0x08},1,0},
     {0x6C,(uint8_t []){0x00},1,0},
     {0x6D,(uint8_t []){0x32},1,0},
 
     {0x6E,(uint8_t []){0x08},1,0},
     {0xE0,(uint8_t []){0x04},1,0},
     {0x2C,(uint8_t []){0x6B},1,0},
     {0x35,(uint8_t []){0x08},1,0},
     {0x37,(uint8_t []){0x00},1,0},
 
     {0xE0,(uint8_t []){0x00},1,0},
     {0x11,(uint8_t []){0x00},1,0},
     {0x29, (uint8_t[]){0x00}, 1, 5},
     {0x11, (uint8_t[]){0x00}, 1, 120},
     {0x35, (uint8_t[]){0x00}, 1, 0},

};

IRAM_ATTR static bool mipi_dsi_lcd_on_vsync_event(esp_lcd_panel_handle_t panel, esp_lcd_dpi_panel_event_data_t *edata, void *user_ctx)
{
    return lvgl_port_notify_lcd_vsync();
}

#define BSP_LCD_BACKLIGHT   GPIO_NUM_23
#define LCD_LEDC_CH         LEDC_CHANNEL_0
static esp_err_t bsp_display_brightness_init(void)
{
    // Setup LEDC peripheral for PWM backlight control
    gpio_config_t bk_gpio_config = {
        .pin_bit_mask = 1ULL << BSP_LCD_BACKLIGHT,
        .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&bk_gpio_config);
     const ledc_channel_config_t LCD_backlight_channel = {
         .gpio_num = BSP_LCD_BACKLIGHT,
         .speed_mode = LEDC_LOW_SPEED_MODE,
         .channel = LCD_LEDC_CH,
         .intr_type = LEDC_INTR_DISABLE,
         .timer_sel = LEDC_TIMER_1,
         .duty = 0,
         .hpoint = 0
     };
     const ledc_timer_config_t LCD_backlight_timer = {
         .speed_mode = LEDC_LOW_SPEED_MODE,
         .duty_resolution = LEDC_TIMER_10_BIT,
         .timer_num = LEDC_TIMER_1,
         .freq_hz = 5000,
         .clk_cfg = LEDC_AUTO_CLK
     };
     ledc_timer_config(&LCD_backlight_timer);
     ledc_channel_config(&LCD_backlight_channel);
    return ESP_OK;
}


esp_lcd_panel_handle_t disp_panel;
esp_lcd_panel_io_handle_t my_tp_io_handle;
esp_lcd_touch_handle_t my_tp_handle;
extern esp_lcd_touch_handle_t tp;


static esp_err_t bsp_display_brightness_set(int brightness_percent)
{
    if (brightness_percent > 100) {
        brightness_percent = 100;
    }
    if (brightness_percent < 0) {
        brightness_percent = 0;
    }

    ESP_LOGI(TAG, "Setting LCD backlight: %d%%", brightness_percent);
    uint32_t duty_cycle = (1023 * brightness_percent) / 100; // LEDC resolution set to 10bits, thus: 100% = 1023
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH);
    return ESP_OK;
}

static esp_err_t bsp_display_backlight_off(void)
{
    return bsp_display_brightness_set(0);
}

static esp_err_t bsp_display_backlight_on(void)
{
    // return bsp_display_brightness_set(100);
    gpio_set_level(BSP_LCD_BACKLIGHT,1);
    return ESP_OK;
}

extern bool esp_lcd_touch_gsl3680_get_xy(esp_lcd_touch_handle_t tp, uint16_t *x, uint16_t *y, uint16_t *strength, uint8_t *point_num, uint8_t max_point_num);

void my_touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
  bool touched;
  uint16_t touchX, touchY, stren;
  uint8_t pnum,maxnum=2;
    Serial.print(".");
  touched = esp_lcd_touch_gsl3680_get_xy(my_tp_handle, &touchX, &touchY,&stren,&pnum,maxnum);

//  touched = esp_lcd_touch_get_coordinates(my_tp_handle, &touchX, &touchY,&stren,&pnum,maxnum);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    // 设置坐标
    data->point.x = touchX;
    data->point.y = touchY;
    Serial.printf("x=%d,y=%d \r\n",touchX,touchY);
  }
}

void touchpad_read(lv_indev_t *indev_drv, lv_indev_data_t *data);
extern esp_lcd_touch_handle_t tp;

void rex_lvgl_init(void)
{
     bsp_display_brightness_init();
    esp_err_t esp_ret;
    // LEDC Setup
    static esp_ldo_channel_handle_t phy_pwr_chan = NULL;
    esp_ldo_channel_config_t ldo_cfg = {
        .chan_id = BSP_MIPI_DSI_PHY_PWR_LDO_CHAN,
        .voltage_mv = BSP_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV,
    };
    esp_ldo_acquire_channel(&ldo_cfg, &phy_pwr_chan);
    ESP_LOGI(TAG, "MIPI DSI PHY Powered on");

    esp_lcd_dsi_bus_handle_t mipi_dsi_bus;
    esp_lcd_dsi_bus_config_t bus_config = JD9365_PANEL_BUS_DSI_2CH_CONFIG();

    esp_lcd_new_dsi_bus(&bus_config, &mipi_dsi_bus);

     ESP_LOGI(TAG, "Install MIPI DSI LCD control panel");
    // we use DBI interface to send LCD commands and parameters
    esp_lcd_panel_io_handle_t io = NULL;
    esp_lcd_dbi_io_config_t dbi_config =JD9365_PANEL_IO_DBI_CONFIG();

    esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_config, &io);

    disp_panel = NULL;
    ESP_LOGI(TAG, "Install LCD driver of JD9365");
    esp_lcd_dpi_panel_config_t dpi_config =JD9365_800_1280_PANEL_60HZ_DPI_CONFIG(LCD_COLOR_PIXEL_FORMAT_RGB565);

    dpi_config.num_fbs = LVGL_PORT_LCD_BUFFER_NUMS;

    jd9365_vendor_config_t vendor_config = {
        .init_cmds = lcd_cmd,
        .init_cmds_size = sizeof(lcd_cmd) / sizeof(jd9365_lcd_init_cmd_t),
        .mipi_config = {
            .dsi_bus = mipi_dsi_bus,
            .dpi_config = &dpi_config,
        },
    };
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BSP_LCD_RST,
        .rgb_ele_order = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
        .vendor_config = &vendor_config,
    };
    esp_lcd_new_panel_jd9365(io, &panel_config, &disp_panel);
    esp_lcd_panel_reset(disp_panel);
    esp_lcd_panel_init(disp_panel);

    esp_lcd_dpi_panel_event_callbacks_t cbs = {
#if LVGL_PORT_AVOID_TEAR_MODE
        .on_refresh_done = mipi_dsi_lcd_on_vsync_event,
#else
        .on_color_trans_done = mipi_dsi_lcd_on_vsync_event,
#endif
    };
    esp_lcd_dpi_panel_register_event_callbacks(disp_panel, &cbs, NULL);
/////////////// ENDE DISPLAY
////// STARt TOUCH , I2C Init am Anfang der Funktion
 //   gsltouch.begin();    // Use this if the Class funktion is used. Not tested
    if(i2c_handle == NULL)  // Check if handle is already used or create new, prepare to share I2C Bus
    {
        // Initialize the I2C bus
        i2c_master_bus_config_t i2c_bus_conf = {
            .i2c_port = LVGL_IC_NUM,
            .sda_io_num = BSP_I2C_SDA,
            .scl_io_num = BSP_I2C_SCL,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .flags = {
                .enable_internal_pullup = true,
                .allow_pd = 1 ,
            }
        };
        
        i2c_new_master_bus(&i2c_bus_conf, &i2c_handle);
        if (esp_ret != ESP_OK) {
            printf("Error: Failed to initialize I2C bus\r\n");
                //Do whatever you want or ignore
        }     
    }
     
    i2c_device_config_t confi;
    i2c_master_dev_handle_t reti;
    confi.device_address=0x40;
    confi.scl_speed_hz=400000;
    i2c_master_bus_add_device(i2c_handle,&confi,&i2c_dev_handle);
    ESP_LOGE(TAG,"TOUCH device added to I2c Bus");
    
    my_tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GSL3680_CONFIG();
    tp_io_config.scl_speed_hz =400000;
    esp_lcd_new_panel_io_i2c(i2c_handle, &tp_io_config, &my_tp_io_handle);
     const esp_lcd_touch_config_t tp_cfg = {
        .x_max = BSP_LCD_H_RES,
        .y_max = BSP_LCD_V_RES,
        .rst_gpio_num = BSP_LCD_TOUCH_RST, 
        .int_gpio_num = BSP_LCD_TOUCH_INT,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    esp_lcd_touch_new_i2c_gsl3680(my_tp_io_handle, &tp_cfg, &tp);
    lvgl_port_interface_t interface = (dpi_config.flags.use_dma2d) ? LVGL_PORT_INTERFACE_MIPI_DSI_DMA : LVGL_PORT_INTERFACE_MIPI_DSI_NO_DMA;
    ESP_LOGI(TAG,"interface is %d",interface);
    lvgl_port_init(disp_panel, tp, interface);     
    esp_lcd_touch_set_mirror_x(tp,false);
    bsp_display_brightness_set(100);

}

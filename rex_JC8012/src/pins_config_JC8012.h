#pragma once


#define EXAMPLE_LVGL_PORT_TASK_MAX_DELAY_MS 200  //range 2 to 2000
#define EXAMPLE_LVGL_PORT_TASK_MIN_DELAY_MS 2     //range 1 to 100
#define EXAMPLE_LVGL_PORT_TASK_PRIORITY     4
#define EXAMPLE_LVGL_PORT_TASK_STACK_SIZE_KB  6   //KB
#define EXAMPLE_LVGL_PORT_TASK_CORE         -1  //range -1 to 1
#define EXAMPLE_LVGL_PORT_TICK              2   //ragne 1 to 100


#define EXAMPLE_LVGL_PORT_AVOID_TEAR_ENABLE   1

#ifdef  EXAMPLE_LVGL_PORT_AVOID_TEAR_ENABLE
#define EXAMPLE_LVGL_PORT_AVOID_TEAR_MODE   3   //range 1 to 3

#define EXAMPLE_LVGL_PORT_ROTATION_DEGREE_ 0   // 0,90,180 or 270
//#define EXAMPLE_LVGL_PORT_PPA_ROTATION_ENABLE 1
#endif

#define BSP_MIPI_DSI_PHY_PWR_LDO_CHAN       (3)  // LDO_VO3 is connected to VDD_MIPI_DPHY
#define BSP_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV (2500)

#define BSP_LCD_H_RES                       (800)
#define BSP_LCD_V_RES                       (1280)

#define BSP_I2C_NUM                         (LVGL_IC_NUM)
#define BSP_I2C_SDA                         (GPIO_NUM_7)
#define BSP_I2C_SCL                         (GPIO_NUM_8)

#define BSP_LCD_TOUCH_RST                   (GPIO_NUM_22)
#define BSP_LCD_TOUCH_INT                   (GPIO_NUM_21)

#define BSP_LCD_RST                         (GPIO_NUM_9)

#define LCD_H_RES 800
#define LCD_V_RES 1280

#define LCD_RST 27
#define LCD_LED 23

#define TP_I2C_SDA (7)
#define TP_I2C_SCL (8)
#define TP_RST 22
#define TP_INT 21

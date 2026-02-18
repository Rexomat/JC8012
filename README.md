
Test Template for the GuitionP41C (I_W_Y) JC8012 ESP32P4 / C6 combination 10 inch LCD Display. Visual Studio / Platformio with Pioarduino is used.  
It might work for the similar Waveshare Device , i cant test that. 
It uses the new I2C funtion calls for the touch and works (only?) with newer Arduino/ESP-IDF Versions.
I had some struggles with the examples from manufacturer so i must make some changes to the I2C routines to run with newer ESP cores.

Preparation : Install Visual Studio Code , install inside VS the Platformio Plugin and PioArduino. Restart VS , open folder with this code. 
Let Platformio/Pioarduino some time to install all required software . 

Copy lv_conf.h to root of lvgl Library source directory when its downloaded. When using Platform IO lvgl is installed during build process. 
You will see errors. Copy the the lv_conf.h the to lvgl Directory in $(BUILD_DIR)/.pio/libdeps/esp32-p4/lvgl  
HAve Fun , implemented is a small Onscreen Keyboard for testing 

Greetz, Achim

When testing the Touch function and get no proper results i noticed that the 6Pin cable to the Touch was not proper connected. I reconnected it and the test was sucessful .
If u have similar effects , check the connection , it cost me 1 day.

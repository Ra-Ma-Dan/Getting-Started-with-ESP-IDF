#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "i2c_master.h"

#define SDA_PIN GPIO_NUM_17
#define SCL_PIN GPIO_NUM_18

void i2c_initialize(){
    i2c_
}

void main(){
    

    while(true){
        
        gpio_set_level(LED_PIN, led_state);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"



#define LED_PIN_NUM     GPIO_PIN_17



void main(){
    
    //configure the led pin as output
    gpio_config_t led_pin = {
        .pin_bit_mask = (1ULL << LED_PIN_NUM),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };
    gpio_config(&led_pin);

    while(true){
        gpio_set_level(LED_PIN_NUM, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(LED_PIN_NUM, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


01_Blink_LED/
├── CMakeLists.txt          (Project-level CMake settings)
├── sdkconfig.defaults      (Optional: Pre-configured settings)
└── main/
    ├── CMakeLists.txt      (Component-level CMake settings)
    └── main.c              (Your primary C code file)

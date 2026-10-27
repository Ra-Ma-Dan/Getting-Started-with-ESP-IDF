#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_17
#define BTN_PIN GPIO_NUM_18

bool led_state = false;

//ISR handler for when NEGEDGE of the btn(when btn goes LOW)
static void IRAM_ATTR btn_pin_hanler(void *pvParameters){
    led_state = !led_state;
}
void main(){
    //configure LED pin as output pin
    gpio_config_t led_pin = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type     = GPIO_INTR_DISABLE
    };
    gpio_config(&led_pin);

    //configure BTN pin as input pin
    gpio_config_t btn_pin = {
        .pin_bit_mask = (1ULL << BTN_PIN),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        intr_type     = GPIO_INTR_NEGEDGE
    };
    gpio_config(&btn_pin);
    gpio_insatll_isr_service(0);
    gpio_isr_handler_add(BTN_PIN, &btn_pin_hanler, NULL); //Add the function to handle btn pressed

    while(true){
        
        gpio_set_level(LED_PIN, led_state);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
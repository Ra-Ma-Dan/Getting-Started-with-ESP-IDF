//  Normal 4x4 matrix keypad over GPIO has 8 pins, 
// 4 col pins and 4 row pins.
// The way the device works is that, a loop is used to pull a row at a time,
// when a row is low, we loop throgh all the 4 colls and check which col is low at that time, which translate too, 
//                 1   2   3   A
//                 4   5   6   B
//                 7   8   9   C
//                 *   0   #   D

// key low at *col* at *row*

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"

//Collumn pins: Map with the ones connected to the real Keypad
#define COL_1_PIN GPIO_NUM_1
#define COL_2_PIN GPIO_NUM_2
#define COL_3_PIN GPIO_NUM_3
#define COL_4_PIN GPIO_NUM_4

//Row pins: Map with the ones connected to the real Keypad
#define ROW_1_PIN GPIO_NUM_1
#define ROW_2_PIN GPIO_NUM_2
#define ROW_3_PIN GPIO_NUM_3
#define ROW_4_PIN GPIO_NUM_4

#define ROWS    4
#define COLS    4

static const char *TAG = "KEYPAD";


gpio_num_t row_pins[ROWS] = {ROW_1_PIN, ROW_2_PIN, ROW_3_PIN, ROW_4_PIN};
gpio_num_t col_pins[COLS] = {COL_1_PIN, COL_2_PIN, COL_3_PIN, COL_4_PIN};

const char keypad_matrix[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


void keypad_pin_init(){

    gpio_config_t row_pins_cfg = {
        .pin_bit_mask = (1ULL << ROW_1_PIN) |
                        (1ULL << ROW_2_PIN) |
                        (1ULL << ROW_3_PIN) |
                        (1ULL << ROW_4_PIN),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&row_pins_cfg));

    for (int i = 0; i < ROWS; i++){
        gpio_set_level(row_pins[i], 1);
    }

    gpio_config_t col_pins_cfg = {
        .pin_bit_mask = (1ULL << COL_1_PIN) |
                        (1ULL << COL_2_PIN) |
                        (1ULL << COL_3_PIN) |
                        (1ULL << COL_4_PIN),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&col_pins_cfg));

    ESP_LOGI(TAG, "Keypad Initialized");

}

char read_key(){
    for(int i = 0; i < ROWS; i++){
        gpio_set_level(row_pins[i], 0);
        esp_rom_delay_us(10);
        
        for(int j = 0; j < COLS; j++){
            if(gpio_get_level(col_pins[j]) == 0){

                while (gpio_get_level(col_pins[j]) == 0) esp_rom_delay_us(10);
                
                gpio_set_level(row_pins[i], 1);
                
                return keypad_matrix[i][j];
            }
        }   
        gpio_set_level(row_pins[i], 1);
    }
    return '\0';
}

void keypad_task(){

    char key;

    while(true){
        key = read_key();
        if (key){
            printf("%c\n", key);
        }
        vTaskDelay(20);
    }
}

void main(){

    keypad_pin_init();
    ESP_LOGI(TAG, "Started");

    xTaskCreate(keypad_task, "Reading_keypad", 2048, NULL, 8, NULL);
}
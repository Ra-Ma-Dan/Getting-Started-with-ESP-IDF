#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "i2c_master.h"

#define SDA_PIN             GPIO_NUM_17
#define SCL_PIN             GPIO_NUM_18
#define I2C_CLK_SPEED       400000 //i2c high clock speed 
#define I2C_PORT_NUM        0

#define DEVICE_ADDRE        0x89 //address of the target device

static const char* TAG = "I2C";

void i2c_initialize(){
    i2c_config_t master_cfg = {
        .clk_source                   = I2C_CLK_SOURCE_DEFAULT,
        .sda_io_num                   = SDA_PIN,
        .scl_io_num                   = SCL_PIN,
        .i2c_port                     = TEST_I2C_PORT,
        .glith_ignore_cnt             = 7,
        .flags.enable_internal_pullup = true
    };
    i2c_master_bus_handle_t master_bus;
    ESP_ERROR_CHECK(i2c_new_master_bus(&master_bus, &master_cfg));

    i2c_master_dev_config_t dev_cfg = {
        .dev_addr_length = I2C_DEV_ADDR_BIT_LEN_7,
        .clk_speed_hz    = I2C_CLK_SPEED,
        .device_address  = DEVICE_ADDRE
    };
    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(master_bus, &dev_handle, &dev_cfg));
    ESP_LOGI(TAG, "i2c initialized");
}

void transmit_data(uint16_t data, size_t data_size, int timeout){
    esp_err_t res = i2c_master_transmit(dev_handle, data, data_size, timeout);
    if(res != ESP_OK) {
        ESP_LOGI(TAG, "Unable to transmit data: %s", esp_err_to_name(res));
    }
}

void recieve_data(uint16_t data, size_t data_size, int timeout){
    esp_err_t res = i2c_master_recieve(dev_handle, data, data_size, timeout);
    if(res != ESP_OK) {
        ESP_LOGI(TAG, "Unable to read data: %s", esp_err_to_name(res));
    }
}


void transmit_recieve(uint16_t data, size_t data_size, uint16_t buffer, size_t buf_size, int timeout){
    esp_err_t res = i2c_master_transmit(dev_handle, data, data_size, buffer, buf_size, timeout);

    if(res != ESP_OK) {
        ESP_LOGI(TAG, "Unable to trancieve data: %s", esp_err_to_name(res));
    }
}


void main(){
    i2c_initialize();

    while(true){
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
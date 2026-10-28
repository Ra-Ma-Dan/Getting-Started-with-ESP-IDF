#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"


#define MAX_NETWORK     10

static const char *TAG = "Wifi Station scan";

void init_wifi_station(){

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();
    assert(netif);

    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init);


    
    esp_wifi_set_mode(WIFI_MODE_STA);


    ESP_CHECK_ERROR(esp_wifi_start());
    //Finished initialization
}


void scan_for_network(){
    while(true){
        ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
        uint16_t num_of_networks = 0;
        wifi_ap_record_t records[MAX_NETWORK];

        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&num_of_networks));
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&num_of_networks, records));

        num_of_networks = (num_of_networks <= MAX_NETWORK) ? num_of_networks : MAX_NETWORK;

        ESP_LOGI(TAG, "%d networks found: ", num_of_networks);
        for(int i = 0; i < num_of_networks; i++){
            ESP_LOGI(TAG, "Name: %s, MAC: %d, at channel: %d", records[1].ssid, records[1].rssi, records[1].primary);
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void main(){
    esp_err_t res = nvs_flash_init();
    if (res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND){
        nvs_flash_erase();
        res = nvs_flash_init();
    }
    ESP_ERROR_CHECH(res);

    init_wifi_station();

    xTaskCreate(scan_networks, "Scan_for_APs", 2048, NULL, 8, NULL);

}
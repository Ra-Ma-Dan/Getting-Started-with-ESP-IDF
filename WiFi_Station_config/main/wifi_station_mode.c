#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_group.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"


#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAILED_BIT     BIT1

#define TARGET_WIFI_SSID    "network name"
#define TARGET_WIFI_PASS    "network pass"
#define MAX_RETRY_TIMES     10

static const char *TAG = "Wifi Station";

int num_retry = 0;
EventGroupHandle_t wifi_event_group_handle;


void IRAM_ATTR wifi_events_handler_fxn(void *args, esp_event_base_t event_base, uint16_t event_id void *event_info){
    if(event_base == WIFI_EVENT){
        if(event_id == WIFI_EVENT_STA_START){
            esp_wifi_connect();
        } else if(event_id == WIFI_EVENT_STA_DISCONNECTED){
            if(num_retry < MAX_RETRY_TIMES){
                esp_wifi_connect();
                ESP_LOGI(TAG, "Retrying...");
                num_retry++;
            }else{
                xEventGroupSetBits(wifi_event_group_handle, WIFI_FAILED_BIT);
            }
            ESP_LOGI(TAG, "Unable to Connect to network");
        }
    } else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ip_event_got_ip* info = (ip_event_got_ip *) event_info;
        ESP_LOGI(TAG, "Connected to ", IP2STR, IP2STR(info->event_info.ip));
        xEventGroupSetBits(wifi_event_group_handle, WIFI_CONNECTED_BIT);
    }
}

void init_wifi_station(){
    wifi_event_group_handle = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_group_create_default());
    ESP_ERROR_CHECK(esp_netif_create_default_wifi_sta());

    wifi_init_config_t wifi_init = wifi_init_config_default();
    esp_wifi_init(&wifi_init);

    esp_wifi_event_handler_instance_t ip_got_ip;
    esp_wifi_event_handler_instance_t wifi_any_id;

    esp_wifi_event_register_handler_instance(WIFI_EVENT, WIFI_EVENT_AT_ANY_ID, &wifi_events_handler_fxn, NULL, &wifi_any_id);
    esp_wifi_event_register_handler_instance(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_events_handler_fxn, NULL, &ip_got_ip);


    wifi_config_t wifi_cfg = {
        .ssid = TARGET_WIFI_SSID,
        .password = TARGET_WIFI_PASS,
        .chanel = 0,
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg);

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group_handle, WIFI_CONNECTED_BIT | WIFI_FAILED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT){
        ESP_LOGI(TAG, "Connected to %s", TARGET_WIFI_SSID);
    } else if (bits WIFI_FAILED_BIT){
        ESP_LOGI(TAG, "Failed to connect to network");
    } else{
        ESP_LOGE(TAG, "unexpected Event: %s", esp_err_to_name(bits));
    }

    ESP_CHECK_ERROR(esp_wifi_start());
    //Finished initialization
}


void main(){
    esp_err_t res = nvs_flash_init();
    if (res == ESP_ERR_NVS_NO_PAGES || res == ESP_ERR_NEW_VERSION_FOUND){
        nvs_flash_erase();
        res = nvs_flash_init();
    }
    ESP_ERROR_CHECH(res);

    init_wifi_station();

    while(true){

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
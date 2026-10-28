#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_netif.h"
#include "esp_mac.h"

#define AP_STADISCONNECTED_BIT         BIT0
#define AP_STACONNECTED_BIT      BIT1

#define NETWORK_NAME             "network name"
#define NETWORK_PASS             "network pass"
#define MAX_DEVICE               10
#define NETWORK_CHANNEL          1
#define NETWORK_AUTH_MODE        WIFI_AUTH_WPA2_PSK


static const char *TAG = "Wifi AP";

EventGroupHandle_t ap_event_group_handle;


void IRAM_ATTR wifi_events_handler_fxn(void *args, esp_event_base_t event_base, int16_t event_id void *event_info){
    if(event_base != WIFI_EVENT){
        return;
    }

    if(event_id == WIFI_EVENT_AP_START){
        ESP_LOGI(TAG, "WiFi Started, broadcasting....")
    } else if(event_id == WIFI_EVENT_AP_STOP){
        ESP_LOGI(TAG, "Soft-Ap Stopped");
    }else if(event_id == WIFI_EVENT_AP_STACONNECTED){
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t *) event_info;
        ESP_LOGI(TAG, "Client joinned" MACSTR ", AID: %d", IP2STR(event->mac), event->aid);
        xEventGroupSetBits(ap_event_group_handle, AP_STACONNECTED_BIT);
    }else if(event_id == WIFI_EVENT_AP_STADISCONNECTED){
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t *) event_info;
        ESP_LOGI(TAG, "Client left " IP2STR ", AID: %d", IP2STR(event->mac), event->aid);
        xEventGroupSetBits(ap_event_group_handle, AP_STADISCONNECTED_BIT);
        xEventGroupClearBits(ap_event_group_handle, AP_STACONNECTED_BIT);
    }
}

void IRAM_ATTR ip_events_handler_fxn(void *args, esp_event_base_t event_base, int16_t event_id void *event_info){

    if (event_base == IP_EVENT && event_id == IP_EVENT_AP_STAIPASSIGNED) {
        ip_event_ap_staipassigned_t* event = (ip_event_ap_staipassigned_t*) event_info;
        ESP_LOGI(TAG, "DHCP assigned IP " IPSTR " to client MAC: " MACSTR, IP2STR(&event->ip), MAC2STR(event->mac));
    }
}

void init_wifi_softap(){
    ap_event_group_handle = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_group_create_default());
    esp_net_t *ap_netif = esp_netif_create_default_wifi_ap();
    assert(ap_netif);
    //if(ap_netif == NULL) ESP_LOGI(TAG, "Failed to create AP.");

    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init);

    esp_wifi_event_handler_instance_t ip_got_ip;
    esp_wifi_event_handler_instance_t wifi_any_id;

    esp_wifi_event_handler_instance_register(WIFI_EVENT, 
                                            ESP_EVENT_ANY_ID, 
                                            &wifi_events_handler_fxn, 
                                            NULL, 
                                            &wifi_any_id);
    esp_wifi_event_handler_instance_register(IP_EVENT, 
                                            P_EVENT_AP_STAIPASSIGNED, 
                                            &ip_events_handler_fxn, 
                                            NULL, 
                                            &ip_got_ip);


    wifi_config_t wifi_cfg = {
        .ip = {
            .ssid = NETWORK_NAME,
            .password = NETWORK_PASS,
            .chanel = NETWORK_CHANNEL,
            .athmode = NETWORK_AUTH_MODE,
            .max_connection = MAX_DEVICE
        }
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_cfg);

    EventBits_t bits = xEventGroupWaitBits(ap_event_group_handle, 
                                            AP_STACONNECTED_BIT | AP_STADISCONNECTED_BIT, 
                                            pdFALSE, 
                                            pdFALSE, 
                                            portMAX_DELAY);
    if (bits & AP_STACONNECTED_BIT){
        ESP_LOGI(TAG, "Client Connected to to network");
    } else if (bits & AP_STADISCONNECTED_BIT){
        ESP_LOGI(TAG, "Client disconnected from network");
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

    init_wifi_softap();

    while(true){
        printf("Broadcasting...\n");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
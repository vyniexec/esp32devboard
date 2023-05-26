#include <stdio.h>
#include "includes.h"
#include "geral.h"

unsigned char *buffer[10];

void saidas( void ){
    int tmp;
    for(tmp = 0; tmp < 8; tmp++){
        gpio_set_level(IO_WR, 1);
        gpio_set_level(IO_CLK, 1);
        gpio_set_level(IO_CLK, 0);
    }
    gpio_set_level(IO_WR, 0);
    gpio_set_level(IO_SH, 1);
    gpio_set_level(IO_SH, 0);
}

// -- Função do código principal --;
void app_main(void)
{
    /* == Inicializando o NVS que armazena configurações do ESP32 == */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    connect_wifi();
    mqtt_app_start();
    devboardInit();
    vTaskDelay(10);
    lcdInit();
    lcdWrite(0, 2, "ESP DEVBOARD ");
    xTaskCreate(pubMessage, "Publica mensagem", 1024 * 5, NULL, 5, NULL);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
}
#ifndef __MQTT__H__
#define __MQTT__H__

#include <stdio.h>
#include "esp_log.h"
#include "includes.h"
#include "geral.h"

/* == Criando a TAG1 de sinalização == */
static const char *TAG1 = "MQTTZINHO";
/* == Criando a TAG de sinalização == */
uint32_t MQTT_CONNEECTED = 0;

/* == Chamando a função MQTT == */
static void mqtt_app_start(void);
int test1 = 1;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG1, "Desapacho da base do loop de eventos=%s, evento=%d", base, (int)event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG1, "MQTT CONECTADO COM SUCESSO ");
        lcdClear();
        vTaskDelay(500);
        lcdWrite(1, 0, "MQTT CONECTADO ");
        MQTT_CONNEECTED=1;
        msg_id = esp_mqtt_client_subscribe(client, "/testVyni/read", 2);
        ESP_LOGI(TAG1, "Mensagem enviada com sucesso, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG1, "MQTT DESCONECTADO");
        lcdClear();
        vTaskDelay(500);
        lcdWrite(1, 0, "MQTT DESCONECTADO ");
        MQTT_CONNEECTED=0;
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG1, "MENSAGEM ENVIADA, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG1, "Evento cancelado, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG1, "Mensagem publicada, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        char *recebido = "";
        lcdClear();
        ESP_LOGI(TAG1, "MENSAGEM RECEBIDA=%.*s\r\n", event->data_len, event->data);
        recebido = event->data;
        lcdWrite(0, 2, "MSG RECEBIDA ");
        lcdWrite(1, 4, recebido);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG1, "MQTT ERRO!!!");
        break;
    default:
        ESP_LOGI(TAG1, "Outro evento:%d", event->event_id);
        break;
    }
}

esp_mqtt_client_handle_t client = NULL;
static void mqtt_app_start(void)
{
    ESP_LOGI(TAG1, "Iniciando MQTT");
    esp_mqtt_client_config_t mqttConfig = {
        .broker.address.uri = "mqtt://192.168.1.112",
        .broker.address.port = 1883,
        .credentials.username = "vini2131",
        .credentials.authentication.password = "vini2131",
    };
    client = esp_mqtt_client_init(&mqttConfig);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void pubMessage(void *params)
{
  while (true)
  {
    if(MQTT_CONNEECTED){
        if(test1 == 1){
            esp_mqtt_client_publish(client, "test/", "mensagem", 0, 0, 0);
            vTaskDelay(500);
        }else ESP_LOGW(TAG1, "test1 = 0");
    }
  }
}
#endif  //!__MQTT__H__
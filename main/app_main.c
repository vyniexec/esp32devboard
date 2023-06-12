#include <stdio.h>
#include <string.h>
#include "includes.h"
#include "geral.h"

unsigned char *buffer[10];

char letras[10][4] = {
  {'0', ' ', ' ', ' '}, // Tecla 0 não tem letras
  {'1', 'A', 'B', 'C'}, // Tecla 1 tem as letras A, B e C
  {'2', 'D', 'E', 'F'}, // Tecla 2 tem as letras D, E e F
  {'3', 'G', 'H', 'I'}, // Tecla 3 tem as letras G, H e I
  {'4', 'J', 'K', 'L'}, // Tecla 4 tem as letras J, K e L
  {'5', 'M', 'N', 'O'}, // Tecla 5 tem as letras M, N e O
  {'6', 'P', 'Q', 'R'}, // Tecla 6 tem as letras P, Q e R
  {'7', 'S', 'T', 'U'}, // Tecla 7 tem as letras S, T e U
  {'8', 'V', 'W', 'X'}, // Tecla 8 tem as letras V, W e X
  {'9', 'Y', 'Z', '*'}  // Tecla 9 tem as letras Y, Z e *
};

// Define uma variável para armazenar a última tecla pressionada
char lastKey;

// Define uma variável para armazenar o índice da letra atual
int letterIndex = -1;

// Define uma variável para armazenar a tecla pressionada
char tec;

void keyboardFull(void){
    tec = teclado();
    printf("%c \n", tec);
    if(pressionado != '-')                                          // Se alguma tecla for pressionada...
    {
        printf("Tecla pressionada \n");
        if (tec == lastKey){                                        // Se a mesma tecla foi pressionada novamente
            letterIndex++;                                          // Incrementa o índice da letra
            printf("Incrementei: %d \n", letterIndex);
        }
        if (letterIndex > 3){                                       // Se o índice ultrapassou o limite da matriz
            letterIndex = -1;                                       // Reseta o índice
            printf("Resetei o indice \n");                          // Imprime o número
        } else {                                                    // Se o índice ainda está dentro do limite da matriz
            printf("Tecla: %c \n", letras[tec - '0'][letterIndex]); // Imprime a letra correspondente ao índice
            printf("\b");                                           // Apaga o último caractere impresso
        }
    } else {                                                        // Se uma tecla diferente foi pressionada
        letterIndex = -1;                                           // Reseta o índice
    }
    lastKey = tec;                                                  // Atualiza a última tecla pressionada
    vTaskDelay(10);                                                 // Aguarda meio segundo para evitar leituras repetidas
}

void keyboard1(void)
{
    while(1)
    {
        keyboardFull();
    }
}

// -- Função do código principal --;
void app_main(void)
{
    /* == Inicializando o NVS que armazena configurações do ESP32 == */
    // esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //   ESP_ERROR_CHECK(nvs_flash_erase());
    //   ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);
    // connect_wifi();
    // mqtt_app_start();
    devboardInit();
    vTaskDelay(10);
    lcdInit();
    lcdWrite(0, 2, "ESP DEVBOARD ");
    xTaskCreate(&keyboard1, "Leitura do teclado", 2048, NULL, 2, NULL);
    // xTaskCreate(pubMessage, "Publica mensagem", 1024 * 5, NULL, 5, NULL);
    // esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
}
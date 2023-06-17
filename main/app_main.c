#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "includes.h"
#include "geral.h"


char letras[10][4] = {                                                  // Criando a matriz alphanumerica
  {'0', ' ', ' ', ' '},                                                 // Tecla 0 não tem letras
  {'1', 'A', 'B', 'C'},                                                 // Tecla 1 tem as letras A, B e C
  {'2', 'D', 'E', 'F'},                                                 // Tecla 2 tem as letras D, E e F
  {'3', 'G', 'H', 'I'},                                                 // Tecla 3 tem as letras G, H e I
  {'4', 'J', 'K', 'L'},                                                 // Tecla 4 tem as letras J, K e L
  {'5', 'M', 'N', 'O'},                                                 // Tecla 5 tem as letras M, N e O
  {'6', 'P', 'Q', 'R'},                                                 // Tecla 6 tem as letras P, Q e R
  {'7', 'S', 'T', 'U'},                                                 // Tecla 7 tem as letras S, T e U
  {'8', 'V', 'W', 'X'},                                                 // Tecla 8 tem as letras V, W e X
  {'9', 'Y', 'Z', '*'}                                                  // Tecla 9 tem as letras Y, Z e *
};

// Definição de variáveis globais
bool maisculo = true;                                                   // Variável de verificação se é maisculo ou não
char lastKey = '-';                                                     // Variável para armazena a ultima tecla pressionada
char tec;                                                               // Variável para ler a tecla pressionada
char keyFull;                                                           // Variável para armazena tecla do indice maisculo
int letterIndex;                                                        // Variável para incrementar o indice
unsigned char *buffer1[10];                                             // Variável para armazenar a tecla maisculo para o LCD 
unsigned char *buffer3[10];                                             // Variável para armazenar a tecla minuscula para o LCD

void keyboardFull(void){
    tec = teclado();                                                    // Chamando a função do teclado
    if(coluna != -1){                                                   // Se alguma tecla for pressionada...
        if(tecla == 'A'){                                               // Se a tecla pressionada for 'A'
            maisculo = !maisculo;                                       // Invertendo o valor de maisculo
        }
        //printf("Tecla pressionada \n");                               // Imprimi a tecla pressionada
        if (tecla == lastKey){                                          // Se a mesma tecla foi pressionada novamente
            letterIndex++;                                              // Incrementa o índice da letra
        }else letterIndex = 0;                                          // Reseta o índice
        if (letterIndex > 3){                                           // Se o índice ultrapassou o limite da matriz
            letterIndex = 0;                                            // Reseta o índice
            keyFull = letras[tecla - '0'][letterIndex];                 // Armazena na variavel a letra correspondente ao índice
            printf("Tecla: %c \n", letras[tecla - '0'][letterIndex]);   // Imprime a letra correspondente ao índice
            sprintf(buffer1, "Tecla: %c ", keyFull);                    // Armazena a letra correspondente ao índice no buffer
            lcdWrite(1, 0, buffer1);                                    // Escreve no display a tecla correspondente ao índice
        } else if(maisculo == true){                                    // Se o índice ainda está dentro do limite da matriz
            keyFull = letras[tecla - '0'][letterIndex];                 // Armazena na variavel a letra correspondente ao índice
            printf("Tecla: %c \n", letras[tecla - '0'][letterIndex]);   // Imprime a letra correspondente ao índice
            sprintf(buffer1, "Tecla: %c ", keyFull);                    // Armazena a letra correspondente ao índice no buffer
            lcdWrite(1, 0, buffer1);                                    // Escreve no display a tecla correspondente ao índice
            printf("\b");                                               // Apaga o último caractere impresso
        }else {
            if(letterIndex != 0){
                keyFull = letras[tecla - '0'][letterIndex] + 0x20;      // Armazena na variavel a letra correspondente ao índice
                printf("Tecla: %c \n", letras[tecla - '0'][letterIndex] + 0x20);// Imprime a letra correspondente ao índice
                sprintf(buffer3, "Tecla: %c ", keyFull);                // Armazena a letra correspondente ao índice no buffer
                lcdWrite(1, 0, buffer3);                                // Escreve no display a tecla correspondente ao índice
                printf("\b");                                           // Apaga o último caractere impresso
            }else {
                keyFull = letras[tecla - '0'][letterIndex];             // Armazena na variavel a letra correspondente ao índice
                printf("Tecla: %c \n", letras[tecla - '0'][letterIndex]);// Imprime a letra correspondente ao índice
                sprintf(buffer3, "Tecla: %c ", keyFull);                // Armazena a letra correspondente ao índice no buffer
                lcdWrite(1, 0, buffer3);                                // Escreve no display a tecla correspondente ao índice
                printf("\b");                                           // Apaga o último caractere impresso
            }
        }
    }
    if(tecla != lastKey) lastKey = tecla;                               // Atualiza a última tecla pressionada
}

void keyboard1(void)
{
    while(1)
    {
        keyboardFull();                                                 // Chamando a função do teclado alphanumerico
        vTaskDelay(10);                                                 // Aguarda 10ms para evitar leituras repetidas
    }
}


// -- Função do código principal --;
void app_main(void)
{
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
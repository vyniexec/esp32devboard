#include <stdio.h>
#include <string.h>
#include "includes.h"
#include "geral.h"
#define CONFIG_FREERTOS_HZ 100

unsigned char *buffer[10];

void machineState(void)
{
    int state = 0;
    switch ( state )
    {
    case 0:                                 // Stand-by
        lcdClear();
        lcdWrite(0,1,"STAND-BY ");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        state = 1;
    case 1:                                 // Enchendo
        lcdClear();
        lcdWrite(0,1,"ENCHENDO ");
        io_le_escreve(0b00001000);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        state = 2;
    case 2:                                 // Batendo
        lcdClear();
        lcdWrite(0,1,"BATENDO ");
        for (int i = 3; i >= 0; i--)
        {
            io_le_escreve(0b00000001);          // bomba saida
            vTaskDelay(20 / portTICK_PERIOD_MS);
            io_le_escreve(0b00000010);          // bomba saida
        }
        state = 3;
    case 3:                                 // Esvaziando
        lcdClear();
        lcdWrite(0,1,"ESVAZIANDO ");
        io_le_escreve(0b00000100);          // bomba saida
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        state = 4;
    case 4:                                 // Enxaguando
        lcdClear();
        lcdWrite(0,1,"ENXAGUANDO ");
        for (int i = 3; i >= 0; i--)
        {
            io_le_escreve(0b00000001);      // bomba saida
            vTaskDelay(10 / portTICK_PERIOD_MS);
            io_le_escreve(0b00000010);      // bomba saida
        }
        lcdWrite(1,0, "esvaziando ");
        io_le_escreve(0b00000100);          // bomba entrada
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        state = 5;
    case 5:                                 // Centrifugando
        lcdClear();
        lcdWrite(0,1,"CENTRIFUGANDO ");
        io_le_escreve(0b00000100);         // ligar saida e motor horariio
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        state = 0;
        lcdClear();
    default:
        break;
    }
}

void app_main( void )
{
    devboardInit();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    lcdInit();
    lcdClear();
    machineState();
}

#include <stdio.h>
#include <string.h>
#include "includes.h"
#include "geral.h"

unsigned char *buffer[10];
int nivelBaixo, nivelAlto, fimCurso, inputs = 0;

void outAndInput(int saida)
{
    inputs = io_le_escreve(saida);
    nivelAlto = (inputs>>7) & 1;
    nivelBaixo = (inputs>>6) & 1;
    fimCurso = (inputs>>5) & 1;
    vTaskDelay(10);
}

int state = 0;
void machineState(void)
{
    
    switch ( state )
    {
    case 0:                                 // Stand-by
        lcdClear();
        lcdWrite(0,1,"STAND-BY ");
        vTaskDelay(100);
        outAndInput(0x00);
        if(fimCurso == 1)state = 1;
        break;
    case 1:                                 // Enchendo
        lcdClear();
        lcdWrite(0,1,"ENCHENDO ");
        outAndInput(0b00001000);
        outAndInput(0x00);
        vTaskDelay(500);
        outAndInput(0B10000000);
        vTaskDelay(500);
        outAndInput(0B11000000);
        vTaskDelay(500);
        outAndInput(0B11100000);
        vTaskDelay(500);
        outAndInput(0B11110000);
        vTaskDelay(100);
        if(nivelAlto == 1)state = 2;
        printf("estado: %d \n", state);
        break;
    case 2:                                 // Batendo
        lcdClear();
        lcdWrite(0,1,"BATENDO ");
        outAndInput(0x00);
        if(nivelAlto == 1){
            for (int i = 7; i >= 0; i--)
            {
                outAndInput(0b00000001);          // bomba saida
                vTaskDelay(100 / portTICK_PERIOD_MS);
                outAndInput(0b00000010);          // bomba saida
            }
            vTaskDelay(100);
            state = 3;
        }
        break;
    case 3:                                 // Esvaziando
        lcdClear();
        lcdWrite(0,1,"ESVAZIANDO ");
        outAndInput(0b00000100);          // bomba saida
        if(nivelAlto == 0)state = 4;
        break;
    case 4:                                 // Enxaguando
        lcdClear();
        lcdWrite(0,1,"ENXAGUANDO ");
        outAndInput(0x00);
        vTaskDelay(500);
        outAndInput(0B10000000);
        vTaskDelay(500);
        outAndInput(0B11000000);
        vTaskDelay(500);
        outAndInput(0B11100000);
        vTaskDelay(500);
        outAndInput(0B11110000);
        if(nivelAlto == 1){
            for (int i = 7; i >= 0; i--)
            {
                outAndInput(0b11110001);      // bomba saida
                vTaskDelay(50 / portTICK_PERIOD_MS);
                outAndInput(0b11110010);      // bomba saida
            }
            state = 5;
        }
        break;
    case 5:
        lcdClear();
        lcdWrite(0,1,"CENTRIFUGANDO ");
        for (int i = 50; i >= 0; i--)
        {
            outAndInput(0b00000100);         // ligar saida e motor horariio
            vTaskDelay(100);
        }
        if(nivelAlto == 0) state = 0;
    }
}

void sla(void){
    while(1){
        //outAndInput();
        machineState();
        vTaskDelay(10);
    }
}

void app_main( void )
{
    devboardInit();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    lcdInit();
    lcdClear();
    xTaskCreate(&sla, "Leitura", 2048, NULL, 2, NULL);
}

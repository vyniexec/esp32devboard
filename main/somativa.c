#include <stdio.h>
#include <string.h>
#include "includes.h"
#include "geral.h"

unsigned char *buffer2[10];
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
    case 0:
        lcdClear();
        lcdWrite(0, 0,"STAND-BY ");
        vTaskDelay(100);
        outAndInput(0x00);
        if(fimCurso == 1)state = 6;
        break;
    case 6:
        lcdClear();
        lcdWrite(0, 0, "DE MOLHO ");
        for(int i = 60; i >= 0; i--){
            sprintf(buffer2, "TEMPO: %d ", i);
            lcdWrite(1, 0, buffer2);
            vTaskDelay(1);
        }
        state = 1;
        break;
    case 1:
        lcdClear();
        lcdWrite(0, 0,"ENCHENDO ");
        outAndInput(0x00);
        if(nivelAlto == 1){
            outAndInput(0x80);
            vTaskDelay(10);
            outAndInput(0xC0);
            vTaskDelay(10);
            outAndInput(0xE0);
            vTaskDelay(10);
            outAndInput(0xF0);
            state = 2;
        }
        break;
    case 2:
        outAndInput(0xF0);
        lcdClear();
        lcdWrite(0, 0,"BATENDO ");
        if(nivelAlto == 1){
            for (int i = 7; i >= 0; i--)
            {
                outAndInput(0b11110001);
                vTaskDelay(10);
                outAndInput(0b11110010);
            }
            vTaskDelay(100);
            state = 3;
        }
        break;
    case 3:
        lcdClear();
        lcdWrite(0, 0,"ESVAZIANDO ");
        if(nivelAlto == 0){
            outAndInput(0xE0);
            vTaskDelay(10);
            outAndInput(0xC0);
            vTaskDelay(10);
            outAndInput(0x80);
            vTaskDelay(10);
            outAndInput(0x00);
            vTaskDelay(10);
            state = 4;
        }
        break;
    case 4:
        lcdClear();
        lcdWrite(0, 0,"ENXAGUANDO ");
        outAndInput(0x00);
        if(nivelAlto == 1){
            outAndInput(0x80);
            vTaskDelay(10);
            outAndInput(0xC0);
            vTaskDelay(10);
            outAndInput(0xE0);
            vTaskDelay(10);
            outAndInput(0xF0);
            vTaskDelay(100);
            vTaskDelay(100);
            outAndInput(0xE0);
            vTaskDelay(10);
            outAndInput(0xC0);
            vTaskDelay(10);
            outAndInput(0x80);
            vTaskDelay(10);
            outAndInput(0x00);
            state = 5;
        }
        break;
    case 5:
        lcdClear();
        outAndInput(0x00);
        lcdWrite(0, 0,"CENTRIFUGANDO ");
        if(nivelAlto == 0){
            for (int i = 10; i >= 0; i--)
            {
                outAndInput(0b00000010);
                vTaskDelay(10);
            }
            state = 0;
            break;
        }
    }
}

void machinaClean(void){
    while(1){
        machineState();
        vTaskDelay(10);
    }
}

void app_main( void )
{
    devboardInit();
    vTaskDelay(10);
    lcdInit();
    lcdClear();
    xTaskCreate(&machinaClean, "Leitura", 2048, NULL, 2, NULL);
}

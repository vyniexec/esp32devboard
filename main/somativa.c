#include <stdio.h>
#include <string.h>
#include "includes.h"
#include "geral.h"

unsigned char *buffer[10];

int nivelBaixo, nivelAlto = 0;

void machineState(void)
{
    int state = 0;
    switch ( state )
    {
    case 0:                                 // Stand-by
        lcdClear();
        lcdWrite(0,1,"STAND-BY ");
        if(tecla == '1') state = 1;
        else while(1){}
    case 1:                                 // Enchendo
        lcdClear();
        lcdWrite(0,1,"ENCHENDO ");
        io_le_escreve(0b00001000);
        if(nivelBaixo == 1 && nivelAlto == 1) state = 2;
        else while(1){}
    case 2:                                 // Batendo
        lcdClear();
        lcdWrite(0,1,"BATENDO ");
        if(nivelBaixo == 1 && nivelAlto == 1){
            for (int i = 7; i >= 0; i--)
            {
                io_le_escreve(0b00000001);          // bomba saida
                vTaskDelay(100 / portTICK_PERIOD_MS);
                io_le_escreve(0b00000010);          // bomba saida
            }
            state = 3;
        }
        else while(1){}
    case 3:                                 // Esvaziando
        lcdClear();
        lcdWrite(0,1,"ESVAZIANDO ");
        io_le_escreve(0b00000100);          // bomba saida
        while(nivelBaixo == 1 && nivelAlto == 1){}
        state = 4;
    case 4:                                 // Enxaguando
        lcdClear();
        lcdWrite(0,1,"ENXAGUANDO ");
        if(nivelBaixo == 1 && nivelAlto == 1){
            for (int i = 7; i >= 0; i--)
            {
                io_le_escreve(0b00000001);      // bomba saida
                vTaskDelay(50 / portTICK_PERIOD_MS);
                io_le_escreve(0b00000010);      // bomba saida
            }
        }else while(1){}
        lcdClear();
        lcdWrite(0,1,"CENTRIFUGANDO ");
        io_le_escreve(0b00000100);         // ligar saida e motor horariio
        if(nivelBaixo == 0 && nivelAlto == 0) state = 0;
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

#ifndef __KEYBOARD__H__
#define __KEYBOARD__H__

#include <stdio.h>
#include "includes.h"
#include "geral.h"

// -- Defininições de variaveis --;
int coluna, linha, linha2, saidinha = 0;
char tecla = '-';
char pressionado = '-';
static const char *TAG = "@vynizinho";

// -- Definindo os valores das teclas --;
const char teclas[4][4] =
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// -- Criando a função do teclado matricial 4x4 --;
void setlinha( void ){
    if( linha <= 1 ) linha = 0b1000;
    else             linha >>= 1;
}
char teclado(void){
    int i;
    gpio_set_level(Teclado_SH, 1);
    coluna = 0;
    for( i = 7; i >= 0; i--){
        if(i < 4){
            if(gpio_get_level(Teclado_RD)== 1){
                pressionado = '1';
                coluna = i;
                if(linha == 1) linha2 = 2;
                if(linha == 2) linha2 = 3;
                if(linha == 4) linha2 = 0;
                if(linha == 8) linha2 = 1;
                tecla = teclas[linha2][coluna];
                ESP_LOGI(TAG, "Captura tecla: %c", tecla);
            }else
            {
                pressionado = '-';
            }
        }
        gpio_set_level(Teclado_WR, (linha >> i) & 1);
        gpio_set_level(Teclado_CLK, 1);
        gpio_set_level(Teclado_CLK, 0);
    }
    gpio_set_level(Teclado_SH, 0);
    setlinha();
    return tecla;
}

#endif  //!__KEYBOARD__H__
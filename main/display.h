#include <stdio.h>
#include "includes.h"
#include "geral.h"

/* ========================================================================= */
/* --- Defines --- */
#define lcdRows          2
#define lcdColumn        16
#define lcdSetCursor( adrs ) (0x80+(adrs & 0x7F))
#define lcdLine0         0x00
#define lcdLine1         0x40

/* ========================================================================= */
/* --- Prototipo das Funcoes --- */
void espInit(void);
void lcdInit(void);
void lcdClear(void);
void sendLCDCommand(unsigned char cmd);
void lcdCursor(unsigned char lin, unsigned char col);
void sendLCDData(unsigned char data);
void lcdWrite(unsigned char lin, unsigned char col, const char *dataSend);
void pulseEnable(unsigned char byte);

/* ========================================================================= */
/* --- Envia os enable em comando para o LCD --- */
void espInit(void){
    gpio_reset_pin(LCD_CLK);
    gpio_reset_pin(LCD_WR);
    gpio_reset_pin(LCD_SH);
    esp_rom_gpio_pad_select_gpio(LCD_CLK);
    esp_rom_gpio_pad_select_gpio(LCD_SH);
    esp_rom_gpio_pad_select_gpio(LCD_WR);
    gpio_set_direction(LCD_CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_SH, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_WR, GPIO_MODE_OUTPUT);
    gpio_set_level(LCD_CLK, 0);
    gpio_set_level(LCD_WR, 0);
    gpio_set_level(LCD_SH, 0);
    int tmp;
    for(tmp = 7; tmp >= 0; tmp--){
        gpio_set_level(LCD_WR, 0);    
        gpio_set_level(LCD_CLK, 1);    
        gpio_set_level(LCD_CLK, 0);    
    }
    gpio_set_level(LCD_SH, 1);
    gpio_set_level(LCD_SH, 0);
}

/* ========================================================================= */
/* --- Envia os enable em comando para o LCD --- */
void pulseEnable(unsigned char byte){
  int tmp;
  gpio_set_level(LCD_SH, 1);
  for(tmp = 7; tmp >= 0; tmp--){
    gpio_set_level(LCD_WR, ((byte | 0x08) >> tmp) & 0x01);
    gpio_set_level(LCD_CLK, 1);
    gpio_set_level(LCD_CLK, 0);
  }
  gpio_set_level(LCD_SH, 0);
  gpio_set_level(LCD_SH, 1);
  for(tmp = 7; tmp >= 0; tmp--){
    gpio_set_level(LCD_WR, ((byte & 0xF7) >> tmp) & 0x01);
    gpio_set_level(LCD_CLK, 1);
    gpio_set_level(LCD_CLK, 0);
  }
  gpio_set_level(LCD_SH, 0);
}

/* ========================================================================= */
/* --- Envia os comandos para o LCD --- */
void sendLCDCommand(unsigned char cmd)
{
  unsigned char cmd_high = (cmd & 0xF0);
  unsigned char cmd_low = (cmd << 4);
  int tmp;
  gpio_set_level(LCD_SH, 1);
  for(tmp = 7; tmp >= 0; tmp--){
    gpio_set_level(LCD_WR, (cmd_high >> tmp) & 0x01);
    gpio_set_level(LCD_CLK, 1);
    gpio_set_level(LCD_CLK, 0);
  }
  vTaskDelay(2);
  gpio_set_level(LCD_SH, 0);
  pulseEnable(cmd_high);
  gpio_set_level(LCD_SH, 1);
  for(tmp = 7; tmp >= 0; tmp--){
    gpio_set_level(LCD_WR, (cmd_low >> tmp) & 0x01);
    gpio_set_level(LCD_CLK, 1);
    gpio_set_level(LCD_CLK, 0);
  }
  vTaskDelay(2);
  gpio_set_level(LCD_SH, 0);
  pulseEnable(cmd_low);
}

/* ========================================================================= */
/* --- Envia os dados para o LCD --- */
void sendLCDData(unsigned char data)
{
  unsigned char data_high = ((data & 0xF0) | 0x04);
  unsigned char data_low = ((data << 4) | 0x04);
  int tmp;
  gpio_set_level(LCD_SH, 1);
  for(tmp = 7; tmp >= 0; tmp--){
    gpio_set_level(LCD_WR, (data_high >> tmp) & 0x01);
    gpio_set_level(LCD_CLK, 1);
    gpio_set_level(LCD_CLK, 0);
  }
  vTaskDelay(5);
  gpio_set_level(LCD_SH, 0);
  pulseEnable(data_high);
  gpio_set_level(LCD_SH, 1);
  for(tmp = 7; tmp >= 0; tmp--){
    gpio_set_level(LCD_WR, (data_low >> tmp) & 0x01);
    gpio_set_level(LCD_CLK, 1);
    gpio_set_level(LCD_CLK, 0);
  }
  vTaskDelay(5);
  gpio_set_level(LCD_SH, 0);
  pulseEnable(data_low);
}

/* ========================================================================= */
/* --- Envia dado para LCD --- */
void lcdWrite(unsigned char lin, unsigned char col, const char *dataSend)
{
  char pos = col;
  lcdCursor( lin, col );
  while(*dataSend)
    {
        sendLCDData(*dataSend);
        ++dataSend;
        ++pos;
    }
}

/* ========================================================================= */
/* --- Configura o cursor do LCD --- */
void lcdCursor( unsigned char lin, unsigned char col)
{
  sendLCDCommand(lcdSetCursor(((lcdLine1 * lin) + (col+lcdLine0))));
}

/* ========================================================================= */
/* --- Inicializa LCD no modo 8 bits --- */
void lcdClear(){
  sendLCDCommand(0x01);
}

/* ========================================================================= */
/* --- Inicializa LCD no modo 4 bits --- */
void lcdInit()
{
  espInit();
  vTaskDelay(15);
  sendLCDCommand(0x30);
  vTaskDelay(5);
  sendLCDCommand(0x30);
  vTaskDelay(0.100);
  sendLCDCommand(0x30);
  vTaskDelay(0.04);

  sendLCDCommand(0x02);
  vTaskDelay(0.04);
  sendLCDCommand(0x28);
  vTaskDelay(0.04);
  sendLCDCommand(0x0C);
  vTaskDelay(0.04);
  sendLCDCommand(0x06);
  vTaskDelay(0.04);
  sendLCDCommand(0x01);
  vTaskDelay(6);
  sendLCDCommand(0x80);
  vTaskDelay(5);
}
/* ========================================================================= */
/* --- Final do Programa --- */
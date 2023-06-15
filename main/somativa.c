
void app_main( void )
{
    devboardInit();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    lcdInit();
    lcdClear();
    lcdWrite(0, 4, "ola ");
    machineState();
}
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <inttypes.h>
#include "driver/GPIO.h"
#include "esp_rom_sys.h"

#define DHT_DATA_PIN GPIO_NUM_4
#define LOW 0

void app_main(void){

    gpio_set_direction(DHT_DATA_PIN, OUTPUT_ONLY);
    gpio_set_level(DHT_DATA_PIN, LOW);
    esp_rom_delay(1200);
    gpio_set_direction(DHT_DATA_PIN, INPUT_ONLY);
    gpio_set_pull_mode(DHT_DATA_PIN, GPIO_PULLUP_ONLY)


    
}
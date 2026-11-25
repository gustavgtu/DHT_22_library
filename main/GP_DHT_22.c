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
        //output low for atleast 1ms is the start signal for dht22
    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_DATA_PIN, LOW);
    esp_rom_delay_us(1200);
        //swith to input pullup mode
    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DHT_DATA_PIN, GPIO_PULLUP_ONLY);

}
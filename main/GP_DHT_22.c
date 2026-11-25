#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <inttypes.h>
#include "driver/GPIO.h"
#include "esp_rom_sys.h"

#define DHT_DATA_PIN GPIO_NUM_4
#define LOW 0
#define HIGH 1

void app_main(void){
    vTaskDelay(pdMS_TO_TICKS(2000));
    int64_t transmission_end[82], transmission_start[82];

    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_INPUT);
    
    gpio_set_pull_mode(DHT_DATA_PIN, GPIO_PULLUP_ONLY);
    esp_rom_delay_us(40);
    
    //output low for atleast 1ms is the start signal for dht22
    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_DATA_PIN, LOW);
    esp_rom_delay_us(1100);
    gpio_set_level(DHT_DATA_PIN, HIGH);
    esp_rom_delay_us(40);
        //swith to inpu;t pullup mode
    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DHT_DATA_PIN, GPIO_PULLUP_ONLY);

    for(int i=0; i<82; i=i+2){

    transmission_start[i] = esp_timer_get_time();
    while(gpio_get_level(DHT_DATA_PIN) == LOW);
    transmission_end[i] = esp_timer_get_time();


    transmission_start[i+1] = esp_timer_get_time();
    while(gpio_get_level(DHT_DATA_PIN) == HIGH);
    transmission_end[i+1] = esp_timer_get_time();

    }
    int level = 0;
    for(int i=0; i<82; ++i){
        printf("%s: %" PRId64 "\n", level%2==0 ? "LOW" : "HIGH",  transmission_end[i] - transmission_start[i]);
        ++level;
    }
}
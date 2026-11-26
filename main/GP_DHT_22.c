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
    int64_t transmission_end[80], transmission_start[80], low_time, high_time;
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint16_t raw_humidity, raw_temperature;

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

    while(gpio_get_level(DHT_DATA_PIN) == LOW);
    while(gpio_get_level(DHT_DATA_PIN) == HIGH);

    for(int i=0; i<80; i=i+2){

    transmission_start[i] = esp_timer_get_time();
    while(gpio_get_level(DHT_DATA_PIN) == LOW);
    transmission_end[i] = esp_timer_get_time();


    transmission_start[i+1] = esp_timer_get_time();
    while(gpio_get_level(DHT_DATA_PIN) == HIGH);
    transmission_end[i+1] = esp_timer_get_time();

    }
    
    for(int i=0; i<80; ++i){
        printf("%s: %" PRId64 "\n", i%2==0 ? "LOW" : "HIGH",  transmission_end[i] - transmission_start[i]);
    }

    for(int i=0; i<40; ++i){
        low_time = transmission_end[2*i] - transmission_start [2*i];
        high_time = transmission_end[2*i+1] - transmission_start[2*i+1];

        data[i/8] <<= 1;

        if(high_time > low_time){
            data[i/8] |= 1;
        }
    }

    raw_humidity = data[0];
    raw_humidity <<= 8;
    raw_humidity |= data[1];
    float humidity = raw_humidity / 10.0f;

    raw_temperature = data[2];
    raw_temperature <<= 8;
    raw_temperature |= data[3];
    float temperature = raw_temperature / 10.0f;


    printf("raw humidity: %d\n", raw_humidity);
    printf("humidity: %.2f\n", humidity);
    printf("raw temperature: %d\n", raw_temperature);
    printf("Temperature: %.2f\n", temperature);
    
    for(int i=0; i<5; ++i){
        printf("data: %d\n", data[i]);
    }

}
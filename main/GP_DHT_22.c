#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <inttypes.h>
#include "driver/gpio.h"
#include "esp_rom_sys.h"

#define DHT_DATA_PIN GPIO_NUM_4
#define LOW 0
#define HIGH 1
#define DHT_TIMEOUT_US 200
#define TIMEOUT -1

int wait_for_level(int level);

void app_main(void){
    vTaskDelay(pdMS_TO_TICKS(2000));
    int64_t transmission_end, transmission_start, low_time, high_time, transmission_time[80];
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint16_t raw_humidity, raw_temperature;

    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_INPUT);
    
    gpio_set_pull_mode(DHT_DATA_PIN, GPIO_PULLUP_ONLY);
    esp_rom_delay_us(40);
    
    //output low for atleast 1ms and high for 20-40us is the start signal for dht22
    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_DATA_PIN, LOW);
    esp_rom_delay_us(1100);
    gpio_set_level(DHT_DATA_PIN, HIGH);
    esp_rom_delay_us(40);
    //switch to input pullup mode for receiving data
    gpio_set_direction(DHT_DATA_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DHT_DATA_PIN, GPIO_PULLUP_ONLY);

    if(wait_for_level(LOW) == TIMEOUT){
        printf("timeout error");
    }
    if(wait_for_level(HIGH) == TIMEOUT){
        printf("timeout error");
    }

    for(int i=0; i<80; i=i+2){

    transmission_start = esp_timer_get_time();
    if(wait_for_level(LOW) == TIMEOUT){
        printf("timeout error");
    }
    transmission_end = esp_timer_get_time();
    transmission_time[i] = transmission_end - transmission_start;

    transmission_start = esp_timer_get_time();
    if(wait_for_level(HIGH) == TIMEOUT){
        printf("timeout error");
    }
    transmission_end = esp_timer_get_time();
    transmission_time[i+1] = transmission_end - transmission_start;

    }
    
    for(int i=0; i<80; ++i){
        printf("%s: %" PRId64 "\n", i%2==0 ? "LOW" : "HIGH",  transmission_time[i]);
    }

    for(int i=0; i<40; ++i){
        low_time = transmission_time[2*i];
        high_time = transmission_time[2*i+1];

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
    
    bool neg_temp = false;
    //negative temperature check, if msb 1 temperature is negative
    if (raw_temperature & (1 << 15)){
        raw_temperature &= ~(1 << 15);
        neg_temp = true;
    }
    
    float temperature = raw_temperature / 10.0f;

    if(neg_temp){
        temperature *= -1;
    }

    printf("raw humidity: %d\n", raw_humidity);
    printf("humidity: %.2f\n", humidity);
    printf("raw temperature: %d\n", raw_temperature);
    printf("Temperature: %.2f\n", temperature);
    
    uint8_t checksum = data[0]+data[1]+data[2]+data[3];
    //checksum
    if(checksum == data[4]){
        printf("Checksum passed\n");
    }
    else{
        printf("checksum failed\n");
    }


    printf("data1234: %d\n", checksum);
    printf("data4: %d", data[4]);
    

}

int wait_for_level(int level){
    int64_t timeout_timer=esp_timer_get_time();
    while(gpio_get_level(DHT_DATA_PIN) == level){
        if( esp_timer_get_time() - timeout_timer > DHT_TIMEOUT_US ){
            return TIMEOUT;
        }
    }
    return 0;
}
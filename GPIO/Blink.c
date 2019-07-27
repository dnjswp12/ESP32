
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

//#define BLINK_GPIO  4// GPIO5>>blue led(sdkconfig.h), 34인풋만 가능한게 있다
//0 2 4 5 12 13 14 15 18 19 21 22 23 25 26 27 32 33 34
#define GPIO0	0
#define GPIO2	2
#define GPIO4	4
#define GPIO5	5
#define GPIO10	10
#define GPIO12	12
#define GPIO13	13
#define GPIO14	14
#define GPIO15	15
#define GPIO18	18
#define GPIO19	19
#define GPIO21	21
#define GPIO22	22
#define GPIO23	23
#define GPIO25	25
#define GPIO26	26
#define GPIO27	27
#define GPIO32	32
#define GPIO33	33
#define GPIO34	34
#define PIN_SEL		((1ULL<<GPIO15)|(1ULL<<GPIO2)|(1ULL<<GPIO0)|(1ULL<<GPIO4)|(1ULL<<GPIO5)|(1ULL<<GPIO18)|(1ULL<<GPIO19)|(1ULL<<GPIO21)|(1ULL<<GPIO22)|(1ULL<<GPIO23))
void blink_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    //gpio_pad_select_gpio(BLINK_GPIO);
    //Set the GPIO as a push/pull output 
    //gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  /* while(1)
		    {
		            // Blink off (output low)
		            gpio_set_level(GPIO2, 0);
		            vTaskDelay(100 / portTICK_PERIOD_MS);
		            //Blink on (output high) 
		            gpio_set_level(GPIO2, 1);
		            vTaskDelay(100 / portTICK_PERIOD_MS);
		    }*/
}

void app_main()
{
	gpio_config_t io_conf;
		    //disable interrupt
		    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
		    //set as output mode
		    io_conf.mode = GPIO_MODE_OUTPUT;
		    //bit mask of the pins that you want to set,e.g.GPIO18/19
		    io_conf.pin_bit_mask = PIN_SEL;
		    //disable pull-down mode
		    io_conf.pull_down_en = 0;
		    //disable pull-up mode
		    io_conf.pull_up_en = 0;
		    //configure GPIO with the given settings
		    gpio_config(&io_conf);

		    while(1)
		    {
		            /* Blink off (output low) */
		            gpio_set_level(GPIO2, 0);
		            vTaskDelay(100 / portTICK_PERIOD_MS);
		            /* Blink on (output high) */
		            gpio_set_level(GPIO2, 1);
		            vTaskDelay(100 / portTICK_PERIOD_MS);
		    }
    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}

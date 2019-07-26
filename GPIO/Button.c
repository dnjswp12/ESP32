/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
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

void app_main()
{
	int sw;
	gpio_pad_select_gpio(GPIO15);
	gpio_pad_select_gpio(GPIO2); // chosen gpio func
	
	gpio_set_direction(GPIO15,GPIO_MODE_INPUT);
	gpio_set_direction(GPIO2,GPIO_MODE_OUTPUT); 

	while(1)
		   {
		    	sw = gpio_get_level(GPIO15); // to read gpio15 pad
		    	printf();
		    	if(sw == 0)
		    	{
		            /* Blink off (output low) */
		            gpio_set_level(GPIO2, 0);
		            vTaskDelay(100 / portTICK_PERIOD_MS);
		            /* Blink on (output high) */
		            gpio_set_level(GPIO2, 1);
		            vTaskDelay(100 / portTICK_PERIOD_MS);
		    	}
		   }
}

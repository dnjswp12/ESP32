/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
/*
 * The pin ISR handlers no longer need to be declared with IRAM_ATTR,
 * unless you pass the ESP_INTR_FLAG_IRAM flag when allocating the ISR in gpio_install_isr_service().
 * ESP_INTR_FLAG_IRAM 플래그를 선언하지 않으면 ISR 처리 하는 함수 IRAM_ATTR을 선언할 필요가 없다.

 *
 * */
#define LEDr	(15)
#define LEDy	(2)
#define SW		(0)
#define ESP_INTR_FLAG_DEFAULT 0


static xQueueHandle gpio_evt_queue = NULL;



// interrupt service routine, called when the button is pressed
void IRAM_ATTR button_isr_handler(void* arg) {

	 uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
    // notify the button task
}

// task that will react to button clicks
void button_task(void* arg) {
	uint32_t io_num;
	// infinite loop
	for(;;) {

		 if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
		 {
			 printf("INT!\n");
			 gpio_set_level(LEDy, 1);
		  }
	}
}
void blink_task (void* arg)
{
	while(1)
	{
		printf("blink red LED!\n");
		gpio_set_level(LEDr,1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(LEDr,0);
		gpio_set_level(LEDy, 0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}

}
void app_main()
{

	gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

	// configure button and led pins as GPIO pins
	gpio_pad_select_gpio(SW);
	gpio_pad_select_gpio(LEDr);
	gpio_pad_select_gpio(LEDy);

	// set the correct direction
	gpio_set_direction(SW, GPIO_MODE_INPUT);
  gpio_set_direction(LEDr, GPIO_MODE_OUTPUT);
  gpio_set_direction(LEDy, GPIO_MODE_OUTPUT);

	// enable interrupt on falling (1->0) edge for button pin
	gpio_set_intr_type(SW, GPIO_INTR_NEGEDGE);

	// start the task that will handle the button
	//xTaskCreate(태스크 함수, 태스크 이름, 사용할 스택크기, 함수 실행 시 넘길 인자, 태스크 우선순위, 태스크 핸들)
	xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
	xTaskCreate(blink_task, "blink_red_LED", 2048, NULL, 10, NULL);

	// install ISR service with default configuration
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

	// attach the interrupt service routine
	gpio_isr_handler_add(SW, button_isr_handler, NULL);


}




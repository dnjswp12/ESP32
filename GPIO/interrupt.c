
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

//0 2 4 5 12 13 14 15 18 19 21 22 23 25 26 27 32 33 까지 I/O 둘 다 지원.
#define LEDr	2
#define LEDy	0
#define SW		4
#define PIN_SEL_IN  (1ULL<<SW)
#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle gpio_evt_queue = NULL;
/*위의 코드를 잘 보면 인터럽트가 발생했을 때 호출될 함수 선언에 IRAM_ATTR 이라는 것이 추가되어 있는것을 알 수 있을 것이다.
IRAM_ATTR을 넣어줘야 인터럽트 처리 함수가 RAM에 들어가게 된다.
IRAM_ATTR이 없으면 인터럽트 처리 함수가 플래쉬에 들어가게 되어 실행 속도가 느려진다. */
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    //int a=0;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            //printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        	printf("interrupt set!\n ");

            gpio_set_level(LEDr,0);
            gpio_set_level(LEDy,1);
        }
    }
}

void app_main()
{
	int t=0;

	gpio_set_direction(LEDr,GPIO_MODE_OUTPUT);
	gpio_set_direction(LEDy,GPIO_MODE_OUTPUT);
  //출력으로 설정
	gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_NEGEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = PIN_SEL_IN;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    //스위치 입력핀 설정 스위치 회로는 풀업으로 구성 눌렀을때 인터럽트 발생함.
    
    //change gpio intrrupt type for one pin
    gpio_set_intr_type(SW, GPIO_INTR_NEGEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(SW, gpio_isr_handler, (void*) SW);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(SW);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(SW, gpio_isr_handler, (void*) SW);


    while(1) {
    	t++;
    	vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_set_level(LEDr, t%2);
        gpio_set_level(LEDr, t%2);
        gpio_set_level(LEDy,0);

    }
}



#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LED_TIMER		LEDC_TIMER_0
#define LED_MODE		LEDC_HIGH_SPEED_MODE
#define LED_HS_CH0_GPIO		(4)
#define LED_CHENNEL_0	 LEDC_CHANNEL_0

#define LEDC_TEST_CH_NUM		(1)
#define LEDC_TEST_DUTY         (1000) //듀티 사이클, 4000>50%, 듀티 비 인거 확인
#define LEDC_TEST_FADE_TIME    (3000)
//80Mhz
//10bit >> 80Mhz / 2^10bit = 78.125kHz
// 80Mhz 

void app_main()
{
	int ch;//타이머를 여러개 쓸때 채널 selection 할때 사용

	ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal // 주기는 200us
        .speed_mode = LED_MODE,           // timer mode
        .timer_num = LED_TIMER            // timer index
    };
	 ledc_timer_config(&ledc_timer);

	ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
        {
            .channel    = LED_CHENNEL_0,
            .duty       = 0,
            .gpio_num   = LED_HS_CH0_GPIO,
            .speed_mode = LED_MODE,
            .timer_sel  = LED_TIMER
        }
    };
	 //Set LED Controller with previously prepared configuration
	    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
	        ledc_channel_config(&ledc_channel[ch]);
	    }//채널 여러개 사용할
      ledc_fade_func_install(0);
      while(1)
      {
    	  printf("1. LEDC fade up to duty = %d\n", LEDC_TEST_DUTY);
    	          for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
    	          {
    	              ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
    	                      ledc_channel[ch].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
    	              ledc_fade_start(ledc_channel[ch].speed_mode,
    	                      ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
    	          }
    	          vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
    	  printf("2. LEDC fade down to duty = 0\n");
    	  for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
    	  {
    		  ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
    				  ledc_channel[ch].channel, 0, LEDC_TEST_FADE_TIME);
    		  ledc_fade_start(ledc_channel[ch].speed_mode,
    				  ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
    	  }
    	  vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
      }
}


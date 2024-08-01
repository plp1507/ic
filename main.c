#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"
#include "esp_adc/adc_continuous.h"
#include "freertos/semphr.h"
#include "sdkconfig.h"


#define EXAMPLE_ADC_UNIT                    ADC_UNIT_1
#define _EXAMPLE_ADC_UNIT_STR(unit)         #unit
#define EXAMPLE_ADC_UNIT_STR(unit)          _EXAMPLE_ADC_UNIT_STR(unit)
#define EXAMPLE_ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define EXAMPLE_ADC_ATTEN                   ADC_ATTEN_DB_0
#define EXAMPLE_ADC_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define EXAMPLE_ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define EXAMPLE_ADC_GET_CHANNEL(p_data)     ((p_data)->type1.channel)
#define EXAMPLE_ADC_GET_DATA(p_data)        ((p_data)->type1.data)
#else
#define EXAMPLE_ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE2
#define EXAMPLE_ADC_GET_CHANNEL(p_data)     ((p_data)->type2.channel)
#define EXAMPLE_ADC_GET_DATA(p_data)        ((p_data)->type2.data)
#endif

#define EXAMPLE_READ_LEN                    256

#if CONFIG_IDF_TARGET_ESP32
static adc_channel_t channel[2] = {ADC_CHANNEL_6, ADC_CHANNEL_7};
#else
static adc_channel_t channel[2] = {ADC_CHANNEL_2, ADC_CHANNEL_3};
#endif

static TaskHandle_t s_task_handle;

static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data){
	BaseType_t mustYield = pdFALSE;
	vTaskNotifyGiveFromISR(s_task_handle, &mustYield);
	return (mustYield == pdTRUE);
}


static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
	adc_continuous_handle_t handle = NULL;

	adc_continuous_handle_cfg_t adc_config = {
		.max_store_buf_size = 1024,
		.conv_frame_size = EXAMPLE_READ_LEN,
	};
	ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

	adc_continuous_config_t dig_cfg = {
		.sample_freq_hz = 183333,
		.conv_mode = EXAMPLE_ADC_CONV_MODE,
		.format = EXAMPLE_ADC_OUTPUT_TYPE,
	};

	adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
	dig_cfg.pattern_num = channel_num;
	for(int i = 0; i < channel_num; i++){
		adc_pattern[i].atten = EXAMPLE_ADC_ATTEN;
		adc_pattern[i].channel = channel[i] & 0x7,
		adc_pattern[i].unit = EXAMPLE_ADC_UNIT,
		adc_pattern[i].bit_width = EXAMPLE_ADC_BIT_WIDTH;
	}
	dig_cfg.adc_pattern = adc_pattern;
	ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));
	*out_handle = handle;
}

void app_main(void)
{

//esp_err_t ret;
uint32_t ret_num = 0;
uint8_t result[EXAMPLE_READ_LEN] = {0};

memset(result, 0xcc, EXAMPLE_READ_LEN);

adc_continuous_handle_t handle = NULL;
continuous_adc_init(channel, sizeof(channel)/sizeof(adc_channel_t), &handle);

adc_continuous_evt_cbs_t cbs = {
	.on_conv_done = s_conv_done_cb,
};
ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
ESP_ERROR_CHECK(adc_continuous_start(handle));

int leitura;
int leitura2;

while(1){
	//leitura = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
	leitura = analogRead(34);
	leitura2 = analogRead(35);
	printf("%d", leitura);
	printf("\n");
	printf("%d", leitura2);
	printf("\n");
	delay(100);
	}	
}

#include "myadc.h"

float get_adc(ADC_HandleTypeDef *hadc)
{
	uint32_t temp;
	HAL_ADC_Start(hadc);
	temp=HAL_ADC_GetValue(hadc);
	return temp*3.3/4096;
}

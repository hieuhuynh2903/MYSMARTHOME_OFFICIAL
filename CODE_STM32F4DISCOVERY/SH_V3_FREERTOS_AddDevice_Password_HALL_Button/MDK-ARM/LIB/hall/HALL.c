#include "HALL.h"

int threshold = 50;

extern ADC_HandleTypeDef hadc2;
uint32_t TA12_value;

uint32_t current_value;
uint64_t start_time_getTA12Value1=0;
uint32_t read_Current(){
	
	if ((HAL_GetTick() - start_time_getTA12Value1) > 1000 ){		
		start_time_getTA12Value1 = HAL_GetTick();
	
		uint32_t TA12Max = 0;
		uint64_t start_time_getTA12Value2 = HAL_GetTick();
		while((HAL_GetTick()-start_time_getTA12Value2) < 300)//sample for 300ms 
			{ 
				HAL_ADC_Start (&hadc2);
				HAL_ADC_PollForConversion (&hadc2, 1);
				TA12_value = HAL_ADC_GetValue(&hadc2);
				HAL_ADC_Stop (&hadc2);
				
				if (TA12_value > TA12Max){
					/*record the maximum sensor value*/
					TA12Max = TA12_value*1000000/4096*3.3/560/1.414;
				}
			}
		current_value	= TA12Max; // mA
	}
	
return current_value;  	
}


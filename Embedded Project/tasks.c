#include "tasks.h"



extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern uint8_t reading_buffer[5];
extern uint8_t temp_entering_text[33];
extern int index_reading_buffer;
extern int volatile threshold_temp_whole;
extern int volatile threshold_temp_decimal;
extern int volatile actual_temp_whole;
extern int volatile actual_temp_decimal;
extern int volatile threshold_exceeded_flag;
extern int flag;
extern q * delay_queue;

int counter_Led_blinking=0;
uint8_t temp_entered_success_text[] = " (THRESHOLD UPDATED)";
uint8_t wrong_format_text[] = " (WRONG FORMAT, PLEASE TRY AGAIN)";

void task_1_read_temperature(void){
	uint8_t tempLSB[2], tempMSB[2];
	uint8_t out[] = {0,0,'.',0,0,'\r','\n'};
	
	tempMSB[0] = 0x11;
	tempLSB[0] = 0x12;
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	
	/*
	uint8_t volatile status=0;
	uint8_t volatile control=0;
	HAL_I2C_Mem_Read(&hi2c1, 0xD0, 0x0F, 1, &status, 1, 100);  // read status register
	if (!(status&0x04))  // if the BSY bit is not set
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xD0, 0x0E, 1, &control, 1, 100);  // read control register
		HAL_I2C_Mem_Write(&hi2c1, 0xD0, 0x0E, 1, (uint8_t *)(control|(0x20)), 1, 100);  // write modified control register with CONV bit as'1'
	}*/
	
	
	
	//send to register address 11h
	HAL_I2C_Master_Transmit(&hi2c1, 0xD0, tempMSB, 1, 10);
	//read data of register 11h to tempMSP[1]
	HAL_I2C_Master_Receive(&hi2c1, 0xD1, tempMSB+1, 1, 10);
	actual_temp_whole=tempMSB[1];
	
	//send to register address 11h
	HAL_I2C_Master_Transmit(&hi2c1, 0xD0, tempLSB, 1, 10);
	//read data of register 11h to tempMSP[1]
	HAL_I2C_Master_Receive(&hi2c1, 0xD1, tempLSB+1, 1, 10);
	actual_temp_decimal=tempLSB[1];
	
	if(actual_temp_whole> threshold_temp_whole || (actual_temp_whole== threshold_temp_whole && actual_temp_decimal>threshold_temp_decimal)){
		//if(threshold_exceeded_flag==0){
			threshold_exceeded_flag=1;
			//QueTask(task_2_blink_leds, 7);
		//}
		counter_Led_blinking =0;
	}else{
		threshold_exceeded_flag=0;
	}
	
	ReRunMe(30);

}
//void task_2_blink_leds(void){
//	int i =0, j=0;
//	
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
//		for(i=0;i<5000000;i++){}
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
//		for(i=0;i<5000000;i++){}
//		//counter_Led_blinking++;	
//		//if(threshold_exceeded_flag==1 /*&& counter_Led_blinking<=10*/) ReRunMe(5);
//}
void task_3_set_threshold(void){
	if(reading_buffer[0]>='0' && reading_buffer[0]<='9' && reading_buffer[1]>='0' && reading_buffer[1]<='9' && reading_buffer[2]=='.' && reading_buffer[3]>='0' && reading_buffer[3]<='9' && reading_buffer[4]>='0' && reading_buffer[4]<='9' )
	{
		HAL_UART_Transmit(&huart1,temp_entered_success_text, sizeof(temp_entered_success_text), HAL_MAX_DELAY); //in case the format is correct
		threshold_temp_whole = (reading_buffer[0] -'0')*10 + reading_buffer[1] -'0';
		threshold_temp_decimal =  (reading_buffer[3] -'0')*10 + reading_buffer[4] -'0';
	}
	else{ 
		HAL_UART_Transmit(&huart1,wrong_format_text, sizeof(wrong_format_text), HAL_MAX_DELAY);
	}
	index_reading_buffer=0;
	HAL_UART_Transmit(&huart1,temp_entering_text, sizeof(temp_entering_text), HAL_MAX_DELAY);


}

void task_4_systick(void){
	if(threshold_exceeded_flag==1){
			if(flag==1){
				flag =0;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			}else{
				flag =1;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
			}
		}else{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		} 
	
	
		t * current_p;
		t * deleted;
    current_p = delay_queue->head_of_queue;

    while(current_p!=NULL){
        current_p->delay -= 1;
        if(current_p->delay<=0){
        	QueTask(current_p->f, current_p->priority);
        	delay_queue->head_of_queue = current_p->next_task;
					deleted = current_p;
					current_p = current_p->next_task;
					free(deleted);
        }else{
					current_p = current_p->next_task;
				}
        
    }
}
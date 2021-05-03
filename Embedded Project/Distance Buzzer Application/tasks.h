#ifndef TASKS_H
#define TASKS_H

#include "main.h"


void delay (uint16_t time); //Micro Delay
void HCSR04_Read (void);
void input_capture(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim);
void task_4_Buzz_sound(void);
void task_3_measure_distance(void);
void task_4_systick(void);




#endif
#include "main.h"
#include "tim.h"
#include "usart.h"
/*Moje include'y*/
	#include "globalVars.h"
	#include "string.h"
	#include "stdio.h"
	#include "lcd_i2c.h"
/*==============================Callbacki przerwań==============================*/
	/*Przerwanie po upłynięciu tim6 równego 10000 cykli (po sekundzie) - zebranie danych z czujników - clock 72MHz prescaler 7200-1, cycles 10000*/
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

		if ((count == sensorRead_freq) || (count == 0)){
		analogDeviceReadDMA();
		displayReadings(disp_No);
		}
		count++; //Dodawaj jeden co sekundę do tego countera
		if (count >= sensorRead_freq) count = 0;
		// Jeśli ilość sekund count jest większa od sensor_read_freq to zresetuj count do 1,
		// pozwala to ustawić jedną zmienną częstotliwość odświeżania pomiarów ile tylko dusza zapragnie
	}
	/*Przerwanie na liniach 10-15, w tym przypadku to jest button na płytce Nucleo*/
		void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
		{
				if(!(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))){
				disp_No++;
				if (disp_No == 5) disp_No = 1;
				displayReadings(disp_No);
				}
		}
/*==========================================================================================*/
	/*Milisekundowy delay na timerze, tim2 prescaler 72-1, brak trybu IT*/
	void delay_us(uint32_t time){
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		while ((__HAL_TIM_GET_COUNTER(&htim2))<time);
	}

	/*Wyślij po UARCIE numer 2 baudrate = 38400/s*/
	void sendString_UART(char*text){
		HAL_UART_Transmit(&huart2,  (uint8_t*)text, strlen(text), 1000);
	}

	/*Przeskaluj wartość analogową z ADCka na procenty*/
	float map(uint16_t val, int in_min, int in_max, int out_min, int out_max) {
	  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

#include "main.h"
#include "tim.h"
#include "usart.h"
/*Moje include'y*/
	#include "globalVars.h"
	#include "string.h"
	#include "stdio.h"
	#include "lcd_i2c.h"
/*==============================Callbacki przerwań==============================*/
	/*Przerwanie po upłynięciu tim6 równego 10000 cykli (po sekundzie) - zebranie danych z czujników po przekroczeniu progu sensorRead_freq - clock 72MHz prescaler 7200-1, cycles 10000*/
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

		if ((count == sensorRead_freq) || (count == 0)){
		analogDeviceReadDMA();
		displayReadings(disp_No);
			if(waterMode == 1 && moistureThreshold >= moistureAverage){
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
				delay_us(500000);
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
			}
		}



		count++; //Dodawaj jeden co sekundę do tego countera
		if (count >= sensorRead_freq) count = 0;
		// Jeśli ilość sekund count jest większa od sensor_read_freq to zresetuj count do 0,
		// pozwala to ustawić jedną zmienną częstotliwość odświeżania pomiarów ile tylko dusza zapragnie
	}

	/*Przerwanie na liniach 10-15, w tym przypadku to jest button na płytce Nucleo*/
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{
		if(!(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))){
		disp_No++;
		if (disp_No == 6) disp_No = 1;
		displayReadings(disp_No);
		}
		/*Ustaw większy próg rozpoczęcia podlewania*/
		if(!(HAL_GPIO_ReadPin(TR_UP_GPIO_Port, TR_UP_Pin))){
			if(moistureThreshold != 100) moistureThreshold+=5;
			sprintf((char *)disp.f_line, "MoistThres: %d%%", moistureThreshold);
			sprintf((char *)disp.s_line, " ");
			lcd_display(&disp);
			delay_us(700000);
			displayReadings(disp_No);
		}

		/*Ustaw mniejszy próg rozpoczęcia podlewania*/
		if(!(HAL_GPIO_ReadPin(TR_DOWN_GPIO_Port, TR_DOWN_Pin))){
			if(moistureThreshold != 0) moistureThreshold-=5;
			sprintf((char *)disp.f_line, "MoistThres: %d%%", moistureThreshold);
			sprintf((char *)disp.s_line, " ");
			lcd_display(&disp);
			delay_us(700000);
			displayReadings(disp_No);
		}

		/*Ustaw tryb manualny / automatyczny*/
		if(!(HAL_GPIO_ReadPin(B_MODE_GPIO_Port, B_MODE_Pin))){
			if (waterMode == 0){
				waterMode = 1;
				sprintf((char *)disp.f_line, "Mode: Auto");
				sprintf((char *)disp.s_line, " ");
				lcd_display(&disp);
				delay_us(700000);
				displayReadings(disp_No);

			}
			else{
				waterMode = 0;
				sprintf((char *)disp.f_line, "Mode: Manual");
				sprintf((char *)disp.s_line, " ");
				lcd_display(&disp);
				delay_us(700000);
				displayReadings(disp_No);
			}
		}


		if(!(HAL_GPIO_ReadPin(B_WATER_GPIO_Port, B_WATER_Pin))){
			if (waterMode == 0){
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			delay_us(500000);
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			}
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

	/*Przeskaluj wartość analogową z ADCka na procenty - czujniki wilgotności gleby*/
	float map(uint16_t val, int in_min, int in_max, int out_min, int out_max) {
	  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

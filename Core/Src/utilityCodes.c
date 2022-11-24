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
		/*Odświeżanie się pomiarów z czujnika i wyświetlacza */
		if ((count == sensorRead_freq) || (count == 0)){
			analogDeviceReadDMA();
			displayReadings(disp_No);
		}
		/*============================================================*/
		/*Logika załączania się podlewania automatycznie - Jeśli tryb jest auto i średnia wilgoci mniejsza od progu załącz podlewanie, dodatkowo żeby co pomiar nie włączał na nowo podlewania co sekundę  */
		if(waterMode == 1 && moistureThreshold >= moistureAverage ){
			if (isIrrigON == 0){
				HAL_GPIO_WritePin(WATER_PIN_GPIO_Port, WATER_PIN_Pin, GPIO_PIN_RESET);
				isIrrigON = 1;
			}
			count2++;
		}
					/*Po upłynięciu czasu podlewania wyłącz pin i zresestuj count2 */
		if (count2 >= waterTimeMins && waterMode == 1 ){ // waterTime w sekundach
			HAL_GPIO_WritePin(WATER_PIN_GPIO_Port, WATER_PIN_Pin, GPIO_PIN_SET);
			count2 = 0;
			isIrrigON = 0;
			/*Odśwież pomiary po podlaniu*/
			analogDeviceReadDMA();
			displayReadings(disp_No);
		}
		/*============================================================*/
		 /*Odpalanie podlewania z przycisku - nie można przez interrupt handler bo jak policzysz czas xd */
		if (waterMode == 0 && waterButtonPressed == 1){
			if (isIrrigONMan == 0){
				HAL_GPIO_WritePin(WATER_PIN_GPIO_Port, WATER_PIN_Pin, GPIO_PIN_RESET);
				isIrrigONMan = 1;
			}
			count2++;

			if (count2 >= waterTimeMins){ // waterTime w sekundach
				HAL_GPIO_WritePin(WATER_PIN_GPIO_Port, WATER_PIN_Pin, GPIO_PIN_SET);
				count2 = 0;
				waterButtonPressed = 0;
				isIrrigONMan = 0;
			}
		}
		/*============================================================*/

		/*Przy włączeniu trybu manualnego upewnić się że wyłączy się pin podlewający*/
		if(waterMode == 0 && isIrrigON == 1){
			HAL_GPIO_WritePin(WATER_PIN_GPIO_Port, WATER_PIN_Pin, GPIO_PIN_SET);
			count2 = 0;
			isIrrigON = 0;
		}


		if(waterMode == 1 && isIrrigONMan == 1){
			HAL_GPIO_WritePin(WATER_PIN_GPIO_Port, WATER_PIN_Pin, GPIO_PIN_SET);
			count2 = 0; isIrrigONMan = 0;
			waterButtonPressed = 0;
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

			valueChanged = 1;
		}

		/*Ustaw mniejszy próg rozpoczęcia podlewania*/
		if(!(HAL_GPIO_ReadPin(TR_DOWN_GPIO_Port, TR_DOWN_Pin))){
			if(moistureThreshold != 0) moistureThreshold-=5;
			sprintf((char *)disp.f_line, "MoistThres: %d%%", moistureThreshold);
			sprintf((char *)disp.s_line, " ");
			lcd_display(&disp);

			valueChanged = 1;
		}

		/*Ustaw tryb manualny / automatyczny*/
		if(!(HAL_GPIO_ReadPin(B_MODE_GPIO_Port, B_MODE_Pin))){
			if (waterMode == 0){
				waterMode = 1;
				sprintf((char *)disp.f_line, "Mode: Auto");
				sprintf((char *)disp.s_line, " ");
				lcd_display(&disp);
			}
			else{
				waterMode = 0;
				sprintf((char *)disp.f_line, "Mode: Manual");
				sprintf((char *)disp.s_line, " ");
				lcd_display(&disp);
			}

			valueChanged = 1;
		}

		/*Podlewanie manualne*/
		if(!(HAL_GPIO_ReadPin(B_WATER_GPIO_Port, B_WATER_Pin))){
			if (waterMode == 0){
				 waterButtonPressed = 1;
			}
		}
		/*Ustaw większy czas podlewania o 5 minut*/
		if(!(HAL_GPIO_ReadPin(T_UP_GPIO_Port, T_UP_Pin))){
			if(waterTimeMins != 55) waterTimeMins+=5;
			sprintf((char *)disp.f_line, "IrrigTime: %dmin", waterTimeMins);
			sprintf((char *)disp.s_line, " ");
			lcd_display(&disp);

			valueChanged = 1;
		}
		/*Ustaw mniejszy czas podlewania o 5 minut*/
		if(!(HAL_GPIO_ReadPin(T_DOWN_GPIO_Port, T_DOWN_Pin))){
			if(waterTimeMins != 5) waterTimeMins-=5;
			sprintf((char *)disp.f_line, "IrrigTime: %dmin", waterTimeMins);
			sprintf((char *)disp.s_line, " ");
			lcd_display(&disp);

			valueChanged = 1;
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

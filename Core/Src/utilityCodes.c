#include "main.h"
#include "tim.h"
#include "usart.h"
/*Moje include'y*/
	#include "globalVars.h"
	#include "string.h"
	#include "stdio.h"
	#include "lcd_i2c.h"


/*==========================================================================================*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	/*Przerwanie po upłynięciu tim6 równego 10000 cykli (po sekundzie) - zebranie danych z czujników*/
		if ((count == sensorRead_freq) || (count == 0)){
		//analogDeviceReadDMA();
		DHT11_allData();
		displayReadings(disp_No);
		}
		count++; //Dodawaj jeden co sekundę do tego countera
		if (count >= sensorRead_freq) count = 1; // Jeśli ilość sekund count jest większa od sensor_read_freq to zresetuj count do 1
	}

/*Przerwanie na liniach 10-15, w tym przypadku to jest button na płytce Nucleo*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if(!(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))){
		disp_No++;
		if (disp_No == numberOfDisplays) disp_No = 1;
		displayReadings(disp_No);
		}

	//	while((HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)));
		/*Zmień częstotliwość odczytów z czujników*/
//		if(!(HAL_GPIO_ReadPin(B2_GPIO_Port,B2_Pin))){
//			pushCount++;
//			if(pushCount == 4) pushCount = 1;
//			switch(pushCount){
//			case 1:
//				sensorRead_freq = 1; // sekunda
//				count = 1;
//				sprintf((char *)disp.f_line, "ReadFreq 1s");
//			break;
//			case 2:
//				sensorRead_freq = 60; // minuta
//				count = 1;
//				sprintf((char *)disp.f_line, "ReadFreq 60s");
//			break;
//			case 3:
//				sensorRead_freq = 300; // 5 minut
//				count = 1;
//				sprintf((char *)disp.f_line, "ReadFreq 300s");
//			break;
//			}
//			sprintf((char *)disp.s_line, " ");
//			lcd_display(&disp);
//			// delay_us(500000);
//			// displayReadings(disp_No);
//
//		}
}



/*Milisekundowy delay na timerze, tim2 prescaler 72-1, brak przerwań przez tim2*/
void delay_us(uint32_t time){
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim2))<time);
}

/*Wyślij po UARCIE numer 2 baudrate = 115200B/s*/
void sendString_UART(char*text){
	HAL_UART_Transmit(&huart2,  (uint8_t*)text, strlen(text), 1000);
}

/*Przeskaluj wartość analogową z ADCka na procenty*/
float map(uint16_t val, int in_min, int in_max, int out_min, int out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

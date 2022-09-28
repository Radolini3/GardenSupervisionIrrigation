#include "main.h"
//#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
	/*Moje include'y*/
	#include "string.h"
	#include "stdio.h"
	#include "lcd_i2c.h"
	     const short numberOfADCChannels = 7;

		 /*Zmienne przechowujące bufor dla przesyłania UART'em*/
		 char UartOutText[100], UartInput[5];

		 /*ADC zmienna*/
		 uint32_t ADC_VAL[7];

		 /*Zmienne do przeliczenia wartości z adc na procenty*/
		 float moisture_percentage[6];
		 float lightIntensity;

		 /*Zmienne do obsługi DHT11*/
		 uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
		 uint16_t SUM, RH, TEMP;
		 uint8_t Presence = 0;

		 /*Zmienne przechowujące odczyt DHT11*/
		 float Temperature;
		 float Humidity;
		 int disp_No = 1;

		 /*Obsługa LCD*/
		 struct lcd_disp disp;
		 int count = 0;

		 /*Zmienna do sterowania czasem między odczytami i przesyłem danych z cuzjników*/
		 short sensorRead_freq = 1;
		 short pushCount = 0;
		 short numberOfDisplays = 4;


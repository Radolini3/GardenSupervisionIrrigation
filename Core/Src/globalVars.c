#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
	/*Moje include'y*/
	#include "string.h"
	#include "stdio.h"
	#include "lcd_i2c.h"
		 /*Zmienne przechowujące bufor dla przesyłania UART'em*/
		 char UartOutText[100], UartInput[5];
		 /*ADC zmienna*/
		 uint32_t ADC_VAL[8];
		 /*Zmienne do przeliczenia wartości z adc na procenty*/
		 float moisture_percentage[6];
		 /*Zmienne przechowujące odczyt czujnika temperatury*/
		 float Temperature;
		 float Humidity;
		 /*Zmienna do przełączania wyświetlacza*/
		 int disp_No = 1;
		 /*Obsługa LCD*/
		 struct lcd_disp disp;
		 int count = 0;
		 /*Zmienna do sterowania czasem między odczytami i przesyłem danych z cuzjników*/
		 short sensorRead_freq = 60;
		 short pushCount = 0;

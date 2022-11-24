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
		 int count2 = 0;
		 /*Zmienna do sterowania czasem między odczytami i przesyłem danych z cuzjników*/
		 short sensorRead_freq = 10; // Zmienna w sekundach czasu odświeżania odczytów z czujników

		 short waterMode = 0; // Jeśli jest 1 to jest auto, jeśli 0 to tryb manual
		 short moistureThreshold = 50; // Próg rozpoczęcia podlewania w trybie auto - domyślnie 50%
		 short moistureAverage = 0; // Średnia pomiarów
		 short waterTimeMins = 15; // Ustawiony czas w minutach podlewania - skok co 5 minut - domyślnie 15 minut

		 /*Zmienne pomocnicze ze sterowaniem z panelu*/
		 short waterButtonPressed = 0;
		 short valueChanged = 0;
		 short isIrrigON = 0;
		 short isIrrigONMan = 0;


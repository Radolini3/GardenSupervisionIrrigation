#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
	/*Moje include'y*/
	#include "stdio.h"
	#include "globalVars.h"

	void analogDeviceReadDMA(){

		/*Wartość analogowa jest odwrotnie proporcjonalna do procentowego wskaźnika wilgotności, im większe napięcie tym mniejsza wilgoć gleby*/

		HAL_ADC_Start_DMA(&hadc1, ADC_VAL, 8);		 //Odczyt poprzez DMA ze wszystkich kanałów ADC
		delay_us(1000);							     //Bez tego delaya się rozlatuje odczyt przez zbyt małą ilość czasu na próbkowanie sygnału analogowego
		HAL_ADC_Stop_DMA(&hadc1); 				     //Zatrzymaj odczyt ADC przez DMA


		Temperature =  -66.875 + (72.917 * ((float)ADC_VAL[0] / 4095 * 3.3));  // T =  -66.875 + (72.917 * Vin)
		Humidity = -12.5 + (41.667 * ((float)ADC_VAL[1] / 4095 * 3.3));  // H = -12.5 + (41.667 * Vin)

		for(int i = 0; i<6; i++){ 			   //Przeskalowanie na procenty (wcześniej test czujnika jak nisko może zejść analogowa wartość w przypadku maksymalnie wilgotnej gleby)
			moisture_percentage[i] = 100-(map(ADC_VAL[i+2], 2100, 3100, 0, 100));  // +2 bo od trzeciego kanału dopiero zczytujemy wilgotność gleby. Wersja dla czujników bez rezystora 1MOhm
			//moisture_percentage[i] = 100-(map(ADC_VAL[i+2], 1900, 2800, 0, 100)); // dla czujników z rezystorem 1MOhm
			if (moisture_percentage[i]>100){moisture_percentage[i] = 100;} //Wyrównaj drobne fluktuacje do 100 lub 0 %, żeby nie pokazywało ujemnych wartości lub większych jak 100
			if (moisture_percentage[i]<0){moisture_percentage[i] = 0;}
		}

		/*Wartość analogowa jest wprost proporcjonalna do procentowego wskaźnika nasłonecznienia*/

		sendAllReadingsUART(); 								 //Wyślij wszystko po UART'cie

		}


	void sendAllReadingsUART(){
		/*Przesył pod przechwytywanie do formatu csv*/
		sendString_UART("S");
		sprintf(UartOutText, "%1.f", Temperature);   //Temperatura z DHT11
		sendString_UART(UartOutText);
		sendString_UART("S");
		sprintf(UartOutText, "%1.f", Humidity);	   // Wilgotność powietrza
		sendString_UART(UartOutText);
		for(int i = 0; i<6; i++){
			sendString_UART("S");
			sprintf(UartOutText, "%1.f", moisture_percentage[i]);
			sendString_UART(UartOutText);
		}
		sendString_UART("\n");

//		sprintf(UartOutText, "git\n");				   // Sygnał końca paczki danych ze wszystkich 9 czujników
//		sendString_UART(UartOutText);


		/*Przesył pod szukanie problemów*/
	//
	//			for(int i = 0; i<6; i++){
	//				sprintf(UartOutText, "Czujnik nr: %d val %2.f%%\n\r ", i+1, moisture_percentage[i]);
	//				sendString_UART(UartOutText);
	//			}
	//
	//			sprintf(UartOutText, "Natezenie oswietlenia: %2.f%% \n\r ", lightIntensity);
	//			sendString_UART(UartOutText);
	//
	//			sprintf(UartOutText, "Temp: %2.f Humidity %2.f \n\r ", Temperature, Humidity);
	//			sendString_UART(UartOutText);


	}



	/*Odczyt wszystkich kanałów analogowych ADC1, ADC_VAL[0-5] to wilgoć gleby, ADC_VAL[6] to czujnik ADC_VAL[6] */
	//	void analogDeviceReadDMA(){
	//
	//			/*Wartość analogowa jest odwrotnie proporcjonalna do procentowego wskaźnika wilgotności, im większe napięcie tym mniejsza wilgoć*/
	//
	//			HAL_ADC_Start_DMA(&hadc1, ADC_VAL, 8); //Odczyt poprzez DMA ze wszystkich kanałów ADC
	//			delay_us(1000);						   //Bez tego delaya się rozlatuje odczyt przez zbyt małą ilość czasu na próbkowanie sygnału analogowego
	//			HAL_ADC_Stop_DMA(&hadc1); 			   //Zatrzymaj odczyt ADC przez DMA
	//
	//			for(int i = 0; i<6; i++){ 			   //Przeskalowanie na procenty (wcześniej test czujnika jak nisko może zejść analogowa wartość w przypadku maksymalnie wilgotnej gleby)
	//				moisture_percentage[i] = 100-( map(ADC_VAL[i], 1000, 3970, 0, 100));
	//
	//				if (moisture_percentage[i]>100){moisture_percentage[i] = 100;} //Przy zwarciu czujnika daje wartość
	//				if (moisture_percentage[i]<0){moisture_percentage[i] = 0;}
	//			}
	//
	//			/*Wartość analogowa jest wprost proporcjonalna do procentowego wskaźnika nasłonecznienia*/
	//			lightIntensity = map(ADC_VAL[6], 150, 4095, 0, 100); // Minimalna wartość jaką się dało uzyskać to 150
	//			sendAllReadingsUART(); 								 //Wyślij wszystko po UART'cie
	//
	//		}


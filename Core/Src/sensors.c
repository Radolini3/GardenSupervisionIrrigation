#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
	/*Moje include'y*/
	#include "stdio.h"
	#include "globalVars.h"
	/*Zdefiniuj PA6*/
	#define PORT_A GPIOA
	#define DHT11_PIN GPIO_PIN_6 //Dla czujnika DHT11 temperatury/wilgotności powietrza (PA6)

/*Zmienne do obsługi czujnika DTH11 */


	/*=============================Ustaw PA6 jako wyjście=============================*/
	void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}
	/*=============================Ustaw PA6 jako wejście=============================*/
	void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}


	/*=============================Inicjalizuj czujnik=============================*/
	void DHT11sensorInit(void){
		Set_Pin_Output (PORT_A, DHT11_PIN); 		// Ustaw PIN jako wyjście
		HAL_GPIO_WritePin (PORT_A, DHT11_PIN, 0);   // Stan niski na pinie
		delay_us (18000);   						// Poczekaj 18ms
	    HAL_GPIO_WritePin (PORT_A, DHT11_PIN, 1);   // Stan wysoki na pinie
		delay_us (20);   							// Poczekaj 20us
		Set_Pin_Input(PORT_A, DHT11_PIN);   		// Ustaw jako wejście
	}


	/*=============================Odpowiedź czujnika=============================*/
	uint8_t sensorResponse(void){
		uint8_t Response = 0;
		delay_us (40);												// Poczekaj 40us
		/*Sprawdź czy czujnik ma odpowiedź, w przeciwnym razie daj błąd*/
		if (!(HAL_GPIO_ReadPin (PORT_A, DHT11_PIN))){
			delay_us (80); 											//Zaczekaj 80us
			if ((HAL_GPIO_ReadPin (PORT_A, DHT11_PIN))) Response = 0;
			else Response = -1; 									// 255
		}
		while ((HAL_GPIO_ReadPin (PORT_A, DHT11_PIN)));     		// Poczekaj na pin aż przejdzie w stan niski

		return Response;
	}

	/*=============================Odczyt=============================*/
	uint8_t sensorRead(void){
		uint8_t i,j;
		for (j=0;j<8;j++)
		{
			while (!(HAL_GPIO_ReadPin (PORT_A, DHT11_PIN)));   // Poczekaj na stan wysoki pinu
			delay_us (40);   								   // Poczekaj 40us
			if (!(HAL_GPIO_ReadPin (PORT_A, DHT11_PIN)))  	   // Jeśli pin jest wysoki
			{
				i&= ~(1<<(7-j));   							   // Zapisz 0 bitowo w rejestrze
			}
			else i|= (1<<(7-j));  							   // Jeśli pin jest w stanie wysokim, zapisz 1
			while ((HAL_GPIO_ReadPin (PORT_A, DHT11_PIN)));    // Poczekaj na stan niski pinu
		}
		return i;
	}
	/*Odczyt z DHT11, inicjalizacja, odpowiedź*/
	void DHT11_allData(){
		  DHT11sensorInit();
		  Presence = sensorResponse();
		  if(Presence == 0){
			  Rh_byte1 = sensorRead ();
			  Rh_byte2 = sensorRead ();
			  Temp_byte1 = sensorRead ();
			  Temp_byte2 = sensorRead ();
			  SUM = sensorRead();
			  TEMP = Temp_byte1;
			  RH = Rh_byte1;
			  Temperature = (float) TEMP;
			  Humidity = (float) RH;
		  }
	}


	/*Odczyt wszystkich kanałów analogowych ADC1, ADC_VAL[0-5] to wilgoć gleby, ADC_VAL[6] to czujnik ADC_VAL[6] */
	void analogDeviceReadDMA(){

			/*Wartość analogowa jest odwrotnie proporcjonalna do procentowego wskaźnika wilgotności, im większe napięcie tym mniejsza wilgoć*/

			HAL_ADC_Start_DMA(&hadc1, ADC_VAL, 7); //Odczyt poprzez DMA ze wszystkich kanałów ADC
			delay_us(3000);						   //Bez tego delaya się rozlatuje odczyt przez zbyt małą ilość czasu na próbkowanie sygnału analogowego
			HAL_ADC_Stop_DMA(&hadc1); 			   //Zatrzymaj odczyt ADC przez DMA

			for(int i = 0; i<sizeof(moisture_percentage)/sizeof(moisture_percentage[0]); i++){ 			   //Przeskalowanie na procenty (wcześniej test czujnika jak nisko może zejść analogowa wartość w przypadku maksymalnie wilgotnej gleby)

				moisture_percentage[i] = 100-( map(ADC_VAL[i], 1000, 3970, 0, 100));

				if (moisture_percentage[i]>100){moisture_percentage[i] = 100;} //Przy zwarciu czujnika daje wartość
				if (moisture_percentage[i]<0){moisture_percentage[i] = 0;}
			}

			/*Wartość analogowa jest wprost proporcjonalna do procentowego wskaźnika nasłonecznienia*/

			lightIntensity = map(ADC_VAL[6], 150, 4095, 0, 100); // Minimalna wartość jaką się dało uzyskać to 150

			//sendAllReadingsUART(); 								 //Wyślij wszystko po UART'cie

		}

//
//
//
//
//		void sendAllReadingsUART(){
//			/*Przesył pod przechwytywanie do formatu csv*/
//
//			for(int i = 0; i<6; i++){
//				sprintf(UartOutText, "%2.f\n", moisture_percentage[i]);
//				sendString_UART(UartOutText);
//			}
//
//			sprintf(UartOutText,"%2.f\n", lightIntensity); //Natężenie światła
//			sendString_UART(UartOutText);
//
//			sprintf(UartOutText, "%2.f\n", Temperature);   //Temperatura z DHT11
//			sendString_UART(UartOutText);
//
//			sprintf(UartOutText, "%2.f\n", Humidity);	   // Wilgotność powietrza
//			sendString_UART(UartOutText);
//
//			sprintf(UartOutText, "git\n");				   // Sygnał końca paczki danych ze wszystkich 9 czujników
//			sendString_UART(UartOutText);
//
//
//			/*Przesył pod szukanie problemów*/
////
////			for(int i = 0; i<6; i++){
////				sprintf(UartOutText, "Czujnik nr: %d val %2.f%%\n\r ", i+1, moisture_percentage[i]);
////				sendString_UART(UartOutText);
////			}
////
////			sprintf(UartOutText, "Natezenie oswietlenia: %2.f%% \n\r ", lightIntensity);
////			sendString_UART(UartOutText);
////
////			sprintf(UartOutText, "Temp: %2.f Humidity %2.f \n\r ", Temperature, Humidity);
////			sendString_UART(UartOutText);
//
//
//		}



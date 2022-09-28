/*
 * globalVars.h
 *
 *  Created on: Sep 18, 2022
 *      Author: HP
 */

#ifndef SRC_GLOBALVARS_H_
#define SRC_GLOBALVARS_H_

	/*Zmienne przechowujące bufor dla przesyłania UART'em*/
	extern char UartOutText[100], UartInput[5];
	/*ADC zmienna*/
	extern const short numberOfADCChannels;
	extern uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
	extern uint16_t SUM, RH, TEMP;
	extern uint8_t Presence;

	extern uint32_t ADC_VAL[7];
	extern float moisture_percentage[6];
	extern float lightIntensity;

	extern float Temperature;
	extern float Humidity;
	extern float lightIntensity;

	extern int disp_No;
	extern struct lcd_disp disp;
	extern int count;

	extern short sensorRead_freq;
	extern short pushCount;

	extern short numberOfDisplays;



#endif /* SRC_GLOBALVARS_H_ */



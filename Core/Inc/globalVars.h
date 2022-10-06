#ifndef SRC_GLOBALVARS_H_
#define SRC_GLOBALVARS_H_

	extern char UartOutText[100], UartInput[5];

	extern uint32_t ADC_VAL[8];
	extern float moisture_percentage[6];


	extern float Temperature;
	extern float Humidity;


	extern int disp_No;
	extern struct lcd_disp disp;
	extern int count;

	extern short sensorRead_freq;
	extern short pushCount;

#endif /* SRC_GLOBALVARS_H_ */


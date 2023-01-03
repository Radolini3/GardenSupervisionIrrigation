#include "main.h"
#include "lcd_i2c.h"
#include "stm32l4xx_hal.h"
#include "i2c.h"

#include "globalVars.h"
#include "stdio.h"
#include "string.h"

void lcd_init(struct lcd_disp * lcd){
	uint8_t xpin = 0;
	/*podświetlenie*/
	if(lcd->bl){
		xpin = BL_PIN;
	}

	/*inicjalizacja*/
	delay_us(40000);
	lcd_write(lcd->addr, INIT_8_BIT_MODE, xpin);
	delay_us(5000);
	lcd_write(lcd->addr, INIT_8_BIT_MODE, xpin);
	delay_us(10000);
	lcd_write(lcd->addr, INIT_8_BIT_MODE, xpin);

	/*tryb 4 bitowy*/
	lcd_write(lcd->addr, INIT_4_BIT_MODE, xpin);

	/*ustaw tryb kursora*/
	lcd_write(lcd->addr, UNDERLINE_OFF_BLINK_OFF, xpin);

	/* clear */
	lcd_clear(lcd);

}

void lcd_write(uint8_t addr, uint8_t data, uint8_t xpin){
	uint8_t tx_data[4];

	/*podziel dane*/
	tx_data[0] = (data & 0xF0) | EN_PIN | xpin;
	tx_data[1] = (data & 0xF0) | xpin;
	tx_data[2] = (data << 4) | EN_PIN | xpin;
	tx_data[3] = (data << 4) | xpin;

	/*wyślij dane po i2c*/
	HAL_I2C_Master_Transmit(&HI2C_DEF, addr, tx_data, 4, 100);

	delay_us(750);
}

void lcd_display(struct lcd_disp * lcd){
	uint8_t xpin = 0, i = 0;

	/*ustaw podswietlenie*/
	if(lcd->bl){
		xpin = BL_PIN;
	}

	lcd_clear(lcd);

	/*wyslij 1 linie*/
	lcd_write(lcd->addr, FIRST_CHAR_LINE_1, xpin);

	while(lcd->f_line[i]){
		lcd_write(lcd->addr, lcd->f_line[i], (xpin | RS_PIN));
		i++;
	}

	/*wyslij 2 linie*/
	i = 0;
	lcd_write(lcd->addr, FIRST_CHAR_LINE_2, xpin);

	while(lcd->s_line[i]){
		lcd_write(lcd->addr, lcd->s_line[i], (xpin | RS_PIN));
		i++;
	}
}

void lcd_clear(struct lcd_disp * lcd)
{
	uint8_t xpin = 0;

	/*ustaw podswietlenie*/
	if(lcd->bl){
		xpin = BL_PIN;
	}

	/*wyczysc lcd*/
	lcd_write(lcd->addr, CLEAR_LCD, xpin);
}
void displayReadings(int disp_No){ /* wszystkie pomiary jako case'y zmieniane z przycisku*/
	switch(disp_No){
	case 1:
		sprintf((char *)disp.f_line, "Air T: %2.f degC", Temperature);
		sprintf((char *)disp.s_line, "Air H: %2.f%% ", Humidity);
		break;
	case 2:
		sprintf((char *)disp.f_line, "SoilSens 1: %2.f%%", moisture_percentage[0]);
		sprintf((char *)disp.s_line, "SoilSens 2: %2.f%%", moisture_percentage[1]);
		break;
	case 3:
		sprintf((char *)disp.f_line, "SoilSens 3: %2.f%%", moisture_percentage[2]);
		sprintf((char *)disp.s_line, "SoilSens 4: %2.f%%", moisture_percentage[3]);
		break;
	case 4:
		sprintf((char *)disp.f_line, "SoilSens 5: %2.f%%", moisture_percentage[4]);
		sprintf((char *)disp.s_line, "SoilSens 6: %2.f%%", moisture_percentage[5]);
		break;
	case 5:
		sprintf((char *)disp.f_line, "SoilAvg: %d%%", moistureAverage);
		if (waterMode == 0){
			sprintf((char *)disp.s_line, "Mode: Manual");
			}
				else
					sprintf((char *)disp.s_line, "Mode: Auto");
		break;
	case 6:
		sprintf((char *)disp.f_line, "MoistThres: %d%%", moistureThreshold);
		sprintf((char *)disp.s_line, "IrrigTime: %dmin", waterTimeMins);
		break;
	default:
		break;
	}
	lcd_display(&disp);
}

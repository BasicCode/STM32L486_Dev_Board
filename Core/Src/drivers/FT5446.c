/*
 * FT5446.c
 *	A small driver for the FT5446 capacative touch panel (CTP) controller chip.
 *
 *  Created on: Jun 25, 2022
 *      Author: tommy
 */

#include "drivers/FT5446.h"

/**
 * Initialisation routine for the FT5446 module
 */
void FT5446_init() {
	//set auto calibration. The datasheet isn't really clear how this works
	HAL_I2C_Mem_Write(&hi2c1, FT5446_ADDR << 1, FT5446_CALIB_ADDR, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 250);
	//Enter auto calibration mode
	HAL_I2C_Mem_Write(&hi2c1, FT5446_ADDR << 1, FT5446_STATE_ADDR, I2C_MEMADD_SIZE_8BIT, FT5446_AUTO_CALIB, 1, 250);

	//some mystery undocumented delay
	HAL_Delay(100);
	//Return to normal mode
	HAL_I2C_Mem_Write(&hi2c1, FT5446_ADDR << 1, FT5446_STATE_ADDR, I2C_MEMADD_SIZE_8BIT, FT5446_WORK_MODE, 1, 250);
}

struct Touch FT5446_getTouch() {
	struct Touch touch;
	unsigned char touchData[6];

	//Get number of touches
	//Wait for device to be ready
	if(HAL_I2C_IsDeviceReady (&hi2c1, FT5446_ADDR << 1, 10, 250) == HAL_OK) {
		//Get the CTP ram
		HAL_I2C_Mem_Read (&hi2c1, FT5446_ADDR << 1, FT5446_GESTURE_ADDR, I2C_MEMADD_SIZE_8BIT, touchData, 6, 250);
		//Combine in to regular human numbers
		touch.Y = ((touchData[2] & 0x0F) << 8) | touchData[3];
		touch.X = ((touchData[4] & 0x0F) << 8) | touchData[5];

		//Get the gesture and n touches values
		touch.gesture = touchData[0];
		touch.nTouches = touchData[1];
		touch.state = touchData[2] >> 6;
	}

	return touch;
}

char FT5446_FindDevice() {
	  for(char i = 0; i < 128; i++) {
	  	if(HAL_I2C_IsDeviceReady (&hi2c1, i << 1, 10, 250) == HAL_OK)
	  		return i;
	  }

	  return 0;
}

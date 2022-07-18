/*
 * FT5446.h
 *
 *  Created on: Jun 25, 2022
 *      Author: tommy
 */

#ifndef INC_FT5446_H_
#define INC_FT5446_H_

#include "main.h"


//The device I2C address
#define FT5446_ADDR			0x38
//Some relevant RAM addresses
#define FT5446_GESTURE_ADDR	0x01
#define FT5446_STATUS_ADDR	0x02
#define FT5446_TOUCH1_ADDR	0x03

//Data for one touch
struct Touch {
	int X;
	int Y;
	unsigned char gesture; //Gesture reported by the controller
	unsigned char nTouches; //Number of touches
	unsigned char state; //Touch, or release
};

//Chose your I2C port here
extern I2C_HandleTypeDef hi2c1;

//Function prototypes
struct Touch FT5446_getTouch();
char FT5446_findDevice();

#endif /* INC_FT5446_H_ */

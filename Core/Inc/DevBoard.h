/*
 * DevBoard.h
 *
 *  Created on: 31 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DEVBOARD_H_
#define INC_DEVBOARD_H_

#include "main.h"
#include "drivers/DS3231.h"

extern struct Time time;
extern char timeString[9];

//Screen handles, and identifiers - List your screens here
typedef enum {
	DEVICE_TEST,
	HOME,
	MAIN_MENU,
	SETTINGS
} Screens;


//Screen dimensions
#define WIDTH 480
#define HEIGHT 320

//Direction of some elements
typedef enum {
	HORIZONTAL,
	VERTICAL
} Orientation;


//Handles for threads which need to receive signals from external sources.
extern osThreadId touchTaskHandle;
extern osThreadId changeScreenTaskHandle;

void DrawTask(void const * argument);
void ChangeScreenTask(void const * argument);
void TouchTask(void const * argument);
void TimeTask(void const * argument);

void home_onPress(int id);

#endif /* INC_DEVBOARD_H_ */

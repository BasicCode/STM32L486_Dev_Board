/*
 * DevBoard.h
 *
 *  Created on: 31 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DEVBOARD_H_
#define INC_DEVBOARD_H_

#include "main.h"
#include "cmsis_os.h"
#include "drivers/DS3231.h"

//Mutexes
extern osMutexId I2CMutex_ID;

//Global variables
extern struct Time time;
extern char timeString[9];

//A timer linked list item. Contains the timer data, and a pointer to the next timer in the list.
struct Timer {
	struct Time		startTime;	//Starting time/date for this timer
	unsigned int	duration;	//Duration (seconds) to run the timer
	char			enabled;	//Enable / disable flag
	struct Timer	*nextTimer;	//Pointer to the next node
};

//Screen handles, and identifiers - List your screens here
typedef enum {
	DEVICE_TEST,
	HOME,
	MAIN_MENU,
	SETTINGS,
	CHANGE_TIME_DATE,
	EDIT_TIMER,
	TIMER_LIST
} Screens;


//Screen dimensions
#define WIDTH 479
#define HEIGHT 319

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

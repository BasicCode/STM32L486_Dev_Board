/*
 * DevBoard.c
 * Tasks associated with running the STM32L486 Dev Board.
 *
 *  Created on: 31 Jul. 2022
 *      Author: tommy
 */

#include "DevBoard.h"
#include "cmsis_os.h"
#include "DisplayManager/DisplayManager.h"
#include "colours.h"
#include "stdio.h"
#include "drivers/FT5446.h"
#include "drivers/DS3231.h"

//Individual tasks to display
#include "screens/DeviceTestScreen.h"
#include "screens/HomeScreen.h"
#include "screens/SettingsTaskScreen.h"
#include "screens/ChangeTimeScreen.h"
#include "screens/EditTimerScreen.h"
#include "screens/TimerListScreen.h"

//Thread handles for threads which always run
osThreadId drawTaskHandle; //Handle to the task which re-draws the display
osThreadId currentScreenHandle; //Handle to the thread drawing the current screen
osThreadId changeScreenTaskHandle; //Thraed which handles screen changes
osThreadId touchTaskHandle; //Thread which handles touch events
osThreadId timeTaskHandle; //Reads the DS3231 RTC chip and updates the global time variable

//Global TIME variable and string
struct Time time;
char timeString[9];

//Global array of Timers
struct Timer timers[64];

//Mutexes
osMutexId I2CMutex_ID;
osMutexId touchMutex_ID;

/**
 * Initialises the threads which run the various peripherals on this board.
 */
void DevBoardBegin() {

	/*
	 * Mutexes
	 */
	//I2C needs a mutex because it's shared by the RTC and CTP
	osMutexDef(I2CMutex);
	I2CMutex_ID = osMutexCreate(osMutex(I2CMutex));

	/*
	 * Tasks
	 */
	//The draw task handles display updates. It should be a HIGH priority for fast refresh.
	osThreadDef(drawTask, DrawTask, osPriorityHigh, 0, 1024);
	drawTaskHandle = osThreadCreate(osThread(drawTask), NULL);

	//Task which handles switching screens
	osThreadDef(changeScreenTask, ChangeScreenTask, osPriorityNormal, 0, 128);
	changeScreenTaskHandle = osThreadCreate(osThread(changeScreenTask), NULL);

	//This task handles touching
	osThreadDef(touchTask, TouchTask, osPriorityNormal, 0, 512);
	touchTaskHandle = osThreadCreate(osThread(touchTask), NULL);

	//Get the current time from the RTC chip
	osThreadDef(timeTask, TimeTask, osPriorityNormal, 0, 128);
	timeTaskHandle = osThreadCreate(osThread(timeTask), NULL);

	return;
}

/**
 * The Draw task handles display updates.
 * It just calls the DisplayManager draw function which decides
 * which elements need refreshing.
 */
void DrawTask(void const * argument)
{
	for(;;)
	{
		//Do a display refresh
		DM_Draw();
		//This is a HIGH priority task so we have to hand control back to the OS.
		//Also sets the refresh rate
		osDelay(100);
	}
}

/**
 * Function which receives signals, usually from button callbacks, and changes
 * the display to the desired screen.
 */
void ChangeScreenTask(void const * arguments) {

	//Any screens you want to display
	osThreadDef(deviceTestTask, DeviceTestTask, osPriorityNormal, 0, 1024);
	osThreadDef(mainMenuTask, MainMenuTask, osPriorityNormal, 0, 512);
	osThreadDef(splashScreenTask, SplashScreenTask, osPriorityNormal, 0, 256);
	osThreadDef(settingsTask, SettingsTask, osPriorityNormal, 0, 512);
	osThreadDef(changeTimeTask, ChangeTimeTask, osPriorityNormal, 0, 512);
	osThreadDef(editTimerTask, EditTimerTask, osPriorityNormal, 0, 1024);
	osThreadDef(timerListTask, TimerListTask, osPriorityNormal, 0, 1024);

	//Initial entry screen
	currentScreenHandle = osThreadCreate(osThread(mainMenuTask), NULL);

	int signal = 0;
	int event;
	while(1) {
			event = xTaskNotifyWait(0x00, 0xFFFF, &signal, 0);

			if(event == pdTRUE) {

				//Clear the display elements
				DM_Clear();

				//End the current screen thread
				osThreadTerminate(currentScreenHandle);

				//Load the desired screen
				if(signal == MAIN_MENU)
					currentScreenHandle = osThreadCreate(osThread(mainMenuTask), NULL);

				if(signal == DEVICE_TEST)
					currentScreenHandle = osThreadCreate(osThread(deviceTestTask), NULL);

				if(signal == SETTINGS)
					currentScreenHandle = osThreadCreate(osThread(settingsTask), NULL);

				if(signal == CHANGE_TIME_DATE)
					currentScreenHandle = osThreadCreate(osThread(changeTimeTask), NULL);

				if(signal == EDIT_TIMER)
					currentScreenHandle = osThreadCreate(osThread(editTimerTask), NULL);

				if(signal == TIMER_LIST)
					currentScreenHandle = osThreadCreate(osThread(timerListTask), NULL);

	    }

		//Give the OS time to do some other tasks
		osThreadYield();
	}
}

/**
 * Handles touch events and reports them to the display manager.
 */
void TouchTask(void const * argument) {
	osEvent evt;
	struct Touch touch;
	int event;
	int signal;
	int I2CMutexStatus;

	for(;;) {
	    // wait for a signal
	    event = xTaskNotifyWait(0x00, 0xFFFF, &signal, 0);

	    //Signal received, get the touch coordinates
	    if (event == pdTRUE)  {

	    	//Check the peripheral is releated by the OS
	        I2CMutexStatus  = osMutexWait(I2CMutex_ID, 250);
	        if (I2CMutexStatus == osOK)  {
	            //Get the most recent touch point
				touch = FT5446_getTouch();
	        }
	        //Release the I2C mutex
	        osMutexRelease(I2CMutex_ID);

	    	//Envoke the pressed element
			DM_Do_Press(touch);
	    }

	    osThreadYield();
	}
}

/**
 * Gets the time from the DS3231 RTC and updates the global time
 */
void TimeTask(void const * argument) {
	int mutexStatus;

	while(1) {
    	//Take the mutex and prevent anything else from touching the bus
        mutexStatus  = osMutexWait(I2CMutex_ID, 250);
        if (mutexStatus == osOK)  {
			//Get the time from the RTC once per second
			time = RTC_get_time_date();
        }
        mutexStatus = osMutexRelease(I2CMutex_ID);

		sprintf(timeString, "%2d%2d%2d", time.hours, time.minutes, time.seconds);

		//Hand control back to the RTOS
		osDelay(1000);
	}
}

/**
 * Callback for any button that wants to return HOME
 */
void home_onPress(int id) {
	//let the OS know to change screens
	xTaskNotify(changeScreenTaskHandle, MAIN_MENU, eSetValueWithOverwrite);

	return;
}

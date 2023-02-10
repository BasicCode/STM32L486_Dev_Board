/*
 * HomeScreen.c
 *	Example home screen using my home made Display Manager.
 *	Designed for use as a task in freeRTOS.
 *
 *  Created on: Jun 25, 2022
 *      Author: tommy
 */

#include "screens/HomeScreen.h"
#include "DevBoard.h"

//Display Manager
#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/Button.h"
#include "DisplayManager/Bitmap.h"
#include "DisplayManager/Animation.h"
#include "DisplayManager/Fill.h"
#include "DisplayManager/Text.h"
#include "DisplayManager/Animation.h"
#include "DisplayManager/TickBox.h"

#include "colours.h"
#include "drivers/DS3231.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/numbers.h"
#include "bitmaps/sse.h"
#include "bitmaps/loading.h"

//Private functions
void settingsScreen_onPress(int id);
unsigned int* Char_To_Bmp(char c);

//Where the human-readable date string is stored.
char fancyDateString[32];
int dateTextId;

/**
 * The main display which is currently a clock. Called Main Menu
 * for historical reasons. Is not actually a menu.
 */
void MainMenuTask(void const * arguments) {

	const unsigned int textColour = COLOR_LIGHTGRAY;

	//Put the current date at the top of the screen
	sprintf(fancyDateString, "%s %d / %d / %d", dayName[time.weekday], time.day, time.month, time.year);
	dateTextId = DM_Add_Element(DM_New_Fast_Text(150, 10, textColour, COLOR_WHITE, fancyDateString));

	//Double gradient effect
	DM_Add_Element(DM_New_Fill_Gradient(0, 90, WIDTH, 120, COLOR_WHITE, COLOR_BLACK, VERTICAL));
	DM_Add_Element(DM_New_Fill_Rectangle(0, 120, WIDTH, 184, COLOR_BLACK));
	DM_Add_Element(DM_New_Fill_Gradient(0, 184, WIDTH, 214, COLOR_BLACK, COLOR_WHITE, VERTICAL));

	//Draw some digits
	const int xMargin = 16;
	const int digitWidth = 64;
	const int colonWidth = 16;

	struct DisplayElement digit1 = DM_New_Bitmap(xMargin, 120, 1, num_1);
	struct DisplayElement digit2 = DM_New_Bitmap(xMargin + digitWidth, 120, 1, num_5);
	struct DisplayElement colon1 = DM_New_Bitmap(xMargin + (digitWidth * 2) - colonWidth, 120, 1, colon);
	struct DisplayElement digit3 = DM_New_Bitmap(xMargin + (2 * digitWidth) + (2 * colonWidth), 120, 1, num_2);
	struct DisplayElement digit4 = DM_New_Bitmap(xMargin + (3 * digitWidth) + (2 * colonWidth), 120, 1, num_4);
	struct DisplayElement colon2 = DM_New_Bitmap(xMargin + (digitWidth * 4) + colonWidth, 120, 1, colon);
	struct DisplayElement digit5 = DM_New_Bitmap(xMargin + (5 * digitWidth), 120, 1, num_5);
	struct DisplayElement digit6 = DM_New_Bitmap(xMargin + (6 * digitWidth), 120, 1, num_7);


	int digit1Id = DM_Add_Element(digit1);
	int colon1Id = DM_Add_Element(colon1); //colon
	int digit2Id = DM_Add_Element(digit2);

	int digit3Id = DM_Add_Element(digit3);
	int colon2Id = DM_Add_Element(colon2); //colon
	int digit4Id = DM_Add_Element(digit4);

	int digit5Id = DM_Add_Element(digit5);
	int digit6Id = DM_Add_Element(digit6);

	//Button for settings
	struct DisplayElement button2 = DM_New_Button(BTN_MIDDLE_X, BTN_BOTTOM_Y, "MENU", ENABLED);
	button2.onPress = settingsScreen_onPress;
	DM_Add_Element(button2);

	//Previous time to compare to new time and decide to update
	char oldMin = '0';

	//The main loop
	while(1) {
		//Update the bitmaps with the new time
		//check if we need to update the whole lot, or just seconds
		if(oldMin != timeString[3]) {
			digit1.bitmap = Char_To_Bmp(timeString[0]);
			digit2.bitmap = Char_To_Bmp(timeString[1]);
			DM_Replace_Element(digit1Id, digit1);
			DM_Replace_Element(colon1Id, colon1);
			DM_Replace_Element(digit2Id, digit2);

			digit3.bitmap = Char_To_Bmp(timeString[2]);
			digit4.bitmap = Char_To_Bmp(timeString[3]);
			DM_Replace_Element(digit3Id, digit3);
			DM_Replace_Element(colon2Id, colon2);
			DM_Replace_Element(digit4Id, digit4);

			//Update the date string and register the text for update
			sprintf(fancyDateString, "%s %d / %d / %d", dayName[time.weekday], time.day, time.month, time.year);
			DM_Refresh_Element(dateTextId);
		}
		//Update the seconds componetnts
		digit5.bitmap = Char_To_Bmp(timeString[4]);
		digit6.bitmap = Char_To_Bmp(timeString[5]);
		DM_Replace_Element(digit5Id, digit5);
		DM_Replace_Element(digit6Id, digit6);

		//Update the old minutes flag for the next update
		oldMin = timeString[3];

		osDelay(1000);
	}
}

/**
 * onPress callback for the settings screen button
 */
void settingsScreen_onPress(int id) {
	xTaskNotify(changeScreenTaskHandle, TIMER_LIST, eSetValueWithOverwrite);
}

/**
 * Displays a splash screen then diverts to the main menu
 */
void SplashScreenTask(void const * arguments) {
	//The SSE Logo
	struct DisplayElement splashHandle = DM_New_Bitmap(60, 20, 1, sse);
	DM_Add_Element(splashHandle);
	//Loading animation
	int loadingAnimation = DM_Add_Element(DM_New_Animation(208, 300, 1, myAnimation, 3));

	//Reset the MP3 player module
	DFPlayer_resetModule();
	//Wait for the device to reset... It's slow
	osDelay(1000);

	//switch to the main menu
	xTaskNotify(changeScreenTaskHandle, MAIN_MENU, eSetValueWithOverwrite);

	while(1);
}

/**
 * Converts a char to a bitmap represnting that number.
 */
unsigned int* Char_To_Bmp(char c) {
	switch(c) {
	case '0':
		return num_0;
	case '1':
		return num_1;
	case '2':
		return num_2;
	case '3':
		return num_3;
	case '4':
		return num_4;
	case '5':
		return num_5;
	case '6':
		return num_6;
	case '7':
		return num_7;
	case '8':
		return num_8;
	case '9':
		return num_9;
	}

	return num_0;
}


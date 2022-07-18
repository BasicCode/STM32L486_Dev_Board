/*
 * DeviceTestScreen.c
 *
 * A task which checks communication with the peripherals on this
 * board and reports their status.
 *
 *  Created on: 17 Jul. 2022
 *      Author: tommy
 */

#include "screens/DeviceTestScreen.h"
#include "drivers/DisplayManager.h"
#include "colours.h"
#include "drivers/FT5446.h"
#include "drivers/DFPlayer.h"
#include "drivers/DS3231.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/loading.h"

//Private functions
void OK_Press(int id);

/**
 * The main task.
 */
void DeviceTestTask(void const * arguments) {

	const unsigned int textColour = COLOR_BLACK;

	//Add some dummy display elements
	DM_Add_Element(DM_New_Title_Bar("Device Test"));

	//Show a loding animation while we test peripherals
	int loadingText = DM_Add_Element(DM_New_Text(208, 160, textColour, 1, "Loading..."));
	int loadingAnimation = DM_Add_Element(DM_New_Animation(218, 180, 1, myAnimation, 3));

	//Reset the MP3 player module
	DFPlayer_resetModule();
	//Wait for the device to reset... It's slow
	osDelay(1000);
	//Test communication with the player
    int status = DFPlayer_getStatus();
    int timeout = 10;
    int songs = 0;
    while(timeout--) {
    	songs = DFPlayer_getTracksInFolder(0);
    	if(songs > 0)
    		break;
    }
    char numSongs[64];
    sprintf(numSongs, "DFPlayer Status: %d, with %d songs.", status, songs);


	//Make a list of I2C devices
	char deviceString[128] = "I2C Devices:";
	for(char i = 0; i < 128; i++) {
		if(HAL_I2C_IsDeviceReady (&hi2c1, i << 1, 10, 250) == HAL_OK) {
			sprintf(deviceString, "%s %d", deviceString, i);
		}
	}

	//Confirm communication with the RTC
	struct Time thisTime = RTC_get_time_date();
	char timeDateString[128];
	sprintf(timeDateString, "%s %d / %d / %d %d:%d:%d", dayName[thisTime.weekday], thisTime.day,
			thisTime.month, thisTime.year, thisTime.hours, thisTime.minutes, thisTime.seconds);


	//List of I2C devices found
	DM_Add_Element(DM_New_Text(10, 45, textColour, 1, deviceString));
	//List tracks on MP3
	DM_Add_Element(DM_New_Text(10, 60, textColour, 1, numSongs));
	//List current time on the clock
	DM_Add_Element(DM_New_Text(10, 75, textColour, 1, timeDateString));

	//Remove the loading animations
	DM_Remove_Element(loadingText);
	DM_Remove_Element(loadingAnimation);

	//OK button
	struct DisplayElement okBtn = DM_New_Button(BTN_RIGHT_X, BTN_BOTTOM_Y, "BACK", ENABLED);
	okBtn.onPress = OK_Press;
	int okBtnId = DM_Add_Element(okBtn);

	while(1) {
		osDelay(1000);
	}
}

void OK_Press(int id) {
	//Stop the music playing
	DFPlayer_setPause();

	//let the OS know to change screens
	xTaskNotify(changeScreenTaskHandle, MAIN_MENU, eSetValueWithOverwrite);
}

/**
 * Screen which displays, edits, and saves settings for the device.
 */


#include "screens/SettingsTaskScreen.h"
#include "DevBoard.h"
#include "colours.h"
//Display Elements
#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/NumPad.h"
#include "DisplayManager/Button.h"
#include "DisplayManager/Text.h"
#include "DisplayManager/TextBox.h"
#include "DisplayManager/TitleBar.h"
#include "DisplayManager/Animation.h"
#include <DisplayManager/Fill.h>

#include "bitmaps/loading.h"

#include "drivers/DS3231.h"

//Private functino prototypes
void deviceTest_onPress(int id);
void changeTime_onPress(int id);

/**
 * Main task to display the settings screen.
 */
void SettingsTask(void const * args) {
	//Add some dummy display elements
	DM_Add_Element(DM_New_Title_Bar("Settings"));

	//HOME
	struct DisplayElement okBtn = DM_New_Button(BTN_LEFT_X, BTN_BOTTOM_Y, "BACK", ENABLED);
	okBtn.onPress = home_onPress;
	DM_Add_Element(okBtn);

	//Change time/date button
	struct DisplayElement changeTimeBtn = DM_New_Button(BTN_LEFT_X, 45, "Change Time/Date", ENABLED);
	changeTimeBtn.onPress = changeTime_onPress;
	DM_Add_Element(changeTimeBtn);


	//Button for the device test
	struct DisplayElement button1 = DM_New_Button(BTN_LEFT_X, 100, "Test Device", ENABLED);
	button1.onPress = deviceTest_onPress;
	DM_Add_Element(button1);

	while(1);
}

/**
 * Goes to the change time/date screen
 */
void changeTime_onPress(int id) {
	//Go to the change time/date screen
	xTaskNotify(changeScreenTaskHandle, CHANGE_TIME_DATE, eSetValueWithOverwrite);

	return;
}



/**
 * Callback for the Test Device button
 */
void deviceTest_onPress(int id) {

	//let the OS know to change screens
	xTaskNotify(changeScreenTaskHandle, DEVICE_TEST, eSetValueWithOverwrite);
}

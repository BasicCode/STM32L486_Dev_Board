/**
 * Displays options to change the time and date on the RTC module.
 */


#include "screens/ChangeTimeScreen.h"
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


void save_onPress(int id);

//The text boxes which will store the new time and date
struct DisplayElement hrsBox;
struct DisplayElement minsBox;
struct DisplayElement dayBox;
struct DisplayElement monthBox;
struct DisplayElement yearBox;


/**
 * Main task to display the settings screen.
 */
void ChangeTimeTask(void const * args) {
	//Some default values
	int textColour = COLOR_GRAY;
	char hrs[3] = "\0  ";
	char mins[3] = "\0  ";
	char day[3] = "\0  ";
	char month[3] = "\0  ";
	char year[5] = "\0    ";

	//Add some dummy display elements
	DM_Add_Element(DM_New_Title_Bar("Change Time/Date"));

	//HOME
	struct DisplayElement okBtn = DM_New_Button(BTN_LEFT_X, BTN_BOTTOM_Y, "BACK", ENABLED);
	okBtn.onPress = home_onPress;
	int okBtnId = DM_Add_Element(okBtn);

	//Save button
	struct DisplayElement saveBtn = DM_New_Button(BTN_MIDDLE_X, BTN_BOTTOM_Y, "SAVE", ENABLED);
	saveBtn.onPress = save_onPress;
	DM_Add_Element(saveBtn);

	//The current time in text boxes
	DM_Add_Element(DM_New_Text(10, 45, textColour, 1, "Time:"));
	hrsBox = DM_New_TextBox(10, 55, 2, "HH", hrs);
	sprintf(hrsBox.text, "%d", time.hours);
	DM_Add_Element(hrsBox);
	minsBox = DM_New_TextBox(60, 55, 2, "MM", mins);
	sprintf(minsBox.text, "%d", time.minutes);
	DM_Add_Element(minsBox);

	//Current date text boxes
	DM_Add_Element(DM_New_Text(10, 95, textColour, 1, "Date:"));
	dayBox = DM_New_TextBox(10, 105, 2, "DD", day);
	sprintf(dayBox.text, "%d", time.day);
	DM_Add_Element(dayBox);
	monthBox = DM_New_TextBox(60, 105, 2, "MM", month);
	sprintf(monthBox.text, "%d", time.month);
	DM_Add_Element(monthBox);
	yearBox = DM_New_TextBox(110, 105, 4, "YYYY", year);
	sprintf(yearBox.text, "%d", time.year);
	DM_Add_Element(yearBox);


	while(1);
}

/**
 * Saves the selected time to the RTC
 */
void save_onPress(int id) {
	//Get the time as actual numbers from the text boxes
	int hrs = atoi(hrsBox.text);
	int mins = atoi(minsBox.text);
	int secs = 0;
	int day = atoi(dayBox.text);
	int month = atoi(monthBox.text);
	int year = atoi(yearBox.text);

	//Get the current time, then update the struct
	struct Time thisTime = RTC_get_time_date();
	//Change the old time to the new time
	thisTime.hours = hrs;
	thisTime.minutes = mins;
	thisTime.seconds = secs;
	thisTime.day = day;
	thisTime.month = month;
	thisTime.year = year;

	//Check bounds and Send it to the device
	RTC_update_clock(checkTimeBounds(thisTime));

	//Return to main menu
	xTaskNotify(changeScreenTaskHandle, MAIN_MENU, eSetValueWithOverwrite);

	return;
}




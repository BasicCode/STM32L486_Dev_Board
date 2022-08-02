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

void numPad_onPress(int id);

char hours[3] = "";
char mins[3] = "";
char secs[3] = "";
int hoursBoxId;
int minsBoxId;
int secsBoxId;


/**
 * Main task to display the settings screen.
 */
void SettingsTask(void const * args) {
	//Clear the text from previous sessions
	hours[0] = '\0';
	mins[0] = '\0';
	secs[0] = '\0';

	//Add some dummy display elements
	DM_Add_Element(DM_New_Title_Bar("Settings"));

	//HOME
	struct DisplayElement okBtn = DM_New_Button(BTN_LEFT_X, BTN_BOTTOM_Y, "BACK", ENABLED);
	okBtn.onPress = home_onPress;
	int okBtnId = DM_Add_Element(okBtn);

	//draw a number pad
	//struct DisplayElement numPad = DM_New_NumPad();
	//numPad.onPress = numPad_onPress;
	//DM_Add_Element(numPad);

	//A text box to put the numbers in
	hoursBoxId = DM_Add_Element(DM_New_TextBox(10, 45, 2, "HH", hours));
	minsBoxId = DM_Add_Element(DM_New_TextBox(60, 45, 2, "MM", mins));
	secsBoxId = DM_Add_Element(DM_New_TextBox(110, 45, 2, "SS", secs));

	while(1);
}

/**
 * Screen which displays, edits, and saves settings for the device.
 */

#include <DisplayManager/Fill.h>
#include "screens/SettingsTaskScreen.h"
#include "colours.h"
//Display Elements
#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/NumPad.h"
#include "DisplayManager/Button.h"
#include "DisplayManager/Text.h"
#include "DisplayManager/TitleBar.h"

void numPad_onPress(int id);

char text[64] = "";
int textBoxId;
int textBlankId;

/**
 * Main task to display the settings screen.
 */
void SettingsTask(void const * args) {
	//Clear the text from previous sessions
	text[0] = '\0';

	//Add some dummy display elements
	DM_Add_Element(DM_New_Title_Bar("Settings"));
	//HOME
	struct DisplayElement okBtn = DM_New_Button(BTN_LEFT_X, BTN_BOTTOM_Y, "BACK", ENABLED);
	okBtn.onPress = home_onPress;
	int okBtnId = DM_Add_Element(okBtn);

	//draw a number pad
	struct DisplayElement numPad = DM_New_NumPad();
	numPad.onPress = numPad_onPress;
	DM_Add_Element(numPad);

	//A blank area to clear the text box - will make this part of the actual element later
	struct DisplayElement blankBox = DM_New_Fill_Rectangle(0, 45, 260, 70, COLOR_WHITE);
	textBlankId = DM_Add_Element(blankBox);
	//A text box to put the numbers in
	struct DisplayElement textBox = DM_New_Text(0, 45, COLOR_BLUE, 2, text);
	textBoxId = DM_Add_Element(textBox);

	while(1);
}

/**
 * Callback from the numpad for digits to input
 */
void numPad_onPress(int id) {
	int num = DM_Get_Element(id).selected;
	//The numbers count up from 1, with 0 being reserved as NULL.
	//So the number zero is actually 11, and backspace is 12.
	if(num == 11)
		num = 0;
	//If the num is 12 then it's a backspace, otherwise it's a digit
	if(num == 12) {
		//Change the last character to null
		int len = strlen(text);
		text[len - 1] = '\0';
	} else {
		//Calculate the new char and add it to the text string
		char c = num + 48;
		strncat(text, &c, 1);
	}

	//Register the blanking square, and the text element for update.
	DM_Refresh_Element(textBlankId);
	DM_Refresh_Element(textBoxId);
}

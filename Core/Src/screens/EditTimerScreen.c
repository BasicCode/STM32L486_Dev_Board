/*
 * EditTimerScreen.c
 *
 * Screen which displays the UI to edit a timer
 *
 *  Created on: 27 Sep. 2022
 *      Author: tommy
 */

#include "screens/EditTimerScreen.h"
#include "DevBoard.h"



//Display Manager
#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/NumPad.h"
#include "DisplayManager/Button.h"
#include "DisplayManager/Text.h"
#include "DisplayManager/TextBox.h"
#include "DisplayManager/TitleBar.h"
#include "DisplayManager/Animation.h"
#include "DisplayManager/TickBox.h"

#include "colours.h"

//Private functions



/**
 * Entry point for the Edit Time screen.
 */
void EditTimerTask(void const * arguments) {
	//Some default colours
	const int textColour = COLOR_BLACK;

	//Variables used throughout the page
	char hrs[3] = "\0  ";
	char mins[3] = "\0  ";
	char secs[3] = "00\0";
	char duration[4] = "\0   ";

	//The text boxes which will store the new time and date
	struct DisplayElement hrsBox;
	struct DisplayElement minsBox;
	struct DisplayElement secsBox;
	struct DisplayElement durationBox;

	//Start with the screen elements
	DM_Add_Element(DM_New_Title_Bar("Edit Timer"));

	//HOME
	struct DisplayElement okBtn = DM_New_Button(BTN_LEFT_X, BTN_BOTTOM_Y, "BACK", ENABLED);
	okBtn.onPress = home_onPress;
	DM_Add_Element(okBtn);

	//Save button
	struct DisplayElement saveBtn = DM_New_Button(BTN_MIDDLE_X, BTN_BOTTOM_Y, "SAVE", ENABLED);
	//saveBtn.onPress = save_onPress;
	DM_Add_Element(saveBtn);


	//Time elements
	DM_Add_Element(DM_New_Text(10, 45, textColour, 1, "Start Time:"));
	hrsBox = DM_New_TextBox(10, 55, 2, "HH", hrs);
	DM_Add_Element(hrsBox);
	minsBox = DM_New_TextBox(60, 55, 2, "MM", mins);
	DM_Add_Element(minsBox);
	secsBox = DM_New_TextBox(110, 55, 2, "SS", secs);
	DM_Add_Element(secsBox);
	//Duration
	DM_Add_Element(DM_New_Text(180, 45, textColour, 1, "Duration:"));
	durationBox = DM_New_TextBox(180, 55, 4, "Secs", duration);
	DM_Add_Element(durationBox);

	//Days to enable
	DM_Add_Element(DM_New_Text(10, 100, textColour, 1, "Days to Run:"));
	int xPos = 10;
	int yPos = 120;
	const int space = 40;
	const int offset = 0;
	DM_Add_Element(DM_New_Text(xPos, yPos, COLOR_BLACK, 1, "SUN"));
	DM_Add_Element(DM_New_TickBox(xPos + offset, yPos + 10, 0));
	xPos += space;
	DM_Add_Element(DM_New_Text(xPos, yPos, COLOR_BLACK, 1, "MON"));
	DM_Add_Element(DM_New_TickBox(xPos + offset, yPos + 10, 0));
	xPos += space;
	DM_Add_Element(DM_New_Text(xPos, yPos, COLOR_BLACK, 1, "TUE"));
	DM_Add_Element(DM_New_TickBox(xPos + offset, yPos + 10, 0));
	xPos += space;
	DM_Add_Element(DM_New_Text(xPos, yPos, COLOR_BLACK, 1, "WED"));
	DM_Add_Element(DM_New_TickBox(xPos + offset, yPos + 10, 0));
	xPos += space;
	DM_Add_Element(DM_New_Text(xPos, yPos, COLOR_BLACK, 1, "THU"));
	DM_Add_Element(DM_New_TickBox(xPos + offset, yPos + 10, 0));
	xPos += space;
	DM_Add_Element(DM_New_Text(xPos, yPos, COLOR_BLACK, 1, "FRI"));
	DM_Add_Element(DM_New_TickBox(xPos + offset, yPos + 10, 0));
	xPos += space;
	DM_Add_Element(DM_New_Text(xPos, yPos, COLOR_BLACK, 1, "SAT"));
	DM_Add_Element(DM_New_TickBox(xPos + offset, yPos + 10, 0));




	while(1);
}


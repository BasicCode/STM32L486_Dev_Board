/*
 * TimrListScreen.c
 *
 * Screen for listing all the available timers. A user can add or select
 * timers to open and edit or delete.
 *
 *  Created on: 27 Sep. 2022
 *      Author: tommy
 */

#include "screens/TimerListScreen.h"
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
#include "DisplayManager/List.h"

#include "colours.h"

//Dummy list for testing
char *dummyList[] = {"Hello World", "Test list item", "Item 2", "Item 3", "Item 4", "Item 5", "Final Item"};
int numItems = 7;

//Private functions
void timerList_onPress(int id);


/**
 * Entry point for the Edit Time screen.
 */
void TimerListTask(void const * arguments) {
	//Some default colours
	const int textColour = COLOR_BLACK;

	//Start with the screen elements
	DM_Add_Element(DM_New_Title_Bar("Timers"));

	//Main list
	struct DisplayElement timersList = DM_New_List(0, 50, WIDTH, HEIGHT - 50, dummyList, numItems);
	timersList.onPress = timerList_onPress;
	DM_Add_Element(timersList);

	while(1);
}

/**
 * Called when a timer item is pressed. Called by the intrinsic List onPress element.
 */
void timerList_onPress(int id) {
	//Open the EDIT timer page
	xTaskNotify(changeScreenTaskHandle, EDIT_TIMER, eSetValueWithOverwrite);
}

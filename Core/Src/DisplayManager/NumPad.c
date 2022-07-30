/*
 * NumPad.c
 * Display Manager Number Pad
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/NumPad.h"
#include "colours.h"

void DM_NumPad(int id);
void DM_NumPad_Button(int x, int y, char c, State state);
void DM_NumPad_onPress(int id, int x, int y);

/**
 * Create a new number pad element.
 *
 * A number pad will always fill the right-hand half of the display.
 * Design your components appropriately.
 */
struct DisplayElement DM_New_NumPad() {
	struct DisplayElement numPad;
	numPad.type = NUMPAD;
	numPad.x1 = WIDTH - (60 * 3);
	numPad.y1 = 40;
	numPad.x2 = WIDTH;
	numPad.y2 = HEIGHT;

	numPad.selected = 0;
	numPad.onPress = NULL;
	numPad.refresh = ALWAYS;
	numPad.draw = DM_NumPad;

	return numPad;
}

/**
 * Draws a number pad to the display;
 */
void DM_NumPad(int id) {
	const int buttonSpace = 60;
	//Draw a few buttons
	State state = ENABLED;
	//Each vertical row
	for(int y = 0; y < 3; y++) {
		//..And Each horizontal row
		for(int x = 0; x < 3; x++) {
			//Figure out which digit this is
			int thisDigit = x + (y * 3) + 1;

			//Is this particular digit selected
			if(elements[id].selected == thisDigit)
				state = SELECTED;
			else
				state = ENABLED;

			//Put the digit with its ASCII to the display
			DM_NumPad_Button(elements[id].x1 + (x * buttonSpace), elements[id].y1 + (y * buttonSpace), thisDigit + 48, state);
		}
	}

	//zero at the very bottom
	if(elements[id].selected == 11)
		state = SELECTED;
	else
		state = ENABLED;
	DM_NumPad_Button(elements[id].x1 + (1 * buttonSpace), elements[id].y1 + (3 * buttonSpace), '0', state);
	//And a backspace button
	if(elements[id].selected == 12)
		state = SELECTED;
	else
		state = ENABLED;
	DM_NumPad_Button(elements[id].x1 + (2 * buttonSpace), elements[id].y1 + (3 * buttonSpace), '<', state);

}

/**
 * Draws an individual number button to the numpad.
 */
void DM_NumPad_Button(int x1, int y1, char c, State state) {
	const int buttonSize = 50;
	int x2 = x1 + buttonSize;
	int y2 = y1 + buttonSize;
	int outlineColour = COLOR_GRAY;
	int fillColour = COLOR_LIGHTGRAY;
	int textColour = COLOR_BLACK;
	if(state == DISABLED) {
		fillColour = COLOR_WHITE;
		outlineColour = COLOR_LIGHTGRAY;
		textColour = COLOR_LIGHTGRAY;
	}
	if(state == SELECTED) {
		fillColour = COLOR_LIGHTBLUE;
	}
	//Draw the background
	fill_rectangle(x1 + 1, y1 + 1, x2 - 1, y2 - 1, fillColour);

	//Draw the outline
	//Top border
	fill_rectangle(x1 + 1, y1, x2, y1 + 1, outlineColour);
	//Left border
	fill_rectangle(x1, y1 + 1, x1 + 1, y2, outlineColour);
	//Bottom border
	fill_rectangle(x1 + 1, y2, x2, y2 + 1, outlineColour);
	//Right border
	fill_rectangle(x2, y1 + 1, x2 + 1, y2, outlineColour);
	//Write the text
	draw_fast_char(x1 + 12, y1 + 16, c, textColour, fillColour);
}

/**
 * Intrinsic onPress function for the number pad. Determines which button was pressed
 * then hands off to the user-defined onPress function.
 */
void DM_NumPad_onPress(int id, int x, int y) {
	const int buttonSize = 50;
	const int buttonMargin = 10;

	//Figure out which number was pressed.
	//The number are arranged in a 3x3 +1 grid.

	//Calculate the X and Y axis location in button-space
	int xIndex = (int) ((x - elements[id].x1) / (buttonSize + buttonMargin));
	int yIndex = (int) ((y - elements[id].y1) / (buttonSize + buttonMargin));

	//Work out the index
	int index = xIndex + (yIndex * 3) + 1;

	//Report the element that's selected
	elements[id].selected = index;

	//Call any user-defind onPress function
	if(elements[id].onPress)
		elements[id].onPress(id);
}

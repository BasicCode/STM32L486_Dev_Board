/*
 * Button.c
 * Display Manager Button element
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */



#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/Button.h"
#include "colours.h"

void DM_Button(int id);
void DM_Button_onPress(int id);

/**
 * Create a new button and add it to the queue
 */
struct DisplayElement DM_New_Button(int x, int y, char *text, State state){
	int width = 120;
	int height = 40;
	//If the string is long, then increase button width
	int len = DM_StrLen(text, 64);
	if(len > 8) {
		width = len * 12;
		//if this button is on the right-hand edge, then move it back a bit
		if(x > BTN_MIDDLE_X)
			x = WIDTH - width - 10;
	}
	struct DisplayElement button;
	button.type = BUTTON;
	button.x1 = x; button.y1 = y; button.x2 = x + width; button.y2 = y + height;
	button.text = text;
	button.state = state;
	button.oldState = state - 1; //Will only refresh when the states DONT match.
	button.draw = DM_Button;
	button.onPress = NULL;
	button.refresh = ALWAYS;

	return button;
}

/**
 * A button looking thing
 */
void DM_Button(int id) {
	int outlineColour = COLOR_GRAY;
	int fillColour = COLOR_LIGHTGRAY;
	int textColour = COLOR_BLACK;
	if(elements[id].state == DISABLED) {
		fillColour = COLOR_WHITE;
		outlineColour = COLOR_LIGHTGRAY;
		textColour = COLOR_LIGHTGRAY;
	}
	if(elements[id].state == SELECTED) {
		fillColour = COLOR_LIGHTBLUE;
	}
	//Draw the background
	fill_rectangle(elements[id].x1 + 1, elements[id].y1 + 1, elements[id].x2 - 1, elements[id].y2 - 1, fillColour);

	//Draw the outline
	//Top border
	fill_rectangle(elements[id].x1 + 1, elements[id].y1, elements[id].x2, elements[id].y1 + 1, outlineColour);
	//Left border
	fill_rectangle(elements[id].x1, elements[id].y1 + 1, elements[id].x1 + 1, elements[id].y2, outlineColour);
	//Bottom border
	fill_rectangle(elements[id].x1 + 1, elements[id].y2, elements[id].x2, elements[id].y2 + 1, outlineColour);
	//Right border
	fill_rectangle(elements[id].x2, elements[id].y1 + 1, elements[id].x2 + 1, elements[id].y2, outlineColour);

	//Calculate the length of the string to center the text
	int strLen = DM_StrLen(elements[id].text, 18);

	//Each char will be 8px wide
	int halfWidth = strLen * 3;
	//Write the text
	draw_fast_string(elements[id].x1 + 50 - halfWidth, elements[id].y1 + 16, textColour, fillColour, elements[id].text);
}

/**
 * Intrinsic button press function. Any global behaviours for all buttons should go here.
 */
void DM_Button_onPress(int id) {
	if(elements[id].onPress)
		elements[id].onPress(id);
}

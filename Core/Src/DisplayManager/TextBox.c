/*
 * TextBox.c
 * Display Manager Text Box element
 *
 *  Created on: Jul 31, 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/TextBox.h"
#include "DisplayManager/NumPad.h"
#include "colours.h"

void DM_TextBox(int id);

/**
 * Creates a Text Box display element for use in the elements array.
 * NOTE: the "Title" property is used for Placeholder Text in this case.
 */
struct DisplayElement DM_New_TextBox(int x, int y, int length, char *placeholderText, char *text) {
	struct DisplayElement textBox = getDefaultElement();
	textBox.type = TEXTBOX;
	textBox.x1 = x; textBox.y1 = y;
	textBox.x2 = x + (length * 18) + 10;
	textBox.y2 = y + 30;

	textBox.selected = 0;
	textBox.size = 2;
	textBox.text = text;
	textBox.title = placeholderText;
	textBox.draw = DM_TextBox;
	textBox.refresh = ALWAYS;
	textBox.canFocus = 1;
	textBox.maxLength = length;

	return textBox;
}

void DM_TextBox(int id) {
	//Select some appropriate colours
	int fillColour = COLOR_WHITE;
	if(elements[id].selected != 0 && focusedElement == id)
		fillColour = COLOR_LIGHTBLUE;

	int textColour = COLOR_BLACK;
	int placeholderTextColour = COLOR_LIGHTGRAY;
	int outlineColour = COLOR_GRAY;

	//Draw a nice box
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

	//Draw the placeholder text or string
	if(elements[id].text[0] == '\0') {
		//If there is no main text, then draw the placeholder
		draw_string(elements[id].x1 + 5, elements[id].y1 + 4, placeholderTextColour, 2, elements[id].title);
	} else {
		draw_string(elements[id].x1 + 5, elements[id].y1 + 4, textColour, 2, elements[id].text);
	}
}

/**
 * Handles highlighting a text box when it is selected
 */
void DM_TextBox_onPress(int id) {
	//Report that this box is selected
	elements[id].selected = 1;

	//Add a number pad to the screen for typing
	DM_Add_Element(DM_New_NumPad());
}



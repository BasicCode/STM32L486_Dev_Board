/*
 * List.c
 * Display Manager List element
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/List.h"
#include "colours.h"


void DM_List(int id);
void DM_List_onPress(int id, int x, int y);

/**
 * Creates a new list element for you
 */
struct DisplayElement DM_New_List(int x1, int y1, int x2, int y2, char **children, int numChildren) {
	struct DisplayElement list = getDefaultElement();
	//Hard coded X, Y and size for the moment
	list.x1 = x1; list.y1 = y1; list.x2 = x2; list.y2 = y2;
	list.type = LIST;
	list.refresh = ONCE;
	list.draw = DM_List;
	list.children = children;
	list.numChildren = numChildren;
	list.onPress = NULL;

	return list;
}

/**
 * Draws the list elements to the screen
 */
void DM_List(int id) {
	const int lineHeight = 35;
	const int textPadding = 4;
	const int selectedColour = PRIMARY_COLOUR;
	const int textColour = COLOR_BLACK;

	//Check if the list is going to overflow and need to scroll
	int maxVisibleElements = (int)((elements[id].y2 - elements[id].y1) / (lineHeight));
	int midX = (int)((elements[id].x2 - elements[id].x1) / 2);
	int hLineColour1 = COLOR_WHITE;
	int hLineColour2 = COLOR_LIGHTGRAY;

	for(int i = 0; i < elements[id].numChildren; i++) {
		//If we have exceeded the display size then break
		if(i >= maxVisibleElements)
			break;

		int thisY = elements[id].y1 + (i * (lineHeight));
		//If this element is selected then draw a box around it
		if(elements[id].state == SELECTED) {
			//If this is the element that is selected then draw a highlight
			if(elements[id].selected == i) {
				fill_gradient(elements[id].x1, thisY, elements[id].x2, thisY + lineHeight, selectedColour, BG_COLOUR,  HORIZONTAL);

			}
			else
				fill_rectangle(elements[id].x1, thisY, elements[id].x2, thisY + lineHeight, BG_COLOUR);

		}

		//Write the string
		draw_string(elements[id].x1 + 10, thisY + textPadding, textColour, 2, elements[id].children[i]);

		//Put a divider line after each item
		fill_gradient(elements[id].x1 + 30, thisY + lineHeight - 1, midX, thisY + lineHeight, hLineColour1, hLineColour2, HORIZONTAL);
		fill_gradient(midX, thisY + lineHeight - 1, elements[id].x2 - 30, thisY + lineHeight, hLineColour2, hLineColour1, HORIZONTAL);


	}
}

/**
 * Called by the touch handler when the list is pressed. Determines which element
 * was pressed.
 */
void DM_List_onPress(int id, int x, int y) {
	const int lineHeight = 35;

	elements[id].state = SELECTED;
	elements[id].refresh = ONCE;
	//Report which item is selected
	int index = (int)((y - elements[id].y1) / lineHeight);

	elements[id].selected = index;

	if(elements[id].onPress)
		elements[id].onPress(id);
}

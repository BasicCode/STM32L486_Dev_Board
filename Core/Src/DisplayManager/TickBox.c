/*
 * TickBox.c
 *
 * A tick or check box type element.
 *
 *  Created on: 27 Sep. 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/TickBox.h"
#include "colours.h"

void DM_TickBox(int id);

/**
 * Create a new tick box element.
 */
struct DisplayElement DM_New_TickBox(int x, int y, int selected) {
	const int width = 30;
	const int height = 30;
	struct DisplayElement box = getDefaultElement();
	box.type = TICKBOX;
	box.x1 = x; box.y1 = y;
	box.x2 = x + width; box.y2 = y + height;
	box.state = ENABLED;
	box.selected = selected;
	box.oldState = -1;
	box.draw = DM_TickBox;
	box.refresh = ONCE;

	return box;
}

/**
 * Draw funciton to draw the tick box to the display
 */
void DM_TickBox(int id) {

	//Draw the background
	//fill_rectangle(elements[id].x1 + 1, elements[id].y1 + 1, elements[id].x2 - 1, elements[id].y2 - 1, SECONDARY_COLOUR);

	//Draw the outline
	empty_rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, PRIMARY_COLOUR, 1);

	//If the box is selected then draw check mark
	int box_fill = 0xF7BE;
	if(elements[id].selected == 1)
		box_fill = CHECKED_COLOUR;
	fill_rectangle(elements[id].x1 + 6, elements[id].y1 + 6, elements[id].x2 - 5, elements[id].y2 - 5, box_fill);
}

/**
 * Intrinsic onPress function to update the state of the check box
 */
void DM_TickBox_onPress(int id) {

	//Change state
	if(elements[id].selected == 0)
		elements[id].selected = 1;
	else
		elements[id].selected = 0;

	elements[id].refresh = ONCE;

	return;
}

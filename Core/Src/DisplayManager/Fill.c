/*
 * FillRectangle.c
 * Display Manager Fill Rectangle element
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/Fill.h"
#include "colours.h"

void DM_Fill_Rectangle(int id);
void DM_Fill_Gradient(int id);

/**
 * Create a rectangle display element and add it to the queue
 */
struct DisplayElement DM_New_Fill_Rectangle(int x1, int y1, int x2, int y2, int colour) {
	struct DisplayElement box = getDefaultElement();
	box.type = FILLRECTANGLE;
	box.x1 = x1; box.y1 = y1; box.x2 = x2; box.y2 = y2;
	box.colour = colour;
	box.draw = DM_Fill_Rectangle;
	box.refresh = ONCE;
	return box;
}

/**
 * A display element that draws a box
 */
void DM_Fill_Rectangle(int id) {
	fill_rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, elements[id].colour);
}


/**
 * Creates a gradient filled rectangle element between two colours.
 */
struct DisplayElement DM_New_Fill_Gradient(int x1, int y1, int x2, int y2, unsigned int startColour, unsigned int endColour, Orientation orientation) {
	struct DisplayElement box = getDefaultElement();
	box.type = FILLGRADIENT;
	box.x1 = x1; box.y1 = y1; box.x2 = x2; box.y2 = y2;
	box.colour = startColour;
	box.bgColour = endColour;
	box.orientation = orientation;
	box.draw = DM_Fill_Gradient;
	box.refresh = ONCE;

	return box;
}

/**
 * Draws a gradient filled rectangle to the display.
 */
void DM_Fill_Gradient(int id) {
	fill_gradient(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, elements[id].colour, elements[id].bgColour, elements[id].orientation);
}

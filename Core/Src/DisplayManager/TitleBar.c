/*
 * TitleBar.c
 * Display Manager Title Bar element
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/TitleBar.h"
#include "colours.h"

void DM_Title_Bar(int id);

/**
 * Add a title bar to the top of the screen
 */
struct DisplayElement DM_New_Title_Bar(char *title) {
	int height = 40;
	struct DisplayElement titleBar = getDefaultElement();
	titleBar.type = TITLEBAR;
	titleBar.x1 = 0; titleBar.y1 = 0; titleBar.x2 = WIDTH; titleBar.y2 = height;
	titleBar.title = title;
	titleBar.draw = DM_Title_Bar;
	titleBar.refresh = ONCE;

	return titleBar;
}

void DM_Title_Bar(int id) {
	int backgroundColour = COLOR_LIGHTBLUE;
	int foregroundColour = COLOR_WHITE;
	int halfWidth = (int)((elements[id].x2 - elements[id].x1) / 2);

	//Draw the background
	//fill_rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, backgroundColour);
	fill_gradient(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, COLOR_NAVY, backgroundColour, HORIZONTAL);
	//And a line
	//fill_rectangle(elements[id].x1, elements[id].y2, elements[id].x2, elements[id].y2 + 1, COLOR_GRAY);

	//Center the text
	int titleLen = DM_StrLen(elements[id].title, 32);
	draw_string(halfWidth - (titleLen * 9), elements[id].y1 + 10, foregroundColour, 2, elements[id].title);
}

/*
 * TitleBar.c
 * Display Manager Title Bar element
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/TitleBar.h"
#include "DevBoard.h"
#include "colours.h"
#include "bitmaps/home_small.h"

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
	int backgroundColour = COLOR_WHITE;
	int foregroundColour = TITLE_COLOUR;
	int textColour = COLOR_BLACK;
	int halfWidth = (int)((elements[id].x2 - elements[id].x1) / 2);

	//Draw the background
	fill_gradient(elements[id].x1, elements[id].y1, halfWidth, elements[id].y2, backgroundColour, foregroundColour, HORIZONTAL);
	fill_gradient(halfWidth, elements[id].y1, elements[id].x2, elements[id].y2, foregroundColour, backgroundColour, HORIZONTAL);

	//Center the text
	int titleLen = DM_StrLen(elements[id].title, 32);
	draw_string(halfWidth - (titleLen * 9), elements[id].y1 + 10, textColour, 2, elements[id].title);

	//The BACK button
	//draw_char(10, 10, '<', foregroundColour, 2);
	draw_bitmap_with_alpha(4, 4, COLOR_BLACK, 1, home_small);
}

/**
 * An intrinsic onPress function for the title bar.
 * Probably handles the BACK button, or maybe a menu later.
 */
void DM_Title_Bar_onPress(int id, int x, int y) {
	int size = 40; // Size of hit box

	//If the top-left corner is pressed then go to HOME
	if(x < 40 && y < 40)
		home_onPress(id);
}

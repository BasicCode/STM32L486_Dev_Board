/*
 * Text.c
 * Display Manager Text and Fast Text elements.
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/Text.h"
#include "colours.h"

void DM_Text(int id);
void DM_Fast_Text(int id);

/**
 * Various string drawing elements. Just puts the string on teh screen with nothing extra.
 */
struct DisplayElement DM_New_Text(int x, int y, int colour, int size, char* text){
	struct DisplayElement string;
	string.type = TEXT;
	string.x1 = x; string.y1 = y;
	//Have to calculate the length to make a prpoer hit box
	int strLen = DM_StrLen(text, 128);
	string.x2 = x + ((strLen + 1) * 8); string.y2 = y + 14;
	string.size = size;
	string.colour = colour;
	string.text = text;
	string.draw = DM_Text;
	string.refresh = ONCE;

	return string;
}
void DM_Text(int id){
	draw_string(elements[id].x1, elements[id].y1, elements[id].colour, elements[id].size, elements[id].text);
}
struct DisplayElement DM_New_Fast_Text(int x, int y, int colour, int backgroundColour, char* text){
	struct DisplayElement string;
	string.type = FASTTEXT;
	string.x1 = x; string.y1 = y;
	//Have to calculate the length to make a prpoer hit box
	int strLen = DM_StrLen(text, 128);
	string.x2 = x + ((strLen + 1) * 8); string.y2 = y + 14;
	string.colour = colour;
	string.bgColour = backgroundColour;
	string.text = text;
	string.draw = DM_Fast_Text;
	string.refresh = ONCE;

	return string;
}
void DM_Fast_Text(int id){
	draw_fast_string(elements[id].x1, elements[id].y1, elements[id].colour, elements[id].bgColour, elements[id].text);
}

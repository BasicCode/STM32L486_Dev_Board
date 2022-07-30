/*
 * MessageBox.c
 * Display Manager Message Box element
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/MessageBox.h"
#include "colours.h"

void DM_MessageBox(int id);

/**
 * Draws a message box in the middle of the screen with the desired buttons
 * and text.
 */
struct DisplayElement DM_New_MessageBox(char *title, char *text) {
	const int width = 300;
	const int height = 166;
	struct DisplayElement messageBox;
	messageBox.type = MESSAGEBOX;
	messageBox.x1 = (int) ((WIDTH - width) / 2);
	messageBox.y1 = (int) ((HEIGHT - height) / 2);
	messageBox.x2 = (int) ((WIDTH + width) / 2);
	messageBox.y2 = (int) ((HEIGHT + height) / 2);
	messageBox.title = title;
	messageBox.text = text;
	messageBox.draw = DM_MessageBox;
	messageBox.refresh = ONCE;

	return messageBox;
}
/**
 * Draws a message box in the center of the screen.
 */
void DM_MessageBox(int id) {
	const int centerX = (int) (WIDTH / 2);
	const int titleHeight = 35;
	const int lineLength = 30;
	const int maxLines = 8;
	const int lineHeight = 12;
	const int marginTop = 5;
	const int maxWrapLength = 10;

	int outlineColour = COLOR_GRAY;
	int fillColour = COLOR_LIGHTGRAY;
	int textColour = COLOR_BLACK;
	int titleColour = COLOR_LIGHTBLUE;

	//Blank the area with the box
	fill_rectangle(elements[id].x1, elements[id].y1+titleHeight, elements[id].x2, elements[id].y2, fillColour);
	fill_rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y1 + titleHeight, titleColour);
	//Blank the title area
	//Draw the outline
	fill_rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y1 + 1, outlineColour);
	fill_rectangle(elements[id].x1, elements[id].y1, elements[id].x1 + 1, elements[id].y2, outlineColour);
	fill_rectangle(elements[id].x1, elements[id].y2, elements[id].x2, elements[id].y2 + 1, outlineColour);
	fill_rectangle(elements[id].x2, elements[id].y1, elements[id].x2 + 1, elements[id].y2, outlineColour);


	//Calculate the length of the Title string to center the text
	int titleLen = DM_StrLen(elements[id].title, 16) +1;

	draw_string(centerX - (titleLen * 8), elements[id].y1 + marginTop, textColour, 2, elements[id].title);

	//Calculate the length of the text stringstrLen = 0;
	int strLen = DM_StrLen(elements[id].text, lineLength * maxLines);

	//Calculate text wrap
	// A text box can display 8 lines of text *30 chars
	char line[lineLength];
	int linePos = 0;
	int lineCount = 0;
	//Itterate through the string, adding characters to the line buffer
	for(int i = 0; i <= strLen; i++) {
		//Add element to the line
		line[linePos] = elements[id].text[i];

		//If this is the last element then draw the remaining text and break
		if(elements[id].text[i] == '\0') {
			draw_fast_string(elements[id].x1 + 10, elements[id].y1 + titleHeight + marginTop + (lineHeight * lineCount), textColour, fillColour, line);
			break;
		}

		//Increment the line position
		linePos++;

		//Detect new line character
		if(elements[id].text[i] == '\n') {
			//End the line
			line[linePos - 1] = '\0';
			//write the line and go to the next line
			draw_fast_string(elements[id].x1 + 10, elements[id].y1 + titleHeight + marginTop + (lineHeight * lineCount), textColour, fillColour, line);
			linePos = 0;
			lineCount++;
		}

		//If the next space is more than the distance to the end of the line then to to next line
		if(linePos > lineLength - maxWrapLength) {
			//If we're close to the end of the line
			if(elements[id].text[i] == ' ') {
				//Check a few characters ahead for the next space
				for(int j = 1; j < maxWrapLength; j++) {
					if(elements[id].text[i + j] == ' ') {
						//Check if this is further than the end of the line
						if((linePos + j) > lineLength) {
							//End the line
							line[linePos] = '\0';
							//write the line and go to the next line
							draw_fast_string(elements[id].x1 + 10, elements[id].y1 + titleHeight + marginTop + (lineHeight * lineCount), textColour, fillColour, line);
							linePos = 0;
							lineCount++;
							break;
						}
					}
				}
			}
		}





		//If the line is a full length then draw it to the screen
		if(linePos == lineLength) {
			draw_string(elements[id].x1 + 10, elements[id].y1 + titleHeight + marginTop + (lineHeight * lineCount), textColour, 1, line);
			//reset the line position
			linePos = 0;
			lineCount++;
		}

		//If the have too many lines then truncate the string and break
		if(lineCount == maxLines) {
			line[linePos - 3] = '.';
			line[linePos - 2] = '.';
			line[linePos - 1] = '.';
			line[linePos] = '\0';
			break;
		}
	}
}

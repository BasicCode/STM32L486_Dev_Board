/*
 * Text.h
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_TEXT_H_
#define INC_DISPLAYMANAGER_TEXT_H_

#include "DisplayManager/DisplayManager.h"

struct DisplayElement DM_New_Text(int x, int y, int size, int colour, char* text);
struct DisplayElement DM_New_Fast_Text(int x, int y, int colour, int backgroundColour, char* text);

#endif /* INC_DISPLAYMANAGER_TEXT_H_ */

/*
 * FillRectangle.h
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_FILL_H_
#define INC_DISPLAYMANAGER_FILL_H_

#include "DisplayManager/DisplayManager.h"
#include "DevBoard.h"

struct DisplayElement DM_New_Fill_Rectangle(int x1, int y1, int x2, int y2, int colour);
struct DisplayElement DM_New_Fill_Gradient(int x1, int y1, int x2, int y2, unsigned int startColour, unsigned int endColour, Orientation orientation);

#endif /* INC_DISPLAYMANAGER_FILL_H_ */

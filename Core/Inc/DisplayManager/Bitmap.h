/*
 * Bitmap.h
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_BITMAP_H_
#define INC_DISPLAYMANAGER_BITMAP_H_

#include "DisplayManager/DisplayManager.h"

struct DisplayElement DM_New_Bitmap(int x, int y, int scale, const unsigned int *bitmap);
struct DisplayElement DM_New_Bitmap_With_Alpha(int x, int y, int alphaColour, int scale, const unsigned int *src);

#endif /* INC_DISPLAYMANAGER_BITMAP_H_ */

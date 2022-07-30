/*
 * Animation.h
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_ANIMATION_H_
#define INC_DISPLAYMANAGER_ANIMATION_H_


#include "DisplayManager/DisplayManager.h"


struct DisplayElement DM_New_Animation(int x1, int y1, int scale, unsigned int **bitmaps, int numFrames);


#endif /* INC_DISPLAYMANAGER_ANIMATION_H_ */

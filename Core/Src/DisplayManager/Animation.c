/*
 * Display Manager Animation element
 * Animation.c
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/Animation.h"
#include "colours.h"

void DM_Animation(int id);



/**
 * Creates a new animation element.
 * bitmaps - an array of pointers to bitmaps for each frame
 * selected - the current frame being displayed
 * numChildren - the number of frames in the array
 */
struct DisplayElement DM_New_Animation(int x1, int y1, int scale, unsigned int **bitmaps, int numFrames) {
	struct DisplayElement animation = getDefaultElement();
	animation.type = ANIMATION;
	animation.x1 = x1; animation.y1 = y1;
	//Get the animation dimensions
	animation.x2 = x1 + (bitmaps[0][0] * scale);
	animation.y2 = y1 + (bitmaps[0][1] * scale);
	animation.size = scale;
	animation.bitmaps = bitmaps;
	animation.selected = 0;
	animation.animationTicks = 0;
	animation.ticksPerFrame = TICKS_PER_FRAME;
	animation.refresh = ALWAYS;
	animation.draw = DM_Animation;
	animation.numChildren = numFrames;

	return animation;
}

/**
 * Draws an animation frame to the display.
 */
void DM_Animation(int id) {
	draw_bitmap(elements[id].x1, elements[id].y1, elements[id].size, elements[id].bitmaps[elements[id].selected]);
}

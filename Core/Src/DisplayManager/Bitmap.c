/*
 * Bitmap.c
 * Display Element Bitmap and Bitmap With Alpha elements.
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */


#include "DisplayManager/DisplayManager.h"
#include "DisplayManager/Bitmap.h"
#include "colours.h"

void DM_Bitmap(int id);
void DM_Bitmap_With_Alpha(int id);


/**
 * Create a bitmap element and add it to the queue.
 */
struct DisplayElement DM_New_Bitmap(int x, int y, int scale, const unsigned int *src){
	  struct DisplayElement bitmap = getDefaultElement();
	  bitmap.type = BITMAP;
	  bitmap.x1 = x; bitmap.y1 = y;
	  //Calculate the bitmap size for proper collision detection
	  bitmap.x2 = x + src[0] * scale; bitmap.y2 = y + src[1] * scale;
	  bitmap.size = scale;
	  bitmap.bitmap = src;
	  bitmap.draw = DM_Bitmap;
	  bitmap.refresh = ONCE;

	  return bitmap;
}

/**
 * Draws a bitmap to the display
 */
void DM_Bitmap(int id) {
	draw_bitmap(elements[id].x1, elements[id].y1, elements[id].size, elements[id].bitmap);
}

/**
 * Create a bitmap element with a transparency colour
 */
struct DisplayElement DM_New_Bitmap_With_Alpha(int x, int y, int alphaColour, int scale, const unsigned int *src) {
	  struct DisplayElement bitmap;
	  bitmap.type = BITMAP;
	  bitmap.x1 = x; bitmap.y1 = y;
	  //Calculate the bitmap size for proper collision detection
	  bitmap.x2 = x + src[0] * scale; bitmap.y2 = y + src[1] * scale;
	  bitmap.size = scale;
	  bitmap.bgColour = alphaColour;
	  bitmap.bitmap = src;
	  bitmap.draw = DM_Bitmap_With_Alpha;
	  bitmap.refresh = ONCE;

	  return bitmap;
}

/**
 * Draw a bitmap with a transparency colour
 */
void DM_Bitmap_With_Alpha(int id) {
	draw_bitmap_with_alpha(elements[id].x1, elements[id].y1, elements[id].bgColour, elements[id].size, elements[id].bitmap);
}

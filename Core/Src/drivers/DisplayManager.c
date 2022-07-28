/**
 * A higher level system to manage display elements, and draw order to the LCD.
 *
 * This uses a collection of "elements", which are stored in an array and drawn
 * to the display when desired.
 */

//Your LCD driver
#include "drivers/DisplayManager.h"
#include "drivers/ILI9488.h"

//Private functions definitions
void DM_Fill_Rectangle(int id);
void DM_Fill_Gradient(int id);
void DM_Text(int id);
void DM_Fast_Text(int id);
void DM_Bitmap(int id);
void DM_Bitmap_With_Alpha(int id);
void DM_Button(int id);
void DM_Button_onPress(int id);
void DM_MessageBox(int id);
void DM_Menu(int id);
void DM_Title_Bar(int id);
void DM_List(int id);
void DM_List_onPress(int id, int x, int y);
void DM_Animation(int id);
void DM_NumPad(int id);
void DM_NumPad_Button(int x, int y, char c, State state);
void DM_NumPad_onPress(int id, int x, int y);

//Some hard coded memory for the display elements - this is just an array of DisplayElements
struct DisplayElement elements[MAX_ELEMENTS];
//The global element counter
int numElements = 0;
//A global background colour
uint16_t BGColour = 0xFFFF;
//const unsigned int *BGImage = background;

//The currently touched item, to keep track of drag gestures
int previously_touched_element = 0;

//Number of calls to Draw() before advancing the frame of an animation
const int ticksPerFrame = 2;

/**
 * Initialises the LCD device, and allocates any memory required.
 */
void DM_Init() {
	lcd_init_parallel();
	DM_Clear();
}

/**
 * Draw the whole queue.
 */
void DM_Draw() {
	//Draw each element int he array of display elements
	for(int i = 0; i < numElements; i++) {
		//Only draw elements flagged for update
		if(elements[i].refresh == ONCE) {
			//Draw this element once then clear the refresh flag
			elements[i].draw(i);
			elements[i].refresh = NEVER;
		}
		if(elements[i].refresh == ALWAYS) {
			//For elements that ALWAYS draw, check if the state has changed.
			//Only draw if the state has changed.
			if(elements[i].oldState != elements[i].state) {
				elements[i].draw(i);
				//Update the oldState
				elements[i].oldState = elements[i].state;
			}

			//Advance the ticks of an animation
			//Once the tick counter is reached, the animation advances to the next frame
			if(elements[i].type == ANIMATION) {
				elements[i].animationTicks++;
				//If this element is due to advance to the next frame
				if(elements[i].animationTicks >= elements[i].ticksPerFrame) {
					//Draw the frame
					elements[i].draw(i);

					//Advance the frame and reset the counters
					elements[i].animationTicks = 0;
					elements[i].selected++;
					if(elements[i].selected >= elements[i].numChildren)
						elements[i].selected = 0;
				}
			}
		}

	}
}

/**
 * Clears the array of elements to draw, and clears the screen.
 */
void DM_Clear() {
	//Add a blank screen element to the start of the display
	elements[0] = DM_New_Fill_Rectangle(0, 0, WIDTH, HEIGHT, BGColour);

	//Use a background image instead
	//elements[0] = DM_New_Bitmap(0, 0, 2, BGImage);

	//Reset the array counter
	numElements = 1;
}

/**
 * Clears the display then re-draws EVERY element (except those set to HIDE).
 */
void DM_Full_Refresh() {
	//Tell anything with a state set to NEVER that it needs to refresh
	for(int i = 0; i < numElements; i++) {
		if(elements[i].refresh == NEVER)
			elements[i].refresh = ONCE;
	}
}

/**
 * Register an element for refresh on next cycle.
 */
void DM_Refresh_Element(int id) {
	elements[id].refresh = ONCE;
}

/**
 * Adds an element to the queue to be drawn later.
 */
int DM_Add_Element(struct DisplayElement newElement) {
	elements[numElements++] = newElement;

	return numElements - 1;
}

/**
 * Replaces a specific element in the queue with a new one
 */
void DM_Replace_Element(int id, struct DisplayElement newElement) {
	elements[id] = newElement;
}

/**
 * Calculate the length of a string up to a maximum length.
 * Returns the maximum length if length is exceeded.
 */
int DM_StrLen(char *str, int length) {
	int strLen = 0;
	for(int i = 0; i < length; i++) {
		if(str[i] == '\0') {
			strLen = i;
			break;
		}
	}

	return strLen;
}

/**
 * Change the state of the selected element.
 */
void DM_Set_State(int id, State state) {
	elements[id].state = state;
}

/**
 * Change the "refresh" state of the element, to show or hide it.
 */
void DM_Set_Refresh(int id, Refresh refresh) {
	elements[id].refresh = refresh;
}

/**
 * Contrary to the name, this does not actually remove the element,
 * it just tells the element not to refresh anymore, and then
 * draws a box over it.
 * The element can still be referenced.
 */
void DM_Remove_Element(int id) {
	elements[id].refresh = NEVER;
	DM_Add_Element(DM_New_Fill_Rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, BGColour));
}

/**
 * Returns a reference to the requested element.
 */
struct DisplayElement DM_Get_Element(int id) {
	return elements[id];
}

/**
 * Determines which element, if any, is at the specified location and returns the ID.
 * If multiple elements exist then the top most element is returned.
 *
 * Remember that most of the time element 0 is the background.
 *
 * Returns -1 if nothing is detected.
 */
int DM_Parse_Press(int x, int y) {
	//Check each element to determine if the x,y coordinates are within it's bounds
	for(int i = numElements; i >= 0; i--) {
		if(x > elements[i].x1 && y > elements[i].y1 && x < elements[i].x2 && y < elements[i].y2) {
			return i;
		}
	}

	return -1;
}

/**
 * Determines the touched element, and calls that element's onPress() functions (if any).
 *
 * Many elements have an DM_element_onPress() function which handles display changes like
 * context highlighting etc., as well as an externally defined onPress() function to handle
 * specific tasks.
 *
 * Returns the ID of the element touched, or -1 if no element was touched.
 */
int DM_Do_Press(struct Touch touch) {
	//Find the touched element, if any
	int id = DM_Parse_Press(touch.X, touch.Y);

	//If no element was found then return right away
	if(id < 0)
		return id;

	//Call any onPress function for this element on RELEASE.
	//Check that the element we released on was the one we pressed originally
	if(touch.state == 1 && id == previously_touched_element) {
		//Revert the element's state
		elements[previously_touched_element].state = ENABLED;

		//Call the relevant onPress function
		switch(elements[id].type) {
		case BUTTON:
			DM_Button_onPress(id);
			break;
		case LIST:
			DM_List_onPress(id, touch.X, touch.Y);
			break;
		case NUMPAD:
			DM_NumPad_onPress(id, touch.X, touch.Y);
			break;
		}
	}

	//A press has been recorded, or moved, but has not been released yet
	if(touch.state == 0 && elements[id].state != DISABLED) {
		//Update the "current touched element"
		previously_touched_element = id;
		//Set the state of the helement for context highlighting
		elements[id].state = SELECTED;
	}

	//Finally, if the touch was released, but it is no longer on the element, then revert the element state
	if(touch.state == 1 && id != previously_touched_element){
		//Un-select the previously touched element
		elements[previously_touched_element].state = ENABLED;
	}

	return id;
}


/**
 * When creating a new element, at a minimum, the 'refresh', 'draw', and 'type' elements must be instantiated.
 * X1, Y1, X2, and Y2 should also be set appropriately for collision and touch detection to work properly.
 *
 * refresh 	- See 'RefreshFrequency'. Determines if this elements is drawn on every screen refresh or not.
 * draw		- A pointer to a funciton which does the actual drawing.
 * type		- The type of element (see Type definition).
 * onPress	- A pointer to a function to call if the element is pressed
 */

/**
 * Create a rectangle display element and add it to the queue
 */
struct DisplayElement DM_New_Fill_Rectangle(int x1, int y1, int x2, int y2, int colour) {
	struct DisplayElement box;
	box.type = FILLRECTANGLE;
	box.x1 = x1; box.y1 = y1; box.x2 = x2; box.y2 = y2;
	box.colour = colour;
	box.draw = DM_Fill_Rectangle;
	box.refresh = ONCE;
	return box;
}

/**
 * A display element that draws a box
 */
void DM_Fill_Rectangle(int id) {
	fill_rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, elements[id].colour);
}

/**
 * Creates a gradient filled rectangle element between two colours.
 */
struct DisplayElement DM_New_Fill_Gradient(int x1, int y1, int x2, int y2, unsigned int startColour, unsigned int endColour, Orientation orientation) {
	struct DisplayElement box;
	box.type = FILLGRADIENT;
	box.x1 = x1; box.y1 = y1; box.x2 = x2; box.y2 = y2;
	box.colour = startColour;
	box.bgColour = endColour;
	box.orientation = orientation;
	box.draw = DM_Fill_Gradient;
	box.refresh = ONCE;

	return box;
}

/**
 * Draws a gradient filled rectangle to the display.
 */
void DM_Fill_Gradient(int id) {
	fill_gradient(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, elements[id].colour, elements[id].bgColour, elements[id].orientation);
}

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

/**
 * Create a bitmap element and add it to the queue.
 */
struct DisplayElement DM_New_Bitmap(int x, int y, int scale, const unsigned int *src){
	  struct DisplayElement bitmap;
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

/**
 * Create a new button and add it to the queue
 */
struct DisplayElement DM_New_Button(int x, int y, char *text, State state){
	int width = 120;
	int height = 40;
	//If the string is long, then increase button width
	int len = DM_StrLen(text, 64);
	if(len > 8) {
		width = len * 12;
		//if this button is on the right-hand edge, then move it back a bit
		if(x > BTN_MIDDLE_X)
			x = WIDTH - width - 10;
	}
	struct DisplayElement button;
	button.type = BUTTON;
	button.x1 = x; button.y1 = y; button.x2 = x + width; button.y2 = y + height;
	button.text = text;
	button.state = state;
	button.oldState = state - 1; //Will only refresh when the states DONT match.
	button.draw = DM_Button;
	button.onPress = NULL;
	button.refresh = ALWAYS;

	return button;
}

/**
 * A button looking thing
 */
void DM_Button(int id) {
	int outlineColour = COLOR_GRAY;
	int fillColour = COLOR_LIGHTGRAY;
	int textColour = COLOR_BLACK;
	if(elements[id].state == DISABLED) {
		fillColour = COLOR_WHITE;
		outlineColour = COLOR_LIGHTGRAY;
		textColour = COLOR_LIGHTGRAY;
	}
	if(elements[id].state == SELECTED) {
		fillColour = COLOR_LIGHTBLUE;
	}
	//Draw the background
	fill_rectangle(elements[id].x1 + 1, elements[id].y1 + 1, elements[id].x2 - 1, elements[id].y2 - 1, fillColour);

	//Draw the outline
	//Top border
	fill_rectangle(elements[id].x1 + 1, elements[id].y1, elements[id].x2, elements[id].y1 + 1, outlineColour);
	//Left border
	fill_rectangle(elements[id].x1, elements[id].y1 + 1, elements[id].x1 + 1, elements[id].y2, outlineColour);
	//Bottom border
	fill_rectangle(elements[id].x1 + 1, elements[id].y2, elements[id].x2, elements[id].y2 + 1, outlineColour);
	//Right border
	fill_rectangle(elements[id].x2, elements[id].y1 + 1, elements[id].x2 + 1, elements[id].y2, outlineColour);

	//Calculate the length of the string to center the text
	int strLen = DM_StrLen(elements[id].text, 18);

	//Each char will be 8px wide
	int halfWidth = strLen * 3;
	//Write the text
	draw_fast_string(elements[id].x1 + 50 - halfWidth, elements[id].y1 + 16, textColour, fillColour, elements[id].text);
}

/**
 * Intrinsic button press function. Any global behaviours for all buttons should go here.
 */
void DM_Button_onPress(int id) {
	if(elements[id].onPress)
		elements[id].onPress(id);
}

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

/**
 * A menu is a full-screen element which displays items from the "text" field.
 * Use \n to separate list entries.
 */
struct DisplayElement DM_New_Menu(char *title, char *text) {
	struct DisplayElement menu;
	menu.type = MENU;
	menu.x1 = 0; menu.y1 = 0; menu.x2 = WIDTH; menu.y2 = HEIGHT;
	menu.title = title;
	menu.text = text;
	menu.draw = DM_Menu;
	menu.refresh = ONCE;

	return menu;
}

/**
 * Draw function for a full screen menu.
 */
void DM_Menu(int id) {

}

/**
 * Add a title bar to the top of the screen
 */
struct DisplayElement DM_New_Title_Bar(char *title) {
	int height = 40;
	struct DisplayElement titleBar;
	titleBar.type = TITLEBAR;
	titleBar.x1 = 0; titleBar.y1 = 0; titleBar.x2 = WIDTH; titleBar.y2 = height;
	titleBar.title = title;
	titleBar.draw = DM_Title_Bar;
	titleBar.refresh = ONCE;

	return titleBar;
}

void DM_Title_Bar(int id) {
	int backgroundColour = COLOR_LIGHTBLUE;
	int foregroundColour = COLOR_BLACK;
	int halfWidth = (int)((elements[id].x2 - elements[id].x1) / 2);

	//Draw the background
	//fill_rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, backgroundColour);
	fill_gradient(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, COLOR_NAVY, backgroundColour, HORIZONTAL);
	//And a line
	fill_rectangle(elements[id].x1, elements[id].y2, elements[id].x2, elements[id].y2 + 1, COLOR_GRAY);

	//Center the text
	int titleLen = DM_StrLen(elements[id].title, 32);
	draw_string(halfWidth - (titleLen * 9), elements[id].y1 + 10, foregroundColour, 2, elements[id].title);
}

/**
 * Creates a new list element for you
 */
struct DisplayElement DM_New_List(int x1, int y1, int x2, int y2, char **children, int numChildren) {
	struct DisplayElement list;
	//Hard coded X, Y and size for the moment
	list.x1 = x1; list.y1 = y1; list.x2 = x2; list.y2 = y2;
	list.type = LIST;
	list.refresh = ONCE;
	list.draw = DM_List;
	list.children = children;
	list.numChildren = numChildren;
	list.onPress = NULL;

	return list;
}

/**
 * Draws the list elements to the screen
 */
void DM_List(int id) {
	const int lineHeight = 30;
	const int linePadding = 4;
	const int textPadding = 4;
	const int selectedColour = COLOR_LIGHTBLUE;
	const int textColour = COLOR_BLACK;

	//Check if the list is going to overflow and need to scroll
	int maxVisibleElements = (int)((elements[id].y2 - elements[id].y1) / (lineHeight + linePadding));

	for(int i = 0; i < elements[id].numChildren; i++) {
		int thisY = elements[id].y1 + (i * (lineHeight + linePadding));
		//If this element is selected then draw a box around it
		if(elements[id].state == SELECTED) {
			//If this is the element that is selected then draw a highlight
			if(elements[id].selected == i)
				fill_rectangle(elements[id].x1, thisY, elements[id].x2, thisY + lineHeight, selectedColour);
			else
				fill_rectangle(elements[id].x1, thisY, elements[id].x2, thisY + lineHeight, BGColour);

		}

		//Write the string
		draw_string(elements[id].x1 + 10, thisY + textPadding, textColour, 2, elements[id].children[i]);

		//Put a divider line after each item
		fill_rectangle(elements[id].x1 + 30, thisY + lineHeight - 1, elements[id].x2 - 30, thisY + lineHeight, COLOR_LIGHTGRAY);
	}
}

/**
 * Called by the touch handler when the list is pressed. Determines which element
 * was pressed.
 */
void DM_List_onPress(int id, int x, int y) {
	const int lineHeight = 30;

	elements[id].state = SELECTED;
	elements[id].refresh = ONCE;
	//Report which item is selected
	int index = (int)((y - elements[id].y1) / lineHeight);

	elements[id].selected = index;

	if(elements[id].onPress)
		elements[id].onPress(id);
}

/**
 * Creates a new animation element.
 * bitmaps - an array of pointers to bitmaps for each frame
 * selected - the current frame being displayed
 * numChildren - the number of frames in the array
 */
struct DisplayElement DM_New_Animation(int x1, int y1, int scale, unsigned int **bitmaps, int numFrames) {
	struct DisplayElement animation;
	animation.type = ANIMATION;
	animation.x1 = x1; animation.y1 = y1;
	//Get the animation dimensions
	animation.x2 = x1 + (bitmaps[0][0] * scale);
	animation.y2 = y1 + (bitmaps[0][1] * scale);
	animation.size = scale;
	animation.bitmaps = bitmaps;
	animation.selected = 0;
	animation.animationTicks = 0;
	animation.ticksPerFrame = ticksPerFrame;
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

/**
 * Create a new number pad element.
 *
 * A number pad will always fill the right-hand half of the display.
 * Design your components appropriately.
 */
struct DisplayElement DM_New_NumPad() {
	struct DisplayElement numPad;
	numPad.type = NUMPAD;
	numPad.x1 = WIDTH - (60 * 3);
	numPad.y1 = 40;
	numPad.x2 = WIDTH;
	numPad.y2 = HEIGHT;

	numPad.selected = 0;
	numPad.onPress = NULL;
	numPad.refresh = ALWAYS;
	numPad.draw = DM_NumPad;

	return numPad;
}

/**
 * Draws a number pad to the display;
 */
void DM_NumPad(int id) {
	const int buttonSpace = 60;
	//Draw a few buttons
	State state = ENABLED;
	//Each vertical row
	for(int y = 0; y < 3; y++) {
		//..And Each horizontal row
		for(int x = 0; x < 3; x++) {
			//Figure out which digit this is
			int thisDigit = x + (y * 3) + 1;

			//Is this particular digit selected
			if(elements[id].selected == thisDigit)
				state = SELECTED;
			else
				state = ENABLED;

			//Put the digit with its ASCII to the display
			DM_NumPad_Button(elements[id].x1 + (x * buttonSpace), elements[id].y1 + (y * buttonSpace), thisDigit + 48, state);
		}
	}

	//zero at the very bottom
	if(elements[id].selected == 11)
		state = SELECTED;
	else
		state = ENABLED;
	DM_NumPad_Button(elements[id].x1 + (1 * buttonSpace), elements[id].y1 + (3 * buttonSpace), '0', state);
	//And a backspace button
	if(elements[id].selected == 12)
		state = SELECTED;
	else
		state = ENABLED;
	DM_NumPad_Button(elements[id].x1 + (2 * buttonSpace), elements[id].y1 + (3 * buttonSpace), '<', state);

}

/**
 * Draws an individual number button to the numpad.
 */
void DM_NumPad_Button(int x1, int y1, char c, State state) {
	const int buttonSize = 50;
	int x2 = x1 + buttonSize;
	int y2 = y1 + buttonSize;
	int outlineColour = COLOR_GRAY;
	int fillColour = COLOR_LIGHTGRAY;
	int textColour = COLOR_BLACK;
	if(state == DISABLED) {
		fillColour = COLOR_WHITE;
		outlineColour = COLOR_LIGHTGRAY;
		textColour = COLOR_LIGHTGRAY;
	}
	if(state == SELECTED) {
		fillColour = COLOR_LIGHTBLUE;
	}
	//Draw the background
	fill_rectangle(x1 + 1, y1 + 1, x2 - 1, y2 - 1, fillColour);

	//Draw the outline
	//Top border
	fill_rectangle(x1 + 1, y1, x2, y1 + 1, outlineColour);
	//Left border
	fill_rectangle(x1, y1 + 1, x1 + 1, y2, outlineColour);
	//Bottom border
	fill_rectangle(x1 + 1, y2, x2, y2 + 1, outlineColour);
	//Right border
	fill_rectangle(x2, y1 + 1, x2 + 1, y2, outlineColour);
	//Write the text
	draw_fast_char(x1 + 12, y1 + 16, c, textColour, fillColour);
}

/**
 * Intrinsic onPress function for the number pad. Determines which button was pressed
 * then hands off to the user-defined onPress function.
 */
void DM_NumPad_onPress(int id, int x, int y) {
	const int buttonSize = 50;
	const int buttonMargin = 10;

	//Figure out which number was pressed.
	//The number are arranged in a 3x3 +1 grid.

	//Calculate the X and Y axis location in button-space
	int xIndex = (int) ((x - elements[id].x1) / (buttonSize + buttonMargin));
	int yIndex = (int) ((y - elements[id].y1) / (buttonSize + buttonMargin));

	//Work out the index
	int index = xIndex + (yIndex * 3) + 1;

	//Report the element that's selected
	elements[id].selected = index;

	//Call any user-defind onPress function
	if(elements[id].onPress)
		elements[id].onPress(id);
}


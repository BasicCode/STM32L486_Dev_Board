/**
 * A higher level system to manage display elements, and draw order to the LCD.
 *
 * This uses a collection of "elements", which are stored in an array and drawn
 * to the display when desired.
 */

#include "DisplayManager/DisplayManager.h"
//Elements used here
#include "DisplayManager/Fill.h"
#include "DisplayManager/NumPad.h"
#include "DisplayManager/Button.h"
#include "DisplayManager/List.h"
#include "DisplayManager/TickBox.h"
#include "DisplayManager/TitleBar.h"


//Your LCD driver
#include "drivers/ILI9488.h"

//Private functions definitions
void DM_Fill_Rectangle(int id);
void DM_Text(int id);
void DM_Fast_Text(int id);
void DM_Menu(int id);

//Some hard coded memory for the display elements - this is just an array of DisplayElements
struct DisplayElement elements[MAX_ELEMENTS];
//The global element counter
int numElements = 0;
//The current focused element (there can be only one)
int focusedElement = 0;
int oldFocusedElement = 0;

//The currently touched item, to keep track of drag gestures
int previously_touched_element = 0;

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
	elements[0] = DM_New_Fill_Rectangle(0, 0, WIDTH, HEIGHT, BG_COLOUR);

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
	DM_Add_Element(DM_New_Fill_Rectangle(elements[id].x1, elements[id].y1, elements[id].x2, elements[id].y2, BG_COLOUR));
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
	if(touch.state == 0x01 && id == previously_touched_element) {
		//Revert the element's state
		elements[previously_touched_element].state = ENABLED;

		//Call the relevant intrinsic onPress function
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
		case TEXTBOX:
			DM_TextBox_onPress(id);
			break;
		case TICKBOX:
			DM_TickBox_onPress(id);
			break;
		case TITLEBAR:
			DM_Title_Bar_onPress(id, touch.X, touch.Y);
			break;
		}

		//The controller seems to put extra RELEASE events, so invalidate the previously_touched_element
		previously_touched_element = -1;
	}

	//A press has been recorded, or moved, but has not been released yet
	if(touch.state == 0x00 && elements[id].state != DISABLED) {
		//Update the "current touched element"
		previously_touched_element = id;
		//Set the state of the helement for context highlighting
		elements[id].state = SELECTED;


		//If an element can take focus, then take it
		if(elements[id].canFocus) {
			oldFocusedElement = focusedElement;
			focusedElement = id;
			//Tell the previously focused element to update
			if(elements[oldFocusedElement].canFocus)
				elements[oldFocusedElement].refresh = ONCE;
		}
	}

	//Finally, if the touch was released, but it is no longer on the element, then revert the element state
	if(touch.state == 0x01 && id != previously_touched_element){
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
 * Returns an empty element with everything set to either zero, or NULL.
 * Use this to build a new element safely without random memory.
 */
struct DisplayElement getDefaultElement() {
	struct DisplayElement newElement;
	newElement.type;
	newElement.x1 = 0;
	newElement.y1 = 0;
	newElement.x2 = 0;
	newElement.y2 = 0;
	newElement.size = 0;
	newElement.selected = 0;
	newElement.colour = 0;
	newElement.bgColour = 0;
	newElement.orientation = 0;
	newElement.bitmap = NULL;
	newElement.bitmaps = NULL;
	newElement.title = NULL;
	newElement.text = NULL;
	newElement.state = 0;
	newElement.oldState = -1;
	newElement.canFocus = 0;
	newElement.maxLength = 0;
	newElement.refresh = 0;
	newElement.draw = NULL;
	newElement.onPress = NULL;
	newElement.onDrag = NULL;
	newElement.children = NULL;
	newElement.numChildren = 0;
	newElement.animationTicks = 0;
	newElement.ticksPerFrame = TICKS_PER_FRAME;

	return newElement;
}


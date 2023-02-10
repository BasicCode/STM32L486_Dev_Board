/*
 * DisplayManager.h
 *
 *  Created on: Jun 3, 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_H_
#define INC_DISPLAYMANAGER_H_

#include "stm32l4xx_hal.h"
#include "DevBoard.h"
#include "drivers/FT5446.h"

//Possible states (usually for a button)
typedef enum  {
	ENABLED,
	DISABLED,
	SELECTED
} State;
//The different types of elements. Used to apply unique behaviouirs.
typedef enum {
	BUTTON,
	FILLRECTANGLE,
	TEXT,
	FASTTEXT,
	MESSAGEBOX,
	BITMAP,
	MENU,
	TITLEBAR,
	FILLGRADIENT,
	LIST,
	ANIMATION,
	NUMPAD,
	KEYBOARD,
	TEXTBOX,
	TICKBOX
} Type;
//Refresh frequency of the element
typedef enum {
	ALWAYS,		//Refresh the element on every refresh
	ONCE,		//Refresh the element once only, then set to NEVER state
	NEVER,		//Don't refresh unless full screen refresh
	HIDE		//Don't draw on first run, OR on full screen refresh (just keeps the elemnt in memory)
} Refresh;

//Each element MUST provide a "draw" function
typedef int (*Draw)(int id);
//Elements MAY provide an onPress function
typedef	int (*onPress)(int id);
//Elements MAY provide an onDrag function
typedef int (*onDrag)(int id);
//The structure of a display element.
struct DisplayElement {
	int					type;
	uint16_t			x1; 			//X,Y positions
	uint16_t			y1;
	uint16_t			x2;
	uint16_t			y2;
	int					size; 			//Scale factor for fonts and bitmaps
	int					selected;		//The currently selected item
	int					colour; 		//Foreground colour
	int					bgColour;		//Background colour
	Orientation			orientation;	//Orientation for things like gradient fill
	const unsigned int *bitmap; 		//Pointer to the desired bitmap
	const unsigned int **bitmaps;		//Pointer to a pointer to multiple bitmaps for an animation
	char				*title;			//Pointer to the title string
	char				*text;			//Pointer to the body text string
	State				state;			//The state of the element (see State definition above).
	State				oldState;		//A way to determine if the element needs refreshing. Shouldn't be needed by user.
	int					canFocus;		//This element can take "focus" on the screen, like a text box.
	int					maxLength;		//Maximum length or size of this element (like a string length or something)
	Refresh				refresh;		//How often to refresh the element (see RefreshFrequency)
	Draw				draw;			//Pointer to the function which draws the element
	onPress				onPress;		//Pointer for the funciton to call if a tap or press is registered
	onDrag				onDrag;			//Pointer to a function celled if the element is dragged or scrolled
	char				**children;		//Pointer to an array of child elements
	int					numChildren;	//Number of child elements in the array
	int					animationTicks; //Counter to track ticks for an animation to advance frames
	int					ticksPerFrame;	//Ticks per frame of the animation - the speed of the animation
};

//Common element display locations
typedef enum {
	BTN_LEFT_X = 10,
	BTN_MIDDLE_X = 190,
	BTN_RIGHT_X = 370,
	BTN_BOTTOM_Y = 272,
} ButtonPositions;

//Total number of elements on the screen at any time
#define MAX_ELEMENTS	32

//The array of screen elements - Declared in the parent C file
extern struct DisplayElement elements[MAX_ELEMENTS];
//The current focused element
extern int focusedElement;
extern int oldFocusedElement;

//Default ticks per frame for animations
#define TICKS_PER_FRAME 2

//A default background colour
#define BG_COLOUR 0xFFFF

//Function Prototypes
void DM_Init();
void DM_Draw();
void DM_Clear();
void DM_Full_Refresh();
void DM_Refresh_Element(int id);
int DM_Add_Element();
void DM_Replace_Element(int id, struct DisplayElement newElement);
int DM_StrLen(char *str, int length);
void DM_Set_State(int id, State state);
void DM_Set_Refresh(int id, Refresh refresh);
void DM_Remove_Element(int id);
struct DisplayElement DM_Get_Element(int id);
int DM_Parse_Press(int x, int y);
int DM_Do_Press(struct Touch touch);

struct DisplayElement getDefaultElement();

#endif /* INC_DISPLAYMANAGER_H_ */

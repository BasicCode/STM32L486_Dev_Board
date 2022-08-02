/*
 * TextBox.h
 *
 *  Created on: Jul 31, 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_TEXTBOX_H_
#define INC_DISPLAYMANAGER_TEXTBOX_H_

#include "DisplayManager/DisplayManager.h"

struct DisplayElement DM_New_TextBox(int x, int y, int length, char *placeholderText, char *text);

#endif /* INC_DISPLAYMANAGER_TEXTBOX_H_ */

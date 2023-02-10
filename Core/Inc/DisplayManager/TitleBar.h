/*
 * TitleBar.h
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_TITLEBAR_H_
#define INC_DISPLAYMANAGER_TITLEBAR_H_

#include "DisplayManager/DisplayManager.h"

struct DisplayElement DM_New_Title_Bar(char *title);
void DM_Title_Bar_onPress(int id, int x, int y);

#endif /* INC_DISPLAYMANAGER_TITLEBAR_H_ */

/*
 * TickBox.h
 *
 *  Created on: 27 Sep. 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_TICKBOX_H_
#define INC_DISPLAYMANAGER_TICKBOX_H_

#include "DisplayManager/DisplayManager.h"

struct DisplayElement DM_New_TickBox(int x, int y, int selected);
void DM_TickBox_onPress(int id);


#endif /* INC_DISPLAYMANAGER_TICKBOX_H_ */

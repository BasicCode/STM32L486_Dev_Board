/*
 * List.h
 *
 *  Created on: 30 Jul. 2022
 *      Author: tommy
 */

#ifndef INC_DISPLAYMANAGER_LIST_H_
#define INC_DISPLAYMANAGER_LIST_H_

#include "DisplayManager/DisplayManager.h"

struct DisplayElement DM_New_List(int x1, int y1, int x2, int y2, char **children, int numChildren);

#endif /* INC_DISPLAYMANAGER_LIST_H_ */

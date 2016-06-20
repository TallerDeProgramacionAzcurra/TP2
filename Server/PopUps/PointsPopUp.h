/*
 * PointsPopUp.h
 *
 *  Created on: Jun 19, 2016
 *      Author: gonzalo
 */

#ifndef POPUPS_POINTSPOPUP_H_
#define POPUPS_POINTSPOPUP_H_

#include "PopUp.h"

class PointsPopUp: public PopUp
{
public:
	PointsPopUp(int ownerID, int points);

private:
	int pointsToTextureID(int points);
};



#endif /* POPUPS_POINTSPOPUP_H_ */

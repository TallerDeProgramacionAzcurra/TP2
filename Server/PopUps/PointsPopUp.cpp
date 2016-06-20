/*
 * PointsPopUp.cpp
 *
 *  Created on: Jun 19, 2016
 *      Author: gonzalo
 */

#include "PointsPopUp.h"

PointsPopUp::PointsPopUp(int ownerID, int points): PopUp(ownerID)
{
	m_textureID = pointsToTextureID(points);
}

int PointsPopUp::pointsToTextureID(int points)
{
	int textureID = 104;
	switch(points)
	{
	case 50:
		textureID = 101;
		break;
	case 100:
		textureID = 102;
		break;
	case 250:
		textureID = 103;
		break;
	case 500:
		textureID = 104;
		break;
	case 750:
		textureID = 105;
		break;
	case 1000:
		textureID = 106;
		break;
	case 1500:
		textureID = 107;
		break;
	case 3000:
		textureID = 108;
		break;
	case 5000:
		textureID = 109;
		break;
	case 10000:
		textureID = 110;
		break;
	default: textureID = 104;
	}

	return textureID;
}

/*
 * ExtraPoints.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#include "ExtraPointsPU.h"

#include "../Player.h"
#include "../Game.h"

ExtraPointsPU::ExtraPointsPU(): PowerUp(),
							m_pointsToGive(100)
{
}

ExtraPointsPU::ExtraPointsPU(int points): PowerUp(),
							m_pointsToGive(points)
{
}

ExtraPointsPU::~ExtraPointsPU()
{
}

void ExtraPointsPU::update()
{
	PowerUp::update();
}

void ExtraPointsPU::pickUp(Player* player)
{
	Game::Instance()->addPointsToScore(m_pointsToGive, player->getObjectId(), player->getPlayerTeam().gameTeamID);

	m_pickedUp = true;
	m_dead = true;
}

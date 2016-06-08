/*
 * PowerUp.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#include "PowerUp.h"
#include "../Game.h"
#include "../Player.h"

PowerUp::PowerUp(): MoveableObject(),
		m_pickedUp(false),
		m_animatingPickUp(false),
		m_dead(false)
{
	m_tag = "PowerUp";
}

PowerUp::~PowerUp()
{

}

void PowerUp::load(int x, int y, int width, int height, int textureID, int numFrames)
{
	MoveableObject::load(x, y, width, height, textureID, numFrames);
}

void PowerUp::draw()
{
	//MoveableObject::draw();
}
void PowerUp::update()
{
	if (!m_pickedUp)
	{
		MoveableObject::update();
	}
	if (m_dirty)
		sendDrawMessage(!m_dead);
}

void PowerUp::clean()
{
	MoveableObject::clean();
}


bool PowerUp::isVisibleFromCamera()
{
	if ((m_position.m_x - m_width < 0) && (m_position.m_x > Game::Instance()->getGameWidth()) &&
		(m_position.m_y - m_height < 0) && (m_position.m_y > Game::Instance()->getGameHeight()))
	{
		return false;
	}

	return true;
}


bool PowerUp::canPickUp()
{
	return !m_pickedUp;
}


void PowerUp::sendDrawMessage(bool isAlive)
{
	DrawMessage drawMsg;
	drawMsg.unused1 = false;
	drawMsg.connectionStatus = true;
	drawMsg.alive = isAlive;
	drawMsg.hasSound = false;

	drawMsg.objectID = m_objectId;
	drawMsg.layer = m_layer;
	drawMsg.soundID = 0;
	drawMsg.column = m_currentFrame;
	drawMsg.row = m_currentRow;
	drawMsg.posX = m_position.getX();
	drawMsg.posY = m_position.getY();
	drawMsg.textureID = m_textureID;
	drawMsg.angle = static_cast<float>(m_angle);
	drawMsg.alpha = m_alpha;
	drawMsg.vacio = 0;

	if (USE_DRAWMESSAGE_PACKAGING)
	{
		Game::Instance()->addToPackage(drawMsg);
	}
	else
	{
		Game::Instance()->sendToAllClients(drawMsg);
	}
}

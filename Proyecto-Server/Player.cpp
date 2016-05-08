/*
 * Player.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: gonzalo
 */

#include "Player.h"

using namespace std;

Player::Player() :  MoveableObject(),
					m_connected(true),
					m_dead(false),
					m_dying(false)
{
	m_tag = "Player";
	m_layer = FOREGROUND;
	m_currentWeapon = new BasicWeapon();
	m_shootOffset = Vector2D(15, -5);
}

void Player::collision()
{

}

void Player::load(int x, int y, int width, int height, int textureID, int numFrames)
{
    // Load comun. Inicializa variables
    MoveableObject::load(x, y, width, height, textureID, numFrames);

   // TextureManager::Instance()->load("Assets/Sprites/BlackShip.png", m_textureID, Game::Instance()->getRenderer());

    // Otras acciones de inicialización del Player más especificas

}

void Player::draw()
{
    TextureManager::Instance()->drawFrame(m_textureID, m_position.getX(), m_position.getY(), m_width, m_height,
    										m_currentRow, m_currentFrame, Game::Instance()->getRenderer(), m_angle, m_alpha, SDL_FLIP_NONE);
	//Nave::draw();
}

void Player::update()
{
	MoveableObject::update();
	m_currentWeapon->update();

	//Probar valores para animacion
	//m_currentFrame = int(((SDL_GetTicks() / (1000 / 3)) % m_numFrames));

	if (m_dirty)
	{
		sendDrawMessage(true);
		m_dirty = false;
	}

	m_direction.setX(0);
	m_direction.setY(0);
}


void Player::clean()
{
    MoveableObject::clean();

	delete m_currentWeapon;
}

void Player::handleInput(InputMessage inputMsg)
{

    if(!m_dead && !m_dying)
    {
        // handle keys
        if ((inputMsg.buttonUp == 1) && (m_position.getY() > 0))
        {
            m_direction.setY(DIRECTION_UP);
            m_dirty = true;
        }
        else if ((inputMsg.buttonDown == 1) && ((m_position.getY() + m_height) < Game::Instance()->getGameHeight() - 10))
        {
        	m_direction.setY(DIRECTION_DOWN);
            m_dirty = true;
        }

        if ((inputMsg.buttonLeft == 1)	&& m_position.getX() > 0)
        {
        	m_direction.setX(DIRECTION_LEFT);
            m_dirty = true;
        }
        else if ((inputMsg.buttonRight == 1) && ((m_position.getX() + m_width) < Game::Instance()->getGameWidth()))
        {
        	m_direction.setX(DIRECTION_RIGHT);
            m_dirty = true;
        }
        //Se mueve a velocidades constantes. Evita que vaay a mayot velocidad en diagonal
        m_direction.normalize();

        if (inputMsg.buttonShoot)
        {
        	m_currentWeapon->shoot(Vector2D(m_position.getX() + m_shootOffset.getX(), m_position.getY() + m_shootOffset.getY()));
            m_dirty = true;
        }
        //printf("Direcion = %f , %f \n", m_direction.m_x, m_direction.m_y);

    }
    //update();
}

void Player::sendDrawMessage(bool isAlive)
{
	DrawMessage drawMsg;
	drawMsg.connectionStatus = m_connected;
	drawMsg.unused1 = false;
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
	Game::Instance()->sendToAllClients(drawMsg);
}

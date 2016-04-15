/*
 * Player.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: gonzalo
 */

#include "Player.h"

using namespace std;

Player::Player() :  Nave(),
					m_controllable(true)
{
	m_tag = "Player";
}

Player::Player(bool canControl) :  Nave()
{
	m_controllable = canControl;
	m_tag = "Player";
}

void Player::collision()
{

}

void Player::load(int x, int y, int width, int height, std::string textureID, int numFrames)
{
    // Load comun. Inicializa variables
    Nave::load(x, y, width, height, textureID, numFrames);

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
    handleInput();
	Nave::update();
	//Probar valores para animacion
	//m_currentFrame = int(((SDL_GetTicks() / (1000 / 3)) % m_numFrames));
}


void Player::clean()
{
    Nave::clean();
}

void Player::handleInput()
{
	m_direction.setX(0);
	m_direction.setY(0);
    if(m_controllable && !m_dead)
    {
        // handle keys
        if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_W)))
        	&& (m_position.getY() > 0))
        {
            m_direction.setY(DIRECTION_UP);
        }
        else if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_S)))
        		&& ((m_position.getY() + m_height) < Game::Instance()->getGameHeight() - 10))
        {
        	m_direction.setY(DIRECTION_DOWN);
        }

        if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_A)))
        		&& m_position.getX() > 0)
        {
        	m_direction.setX(DIRECTION_LEFT);
        }
        else if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_D)))
        		&& ((m_position.getX() + m_width) < Game::Instance()->getGameWidth()))
        {
        	m_direction.setX(DIRECTION_RIGHT);
        }
        //Se mueve a velocidades constantes. Evita que vaay a mayot velocidad en diagonal
        m_direction.normalize();

        //Codigo de disparo

    }
}

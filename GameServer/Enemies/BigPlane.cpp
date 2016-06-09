/*
 * BigPlane.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#include "BigPlane.h"
#include "../Game.h"
#include "../Vector2D.h"
#include "../Weapons/EnemyWeapons/EnemyBaseGun.h"
#include "../Player.h"


BigPlane::BigPlane() :Enemy(),
					  m_goingUp(true),
					  m_goingRight(false),
					  m_explotionAnimationTime(1000),
					  m_explotionRemainingTime(0)
{
	m_speed = Vector2D(1.4f, 1.75f);
	m_direction.setX(0);
	m_direction.setY(DIRECTION_UP);

	m_width = 128;// hardcodeado como valores iniciales antes de llamar a load
	m_height = 96;

	if (Random::getRange(-1, 1) >= 0)
		m_goingRight = true;

	generateBorderReturnOffset();

	m_health= 1000;
	m_pointOnKill = 1000;
	m_pointOnHit = 100;
	m_enemyWeapon = new EnemyBaseGun();
	setWeaponStats(4, 300, m_objectId, -1);

	m_shootingOffset = Vector2D(m_width/2, m_height/2);

}

BigPlane::~BigPlane()
{

}

void BigPlane::load(int x, int y, int width, int height, int textureID, int numFrames)
{
	 Enemy::load(x, y, width, height, textureID, numFrames);
	 m_hitRadius = m_width/2.5f;
}

void BigPlane::update()
{
	if (m_exploting)
	{
		updateExplotionAnimation();
	}

	m_enemyWeapon->update();

	if (!m_dead && !m_dying)
	{
		//Si esta en la parte de abajo de la pantalla, Sube
		if (m_position.getY() >= (Game::Instance()->getGameHeight()/3) - m_height)
		{
			m_direction.setX(0);
			m_direction.setY(DIRECTION_UP);
			m_goingUp = true;
		}
		else
		{
			m_goingUp = false;
		}

		//Si llega a un borde, invierta la dirección en X
		if ((!m_goingRight) && (m_position.getX() <= m_borderReturnOffset))
		{
			flip();
		}
		if ((m_goingRight) && (m_position.getX() >= m_borderReturnOffset))
		{
			flip();
		}

		//Si no esta subiendo, tiende a moverse hacia la derecha o izquierda, segun corresponda
		if (!m_goingUp)
		{
			if (m_goingRight)
			{
				m_direction.setX(DIRECTION_RIGHT);
				m_direction.m_y += Random::getFloatRange(-0.25f, 0.25f);
				m_direction.normalize();
			}
			else
			{
				m_direction.setX(DIRECTION_LEFT);
				m_direction.m_y += Random::getFloatRange(-0.25f, 0.25f);
				m_direction.normalize();
			}
		}

		if (m_position.getY() < 50)
		{
			m_direction.m_y = DIRECTION_DOWN;
		}


		//Analiza si debe disparar
		if (!m_goingUp)
		{
			int shootLuck = Random::getRange(0, 1000);
			if (shootLuck <= SHOOT_CHANCE)
			{
				shoot();
			}
		}
	}

	//Actualiza la posición y comunica a los clientes
	Enemy::update();

}

void BigPlane::randomizeY()
{

}

bool BigPlane::damage(int damageReceived, Player* player)
{
	bool killed = false;

	//Daña al avion
	m_health -= damageReceived;
	if (player)
	{
		Game::Instance()->addPointsToScore(m_pointOnHit, player->getObjectId(), player->getTeamNumber());
	}

	//Si lo mato suma puntos al player que lo mato
	if (m_health <= 0)
	{
		m_dying = true;
		explote();
		if (canRetrievePoints() && player)
		{
			int points = retrievePoints();
			Game::Instance()->addPointsToScore(points, player->getObjectId(), player->getTeamNumber());
		}
	}
	return killed;
}

void BigPlane::shoot()
{
	m_target = Game::Instance()->getRandomPLayerCenter();

	if (m_enemyWeapon)
	{
		Vector2D shootDirection;
		shootDirection.setX(m_target.m_x - m_position.m_x);
		shootDirection.setY(m_target.m_y - m_position.m_y);
		shootDirection.normalize();
		Vector2D shootPosition = m_shootingOffset + m_position;
		m_enemyWeapon->shoot(shootPosition, shootDirection);
	}
}

void BigPlane::generateBorderReturnOffset()
{
	if (m_goingRight)
	{
		m_borderReturnOffset = Game::Instance()->getGameWidth() - m_width - Random::getRange(0, 50);
	}
	else
	{
		m_borderReturnOffset = Random::getRange(0, 50);
	}
}

void BigPlane::flip()
{
	if (m_goingRight)
	{
		m_goingRight = false;
		m_direction.setX(DIRECTION_LEFT);
		m_direction.setY(m_direction.m_y * (-1));
	}
	else
	{
		m_goingRight = true;
		m_direction.setX(DIRECTION_RIGHT);
		m_direction.setY(m_direction.m_y * (-1));
	}

	generateBorderReturnOffset();

}


void BigPlane::explote()
{
	m_exploting = true;
	m_explotionRemainingTime = m_explotionAnimationTime;
	//hardcodeado por ahora
	/*m_numFrames = 12;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_textureID = 31;*/
}

void BigPlane::updateExplotionAnimation()
{
	m_explotionRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_explotionAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	int lastRow = m_currentRow;

	m_currentFrame = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) % 4;
	m_currentRow = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) / 4;

	if (m_explotionRemainingTime <= 0)
	{
		m_dying = false;
		m_dead = true;
		m_exploting = false;
	}

	if ((lastFrame != m_currentFrame) || (lastRow != m_currentRow))
	{
		m_dirty = true;
	}
}

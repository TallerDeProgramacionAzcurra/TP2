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
					m_doingFlip(false),
					m_flipAnimationTime(FLIP_ANIMATION_TIME),
					m_flipRemainingTime(0),
				    m_explotionAnimationTime(EXPLOTION_ANIMATION_TIME),
				    m_explotionRemainingTime(0),
					m_holdQuietTimer(100),
					m_currentHoldQuietTime(0),
					m_dead(false),
					m_dying(false),
					m_exploting(false),
					m_invulnerable(false),
					m_health(100),
					m_collisionDamage(100),
					m_movedByPlayer(false),
					m_teamNumber(1)
{
	m_score = Score();
	m_tag = "Player";
	m_layer = FOREGROUND;
	m_currentWeapon = new BasicWeapon();
	m_shootOffset = Vector2D(15, -5);
}

void Player::collision()
{

}

void Player::damage(int damageReceived)
{
	if (m_invulnerable)
	{
		return;
	}
	m_health -= damageReceived;
	if (m_health <= 0)
	{
		//Hacer explosion, setear dying en true, etc
		m_dying = true;
		explote();
	}
}

void Player::setShootingSpeed(int speed)
{
	if (m_currentWeapon)
	{
		m_currentWeapon->setBulletSpeed(speed);
	}
}
void Player::setShootingCooldown(int cooldown)
{
	if (m_currentWeapon)
	{
		m_currentWeapon->setShootingCooldown(cooldown);
	}
}

void Player::setWeapon(Weapon* weapon)
{
	if (!weapon)
		return;
	delete m_currentWeapon;
	m_currentWeapon = weapon;
}

void Player::setWeaponStats(int shootingSpeed, int shootingCooldown, int ownerID, int teamID)
{
	if (m_currentWeapon)
	{
		m_currentWeapon->setBulletSpeed(shootingSpeed);
		m_currentWeapon->setShootingCooldown(shootingCooldown);
		m_currentWeapon->setOwnerID(ownerID);
		m_currentWeapon->setOwnerTeam(teamID);
	}
}


void Player::StopFlipAnimation()
{
	//m_doingFlip = false;
	m_flipRemainingTime = 0;
	m_invulnerable = false;
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
	//Nave::draw();    	int dy = 0;
	int dx = 0;
}

void Player::update()
{
	//Workaround para que no aparezca invisible
	if (!m_movedByPlayer)
	{
		m_dirty = true;
	}

	if (m_exploting)
	{
		updateExplotionAnimation();
	}

	//printf("currentFrame: %d \n", m_currentFrame);
	//printf("doing flip: %d \n", m_doingFlip);
	if (m_doingFlip)
	{
		updateFlipAnimation();
	}

	//Si esta girando no actualiza posicion
	if (!m_doingFlip)
	{
		MoveableObject::update();

		//FUNCIONALIDAD DE ARRASTRE, POR LAS DUDAS LO DEJO
		if (DRAG_PLAYER)
		{
			// si esta quieto Y no esta enl borde de abajo, lo empuja hacia abajo. Si esta desconectado se comporta de acuerdo a DRAG_DISCONNECTED_PLAYER
			if ((m_direction.getX() == 0) && (m_direction.getY() == 0) &&
				(((m_position.getY() + m_height) < Game::Instance()->getGameHeight() - 10)))
			{
				//Esta quieto. Iniciar Timer para arrastrar
				if (m_currentHoldQuietTime >= m_holdQuietTimer)
				{
					m_position.m_y += Game::Instance()->getScrollSpeed();
					m_dirty = true;
				}
				else
				{
					m_currentHoldQuietTime += GameTimeHelper::Instance()->deltaTime();
				}
			}
		}
	}

	m_currentWeapon->update();


	if (m_dirty)
	{
		sendDrawMessage(true);
		m_dirty = false;
	}

	m_direction.setX(0);
	m_direction.setY(0);
}

void Player::updateFlipAnimation()
{
	m_flipRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_flipAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	m_currentFrame = (m_flipAnimationTime - m_flipRemainingTime) / step;

	if (m_flipRemainingTime <= 0)
	{
		m_currentFrame = 0;
		m_doingFlip = false;
		m_invulnerable = false;
	}

	if (lastFrame != m_currentFrame)
		m_dirty = true;
}

void Player::explote()
{
	m_exploting = true;
	m_explotionRemainingTime = m_explotionAnimationTime;
	//hardcodeado por ahora
	m_numFrames = 14;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_textureID = 41;
	m_soundDirty = true;

    m_soundSendId = 53;
}

void Player::updateExplotionAnimation()
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


void Player::clean()
{
    MoveableObject::clean();

    if (m_currentWeapon)
    	delete m_currentWeapon;
}

void Player::handleInput(InputMessage inputMsg)
{
	if (!m_movedByPlayer)
	{
		m_movedByPlayer = true;
	}

	if (inputMsg.togglePracticeMode == 1)
	{
		bool practiceMode = (!(Game::Instance()->isPracticeMode()));
		Game::Instance()->setPracticeMode(practiceMode);
	}

    if(!m_dead && !m_dying)
    {
    	//Si esta girando no admite inputs de disparo ni movimiento
    	if(m_doingFlip)
    	{
    		return;
    	}

        // handle keys
    	int dy = 0;
    	int dx = 0;
        if ((inputMsg.buttonUp == 1) && (m_position.getY() > 0))
        {
            --dy;
            m_currentHoldQuietTime = 0;
            m_dirty = true;
        }
        if ((inputMsg.buttonDown == 1) && ((m_position.getY() + m_height) < Game::Instance()->getGameHeight() - 10))
        {
        	++dy;
        	m_currentHoldQuietTime = 0;
            m_dirty = true;
        }

        if ((inputMsg.buttonLeft == 1)	&& m_position.getX() > 0)
        {
        	--dx;
        	m_currentHoldQuietTime = 0;
            m_dirty = true;
        }
        if ((inputMsg.buttonRight == 1) && ((m_position.getX() + m_width) < Game::Instance()->getGameWidth()))
        {
        	++dx;
        	m_currentHoldQuietTime = 0;
            m_dirty = true;
        }
        m_direction.setX(dx);
        m_direction.setY(dy);
        //Se mueve a velocidades constantes. Evita que vaay a mayot velocidad en diagonal
        m_direction.normalize();

        if (inputMsg.buttonShoot)
        {
        	m_soundDirty = true;

        	m_soundSendId = 51;
        	m_currentWeapon->shoot(Vector2D(m_position.getX() + m_shootOffset.getX(), m_position.getY() + m_shootOffset.getY()),
        							Vector2D(0, DIRECTION_UP));
            m_dirty = true;
        }

        if (inputMsg.buttonRoll)
        {
        	if (!m_doingFlip)
        	{
        		m_flipRemainingTime = m_flipAnimationTime;
        		m_doingFlip = true;
        		m_invulnerable = true;
        	}
            m_dirty = true;
        }
    }
}

void Player::addPoints(int points)
{
	m_score.addPoints(points);

	ScoreMessage scoreMsg;
	scoreMsg.playerID = m_objectId;
	scoreMsg.pointsacquire = points;
	scoreMsg.teamID = m_teamNumber;
	scoreMsg.somethingElse = 0;

	Game::Instance()->sendScoreToClients(scoreMsg);
}

const int Player::getScore()
{
	return m_score.getScore();
}

void Player::reset()
{
	m_connected = true;
	m_doingFlip = false;
	m_flipAnimationTime = FLIP_ANIMATION_TIME;
	m_flipRemainingTime = 0;
    m_explotionAnimationTime = EXPLOTION_ANIMATION_TIME;
    m_explotionRemainingTime = 0;
	m_currentHoldQuietTime = 0;
	m_dead = false;
	m_dying = false;
	m_exploting = false;
	m_health = 100;
	m_movedByPlayer = false;

	m_score.reset();
}


void Player::sendDrawMessage(bool isAlive)
{
	DrawMessage drawMsg;
	drawMsg.connectionStatus = m_connected;
	drawMsg.unused1 = false;
	drawMsg.alive = isAlive;

	if(m_soundDirty)
	{
		drawMsg.hasSound = true;
		m_soundDirty = false;

	}else
	{
		drawMsg.hasSound = false;
	}

	drawMsg.objectID = m_objectId;
	drawMsg.layer = m_layer;
	drawMsg.soundID = m_soundSendId;
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

/*
 * CollisionHandler.cpp
 *
 *  Created on: May 22, 2016
 *      Author: gonzalo
 */

#include "CollisionHandler.h"
#include "../PowerUps/PowerUp.h"
#include "../Enemies/Enemy.h"
#include "../Player.h"
#include "../Weapons/Bullet.h"
#include "../GameObject.h"
#include "../Game.h"

CollitionHandler* CollitionHandler::s_pInstance;


CollitionHandler::CollitionHandler()
{

}

CollitionHandler::~CollitionHandler()
{

}

void CollitionHandler::handleCollitions()
{
	handlePlayerCollitions();
	handleEnemyCollitions();
}

void CollitionHandler::handlePlayerCollitions()
{
	for( vector<Player*>::iterator playersIterator = m_players.begin(); playersIterator != m_players.end(); ++playersIterator)
	{
		if ((*playersIterator)->isDead() || (*playersIterator)->isDying())
			continue;

		//Compara cada jugador contra cada bala enemiga
		for(vector<std::shared_ptr<Bullet>>::iterator it = m_enemiesBullets.begin(); it != m_enemiesBullets.end(); )
		{
			if ((*it)->isDead())
			{
				it = m_enemiesBullets.erase(it);
				continue;
			}

			if (areColliding((*playersIterator), (*it).get()))
			{
				printf("Colision de player con bala\n");
				//Hay colision del jugador con una bala enemiga
				//todo setea la bala en muerta, deberia tal vez hacer una explosion primero y dsp de la animacion matarla
				(*playersIterator)->damage((*it)->getDamage());
				(*it)->kill(); //mata la bala

				//elimina bala del chekeo de colisiones
				it = m_enemiesBullets.erase(it);
			}
			else
		    {
			  ++it;
		    }
		}

		//Compara cada Jugador con cada PowerUp
		for(vector<PowerUp*>::iterator it = m_powerUps.begin(); it != m_powerUps.end(); )
		{
			if ((*it)->isDead() || ( ((*it)->canPickUp()) == false ))
			{
				it = m_powerUps.erase(it);
				continue;
			}

			if (areColliding((*playersIterator), (*it)))
			{
				printf("Colision de player con powerUp\n");
				//Hay colision del jugador con un power up

				if ((*playersIterator))
					(*it)->pickUp((*playersIterator));

				//elimina el powerUp del checkeo de colisiones
				it = m_powerUps.erase(it);
			}
			else
		    {
			  ++it;
		    }
		}

		//Compara cada jugador con cada enemigo
		for(vector<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end(); )
		{
			if ((*it)->isDead() || (*it)->isDying())
			{
				it = m_enemies.erase(it);
				continue;
			}

			if (areColliding((*playersIterator), (*it)))
			{
				//Hay colision del enemigo con una bala de un jugador
				printf("Colision de player con enemigo\n");

				//daña al jugador
				(*playersIterator)->damage((*it)->getCollisionDamage());
				//daña al enemigo
				bool enemyKilled = (*it)->damage((*playersIterator)->getCollisionDamage());
				if ((enemyKilled) && ((*it)->canRetrievePoints()))
				{
					Game::Instance()->addPointsToScore((*it)->retrievePoints(), (*playersIterator)->getObjectId(), (*playersIterator)->getTeamNumber());
				}

				//elimina al enemigo del chekeo de colisiones si el enemigo esta muerto o muriendo
				if ((*it)->isDead() || (*it)->isDying())
				{
					it = m_enemies.erase(it);
					continue;
				}
			}
			//continua en el siguiente
			++it;

		}
	}
}

void CollitionHandler::handleEnemyCollitions()
{
	addNewPlayerBullets();//Para evitar agregarlas mientras loopea. Las almacena y las agrega en el proximo frame (este)

	for( vector<Enemy*>::iterator enemiesIterator = m_enemies.begin(); enemiesIterator != m_enemies.end(); )
	{
		if ((*enemiesIterator)->isDead() || (*enemiesIterator)->isDying())
		{
			enemiesIterator = m_enemies.erase(enemiesIterator);
			continue;
		}

		//Compara cada jugador contra cada bala enemiga
		for(vector<std::shared_ptr<Bullet>>::iterator it = m_playersBullets.begin(); it != m_playersBullets.end(); )
		{
			if ((*it)->isDead())
			{
				it = m_playersBullets.erase(it);
				continue;
			}

			if (areColliding((*enemiesIterator), (*it).get()))
			{
				printf("Colision de bala con enemigo\n");
				//Hay colision del jugador con una bala enemiga
				//todo setea la bala en muerta, deberia tal vez hacer una explosion primero y dsp de la animacion matarla
				bool enemyKilled = (*enemiesIterator)->damage((*it)->getDamage());
				(*it)->kill(); //mata la bala
				if ((enemyKilled) && ((*enemiesIterator)->canRetrievePoints()))
				{
					Game::Instance()->addPointsToScore((*enemiesIterator)->retrievePoints(), (*it)->getOwnerID(), (*it)->getOwnerTeamNumber());
				}

				//elimina bala del chekeo de colisiones
				it = m_playersBullets.erase(it);
			}
			else
		    {
			  ++it;
		    }
		}

		++enemiesIterator;
	}
}

bool CollitionHandler::areColliding(GameObject* gameObjectOne, GameObject* gameObjectTwo)
{
	bool collision = false;
	int minimumCollisionDistance = gameObjectOne->getHitRadius() + gameObjectTwo->getHitRadius();
	//calcula el cuadrado para reducir el costo
	minimumCollisionDistance = minimumCollisionDistance * minimumCollisionDistance;

	//calcula el centro de los objetos
	Vector2D gameObjectOneCenter;
	gameObjectOneCenter.setX(gameObjectOne->getPosition().m_x + (gameObjectOne->getWidth()/2));
	gameObjectOneCenter.setY(gameObjectOne->getPosition().m_y + (gameObjectOne->getHeight()/2));

	Vector2D gameObjectTwoCenter;
	gameObjectTwoCenter.setX(gameObjectTwo->getPosition().m_x + (gameObjectTwo->getWidth()/2));
	gameObjectTwoCenter.setY(gameObjectTwo->getPosition().m_y + (gameObjectTwo->getHeight()/2));

	//calcula la distancia
	Vector2D distanceVector = gameObjectOneCenter - gameObjectTwoCenter;
	//calcula la longitud del vector distancia
	int SquareDistance = (distanceVector.m_x * distanceVector.m_x) + (distanceVector.m_y * distanceVector.m_y);
	if (SquareDistance <= minimumCollisionDistance)
	{
		//Hubo colision
		collision = true;
	}
	return collision;

}

void CollitionHandler::addPlayer(Player* player)
{
	m_players.push_back(player);
}

void CollitionHandler::addEnemy(Enemy* enemy)
{
	m_enemies.push_back(enemy);
}

void CollitionHandler::addPowerUp(PowerUp* powerUp)
{
	m_powerUps.push_back(powerUp);
}

void CollitionHandler::addPlayerBullet(std::shared_ptr<Bullet> playerBullet)
{
	m_playerBulletsToAdd.push_back(playerBullet);
	//m_playersBullets.push_back(playerBullet);
}

void CollitionHandler::addNewPlayerBullets()
{
	for (std::vector<std::shared_ptr<Bullet>>::iterator it = m_playerBulletsToAdd.begin() ; it != m_playerBulletsToAdd.end(); ++it)
	{
		m_playersBullets.push_back((*it));
	}
	m_playerBulletsToAdd.clear();
}

void CollitionHandler::addEnemyBullet(std::shared_ptr<Bullet> enemyBullet)
{
	m_enemiesBullets.push_back(enemyBullet);
}

void CollitionHandler::reset()
{
	m_enemies.clear();
	m_playersBullets.clear();
	m_enemiesBullets.clear();
	m_powerUps.clear();
}

void CollitionHandler::clean()
{
	//El delete se hace en otro lado. El collition Handler solo los usa para colisiones pero no modifica los punteros
	m_players.clear();
	m_enemies.clear();
	m_playersBullets.clear();
	m_enemiesBullets.clear();
	m_powerUps.clear();
}

/*
 * EnemySpawner.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: gonzalo
 */

#include "EnemySpawner.h"
#include "../Enemies/Enemy.h"
#include "../Enemies/SmallEnemy.h"
#include "../Enemies/Formation.h"
#include "../Enemies/BigPlane.h"
#include "../Utils/TextureHelper.h"

EnemySpawner::EnemySpawner()
{
	m_textureHelper = new TextureHelper();
}

EnemySpawner::~EnemySpawner()
{
	m_enemiesToSpawn.clear();
}

void EnemySpawner::clean()
{
	for (std::vector<EnemySpawnInfo>::iterator it = m_enemiesToSpawn.begin() ; it != m_enemiesToSpawn.end(); ++it)
	{
		if ((*it).gameObjectToSpawn)
		{
			delete (*it).gameObjectToSpawn;
		}
	}
	m_enemiesToSpawn.clear();
	delete m_textureHelper;
}


void EnemySpawner::feed(std::vector<Enemigo> enemies)
{
	for (std::vector<Enemigo>::iterator it = enemies.begin() ; it != enemies.end(); ++it)
	{

	}
}

/*void EnemySpawner::addEnemy(Enemigo enemyInfo)
{
	if (enemyInfo.id.compare("AvionPequeño") == 0)
	{

	}

	int enemyID = m_textureHelper->stringToInt(enemyInfo.id);
}

Enemy* EnemySpawner::createEnemy(std::string enemyNameID)
{

}*/

/*
 * PowerUpSpawner.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: gonzalo
 */

#include "PowerUpSpawner.h"

void PowerUpSpawner::clean()
{
	for (std::vector<PowerUpSpawnInfo>::iterator it = m_powerUpsToSpawn.begin() ; it != m_powerUpsToSpawn.end(); ++it)
	{
		if ((*it).powerUpToSpawn)
		{
			delete (*it).powerUpToSpawn;
		}
	}
	m_powerUpsToSpawn.clear();
	//delete m_textureHelper;
}

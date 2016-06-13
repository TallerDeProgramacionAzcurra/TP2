/*
 * PowerUpSpawner.h
 *
 *  Created on: Jun 12, 2016
 *      Author: gonzalo
 */

#ifndef SPAWNERS_POWERUPSPAWNER_H_
#define SPAWNERS_POWERUPSPAWNER_H_

#include "../Utils/TiposDefinidos.h"
#include <vector>
class PowerUp;

class PowerUpSpawner
{
public:

	void feed(std::vector<Powerup> powerUps);
	void clean();

private:

	std::vector<PowerUpSpawnInfo> m_powerUpsToSpawn;

};


#endif /* SPAWNERS_POWERUPSPAWNER_H_ */

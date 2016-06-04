/*
 * BulletsHandler.cpp
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#include "BulletsHandler.h"
#include "../Game.h"
#include "Bullet.h"


BulletsHandler* BulletsHandler::s_pInstance = new BulletsHandler();

BulletsHandler::BulletsHandler()
{
	pthread_mutex_init(&m_bulletsMutex, NULL);
}

BulletsHandler::~BulletsHandler()
{
	  pthread_mutex_destroy(&m_bulletsMutex);
}

void BulletsHandler::addBullet(std::shared_ptr<Bullet> bullet)
{
    m_bullets.push_back(bullet);
    bullet.reset();
}

void BulletsHandler::clearBullets()
{
	 pthread_mutex_lock(&m_bulletsMutex);
	for (std::vector< std::shared_ptr<Bullet> >::iterator it = m_bullets.begin() ; it != m_bullets.end(); ++it)
	{
		if ((*it))
		{
			(*it).reset();
		}
	}
	m_bullets.clear();
	pthread_mutex_unlock(&m_bulletsMutex);
}

void BulletsHandler::updateBullets()
{
    for (std::vector<std::shared_ptr<Bullet>>::iterator p_it = m_bullets.begin(); p_it != m_bullets.end();)
    {
    	if (!(*p_it))
    	{
    		p_it = m_bullets.erase(p_it);
    		continue;
    	}
        if((*p_it)->getPosition().getX() < -20 || (*p_it)->getPosition().getX() > Game::Instance()->getGameWidth()
           || (*p_it)->getPosition().getY() < -20 || (*p_it)->getPosition().getY() > Game::Instance()->getGameHeight() || (*p_it)->isDead())
        {
        	if ((*p_it))
        	{
				(*p_it)->clean();
				(*p_it).reset();
				p_it = m_bullets.erase(p_it);
        	}
        }
        else
        {
            (*p_it)->update();
            ++p_it;
        }
    }
}


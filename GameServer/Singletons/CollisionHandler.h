/*
 * CollisionHandler.h
 *
 *  Created on: May 22, 2016
 *      Author: gonzalo
 */

#ifndef SINGLETONS_COLLISIONHANDLER_H_
#define SINGLETONS_COLLISIONHANDLER_H_

#include "../Vector2D.h"
#include <vector>
#include <memory>
class Bullet;
class Enemy;
class Player;
class GameObject;

class CollitionHandler
{
public:
    static CollitionHandler* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new CollitionHandler();
            return s_pInstance;
        }
        return s_pInstance;
    }

    void handleCollitions();
    void reset();

    void addPlayer(Player* player);
    void addEnemy(Enemy* enemy);
    void addPlayerBullet(std::shared_ptr<Bullet> playerBullet);
    void addEnemyBullet(std::shared_ptr<Bullet> enemyBullet);

    void clean();

private:

    static CollitionHandler* s_pInstance;

    std::vector<Player*> m_players;
    std::vector<Enemy*> m_enemies;

    std::vector<std::shared_ptr<Bullet>> m_playersBullets;
    std::vector<std::shared_ptr<Bullet>> m_enemiesBullets;

    std::vector<std::shared_ptr<Bullet>> m_playerBulletsToAdd;
    void addNewPlayerBullets();

    //chekea player contra balas enemigas y enemigos
    void handlePlayerCollitions();

    //chekea enemigos contra balas de players (contra players ya se chekea en handle player collitions)
    void handleEnemyCollitions();

    //hit box circular
    //Devuelve true si hay colision
    bool areColliding(GameObject* gameObjectOne, GameObject* gameObjectTwo);

    CollitionHandler();
    ~CollitionHandler();

    CollitionHandler(const CollitionHandler&);
    CollitionHandler& operator=(const CollitionHandler&);
};


#endif /* SINGLETONS_COLLISIONHANDLER_H_ */

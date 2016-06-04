/*
 * Enemy.h
 *
 *  Created on: May 22, 2016
 *      Author: gonzalo
 */

#ifndef ENEMIES_ENEMY_H_
#define ENEMIES_ENEMY_H_

#include "../MoveableObject.h"
#include "../Utils/TiposDefinidos.h"
class Weapon;

class Enemy : public MoveableObject
{
public:

    virtual ~Enemy();

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void draw();
    virtual void update();
    virtual void clean();

    virtual bool damage(int damageReceived);

    virtual void shoot();

    virtual void collision();

    void setWeaponStats(int shootingSpeed, int shootingCooldown, int ownerID, int teamID);

    void setCollisionDamage(int collisionDamage) { m_collisionDamage = collisionDamage; }

    bool isDead() { return m_dead; }
    bool isDying() { return m_dying; }
    int getCollisionDamage() { return m_collisionDamage; }
    int retrievePoints();
    bool canRetrievePoints();

    bool isVisibleFromCamera();


    void sendDrawMessage(bool isAlive);

    Vector2D m_target;


protected:

    Enemy();

    Weapon* m_enemyWeapon;

    int m_health;
    int m_collisionDamage;

    bool m_dead;
    bool m_dying;
    bool m_pointsRetrieved;

    int m_pointOnKill;
};


#endif /* ENEMIES_ENEMY_H_ */

/*
 * BigPlane.h
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#ifndef ENEMIES_BIGPLANE_H_
#define ENEMIES_BIGPLANE_H_

#include "Enemy.h"

class BigPlane: public Enemy
{
public:
	BigPlane();
    virtual ~BigPlane();

    virtual void update();
    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void shoot();

    virtual bool damage(int damageReceived, Player* damager);

private:

    const int m_shootChance;

    bool m_goingUp;
    bool m_goingRight;

    int m_borderReturnOffset;
    Vector2D m_shootingOffset;

    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    void flip();
    void updateExplotionAnimation();
    void explote();

    void generateBorderReturnOffset();
};



#endif /* ENEMIES_BIGPLANE_H_ */

/*
 * BigPlane.h
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#ifndef ENEMIES_BIGPLANE_H_
#define ENEMIES_BIGPLANE_H_

#include "Enemy.h"

#define SHOOT_CHANCE 10

class BigPlane: public Enemy
{
public:
	BigPlane();
    virtual ~BigPlane();

    virtual void update();
    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void shoot();

    virtual bool damage(int damageReceived, Player* player);

private:
    bool m_goingUp;
    bool m_goingRight;

    int m_borderReturnOffset;
    Vector2D m_shootingOffset;

    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    void flip();
    void updateFlipAnimation();
    void updateExplotionAnimation();
    void explote();

    void generateBorderReturnOffset();
    void randomizeY();
};



#endif /* ENEMIES_BIGPLANE_H_ */

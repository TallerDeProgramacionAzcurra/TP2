/*
 * PowerUp.h
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#ifndef POWERUPS_POWERUP_H_
#define POWERUPS_POWERUP_H_

#include "../MoveableObject.h"
class Player;

class PowerUp: public MoveableObject
{
public:

    virtual ~PowerUp();

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void draw();
    virtual void update();
    virtual void clean();

    virtual void pickUp(Player* player) = 0;

    bool canPickUp();

    bool isVisibleFromCamera();

    void sendDrawMessage(bool isAlive);

    bool isDead() { return m_dead; }

protected:

    PowerUp();
    bool m_pickedUp;
    bool m_animatingPickUp;
    bool m_dead;

};



#endif /* POWERUPS_POWERUP_H_ */

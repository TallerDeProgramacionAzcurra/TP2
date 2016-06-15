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
    void doAppearAnimation();

protected:

    PowerUp();
    bool m_pickedUp;
    bool m_animatingPickUp;
    bool m_dead;
    bool m_appearing;

    int m_savedTextureID;
    int m_savedNumFrames;
    int m_appearenceAnimationTime;
    int m_appearenceRemainingTime;
    void updateAppearingAnimation();

};



#endif /* POWERUPS_POWERUP_H_ */

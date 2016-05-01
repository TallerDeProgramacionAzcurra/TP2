/*
 * Player.h
 *
 *  Created on: Apr 9, 2016
 *      Author: gonzalo
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Game.h"
#include "MoveableObject.h"
#include "Singletons/InputHandler.h"
#include "Singletons/TextureManager.h"


class Player : public MoveableObject
{
public:

	//constructor sin argumento: setea m_controllable siempre en true
    Player();
    //constructor que permite determinar si el player reacciona o no a eventos de input. Si se puede o no controlar
    Player(bool canControl);
    virtual ~Player() {}

    virtual void load(int x, int y, int width, int height, std::string textureID, int numFrames);

    virtual void draw();
    virtual void update();
    virtual void handleInput();
    virtual void clean();

    virtual void collision();

    //Getters
    bool isDead() { return m_dead; }
    bool isDying() { return m_dying; }


private:

    // Determina si el usuario puede controlar este player o no. Sirve por si hay varias instancias de Player en juego.
    // Si m_controllable es false, el Player no reaccionará a eventos de Input
    bool m_controllable;

    // variables de control de estado
    bool m_dead;
    bool m_dying;

    //Modifica el estado del juego de acuerdo al input del jugador
};


#endif /* PLAYER_H_ */

/*
 * Game.h
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#ifndef GAME_H_
#define GAME_H_

#include "Player.h"
#include "Singletons/InputHandler.h"
#include "Singletons/TextureManager.h"
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

class Player;

class Game
{
public:

    static Game* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new Game();
            return s_pInstance;
        }
        return s_pInstance;
    }


    bool init(const char* title, int xpos, int ypos, int width, int height, int SDL_WINDOW_flag);

    //Funciones generales del ciclo de jueo
    void render();
    void update();
    void handleEvents();
    void clean();

    SDL_Renderer* getRenderer() const { return m_pRenderer; }
    SDL_Window* getWindow() const { return m_pWindow; }

    bool isRunning() { return m_running; }

    void quit() { m_running = false; }

    //Alto y Ancho de la ventana de juego
    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }

    float getScrollSpeed() { return m_scrollSpeed; }


private:

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    //Provisorio
    Player* m_player;

    bool m_running;

    static Game* s_pInstance;

    int m_gameWidth;
    int m_gameHeight;
    float m_scrollSpeed;

    Game();
    ~Game();
    Game(const Game&);
	Game& operator=(const Game&);
};


#endif /* GAME_H_ */

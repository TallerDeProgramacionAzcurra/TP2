/*
 * Game.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#include "Game.h"

Game* Game::s_pInstance = 0;

Game::Game():
m_pWindow(0),
m_pRenderer(0),
m_running(false),
m_scrollSpeed(0.8)
{

}

Game::~Game()
{
    // we must clean up after ourselves to prevent memory leaks
    m_pRenderer= 0;
    m_pWindow = 0;
}


bool Game::init(const char* title, int xpos, int ypos, int width, int height, int SDL_WINDOW_flag)
{
    // Tamaño de la ventana
    m_gameWidth = width;
    m_gameHeight = height;

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "SDL init success\n";

        m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_flag);

        if(m_pWindow != 0)
        {
            cout << "window creation success\n";
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);

            if(m_pRenderer != 0)
            {
                cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(m_pRenderer, 0,0,0,255);
            }
            else
            {
                cout << "renderer init fail\n";
                return false;
            }
        }
        else
        {
            cout << "window init fail\n";
            return false;
        }
    }
    else
    {
        cout << "SDL init fail\n";
        return false;
    }


    //Provisorio
    m_player = new Player(true);
    m_player->load(m_gameWidth/2, m_gameHeight/2, 38, 64, "blackship", 1);
    TextureManager::Instance()->load("Assets/Sprites/BlackShip.png", "blackship", m_pRenderer);

    //tudo ben
    m_running = true;
    return true;
}

void Game::render()
{
    SDL_RenderClear(m_pRenderer);

    //Dibujar lo que haya que dibujar
    m_player->draw();//Provisorio

    SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
	m_player->update(); // Provisorio
}

void Game::handleEvents()
{
	InputHandler::Instance()->update();
}

void Game::clean()
{
    cout << "cleaning game\n";

    delete m_player; //Provisorio
    InputHandler::Instance()->clean();

    TextureManager::Instance()->clearTextureMap();

    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

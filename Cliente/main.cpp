/*
 * main.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#include "Game.h"
#include "Utils/Random.h"
#include <SDL2/SDL.h>
#include "Singletons/GameTimeHelper.h"
#include <iostream>

using namespace std;

const int FPS = 100;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char **argv)
{
	Random::initialize();
	Game::Instance()->askForName();

	if(Game::Instance()->initializeClient()){
			nombre:
			//Crea un juego con la ventanita bla bla
			Game::Instance()->createGame(DELAY_TIME);
		}


	if(Game::Instance()->getRestart()){
					std::cout << "Apretaste Restart\n";
					Game::Instance()->mrMusculo();
					Game::Instance()->setRunning(true);
					goto nombre;
				}

	//printf("FPS PROMEDIO: %d \n", (fpsCount/ framesCount));
    std::cout << "game closing...\n";
    Game::Instance()->clean();

    return 0;

}

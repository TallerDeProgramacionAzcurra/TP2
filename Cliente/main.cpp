/*
 * main.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */
#include <SDL2/SDL.h>
#include <iostream>

#include "Singletons/GameTimeHelper.h"
#include "Menu/ClienteMenu.hpp"
#include "Utils/Random.h"
#include "Game.h"

using namespace std;

const int FPS = 100;
const int DELAY_TIME = 1000.0f / FPS;

bool mainLoop() {
    Game::Instance()->createGame(DELAY_TIME);
    
    if (Game::Instance()->getRestart()) {
        std::cout << "Apretaste Restart\n";
        Game::Instance()->mrMusculo();
        Game::Instance()->setRunning(true);
        return true;
    }
    
    return false;
}

int main(int argc, char **argv)
{
    // initialization.
    Random::initialize();
    
    if (Game::Instance()->initializeClient() == false) {
        return 0;
    }
    
    ClientMenu clientMenu = ClientMenu("Menú", 800, 600, {});
    clientMenu.clientMenuRun();
    
    std::string playerNameSelected = clientMenu.clientMenuGetPlayerName();
    std::string playerTeamSelected = clientMenu.clientMenuGetPlayerTeam();
    bool playButtonSelected = clientMenu.clienMenuGetPlayButtonSelected();
    
    if (playButtonSelected == true) {
        // Play the Game.
        Game::Instance()->gameSetPlayerName(playerNameSelected);
        
        bool continueLooping = true;
        while (continueLooping == true) {
            continueLooping = mainLoop();
        }
        
        //printf("FPS PROMEDIO: %d \n", (fpsCount/ framesCount));
        std::cout << "game closing...\n";
        Game::Instance()->clean();
    }
    
    return 0;
}

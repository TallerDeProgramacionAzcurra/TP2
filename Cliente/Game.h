#ifndef GAME_H_
#define GAME_H_

#include "Player.h"
#include "Cliente/cliente.h"
#include "Utils/Parser/ParserCliente.h"
#include "Background/Island.h"
#include "Background/Background.h"
#include "Singletons/InputHandler.h"
#include "Singletons/TextureManager.h"
#include "Singletons/GameTimeHelper.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include "DrawObject.h"
#include <map>
#include <string>
#include <math.h>

using namespace std;
class Island;
class Background;
class Player;
class cliente;
class DrawObject;

#define TiMEOUT_MESSAGE_RATE 3000


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

    void paintbackground(int backgroundTextureID);

    bool canContinue();
    void checkContinueConditions();

    void resetGame();
    void requestTexturesInfo();
    void addTexture(TextureInfo textureInfo);
    void loadTextures();

    void createPlayer(int objectID, int textureID);
    void disconnectObject(int objectID, int layer);
    void disconnect();
    bool initializeClient();
    void askForName();
    bool conectToKorea();
   	void sendInputMsg(InputMessage mensaje);
   	void sendNetworkMsg(NetworkMessage netMsg);
   	void* koreaMethod(void);
   	void readFromKorea();
   	void interpretarDrawMsg(DrawMessage drwMsg);

   	bool updateTimeOut();

    SDL_Renderer* getRenderer() const { return m_pRenderer; }
    SDL_Window* getWindow() const { return m_pWindow; }
    void mrMusculo();
    void setRunning(bool loco){m_running = loco;}
    bool isRunning() { return m_running; }
    bool isReseting() { return m_reseting; }
    bool isInitializingSDL(){ return m_initializingSDL;}
    bool isWaitingForTextures() {return m_waitingTextures; }

    void quit() { m_running = false; }

    //Alto y Ancho de la ventana de juego
    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }
    float getScrollSpeed() { return m_scrollSpeed; }
    void setGameStarted(bool state) { m_gameStarted = state; }
    void setReseting(bool state) { m_reseting = state; }
    void setWindowSize(int width, int heigth);
    void setRestart(bool loco){m_restart = loco;}
    bool getRestart(){return m_restart;}
    int createGame(int DELAY_TIME);
    static void *thread_method(void *context);
    pthread_t listenThread;


private:

    //Layers
    std::map<int,DrawObject*> backgroundObjects;
    std::map<int,DrawObject*> middlegroundObjects;
    std::map<int,DrawObject*> foregroundObjects;

    void addDrawObject(int objectID, int layer, DrawObject* newDrawObject);
    void removeDrawObject(int objectID, int layer);
    void updateGameObject(const DrawMessage drawMessage);
    bool existDrawObject(int objectID, int layer);
    bool m_restart;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

   	int m_timeOutCounter;

    //Provisorio
    Player* m_player;
    Background* m_background;
    Island* m_island;
    cliente* m_client;
    int m_backgroundTextureID;
    bool m_running;

    bool m_gameStarted;
    bool m_reseting;
    bool m_initializingSDL;
    bool m_waitingTextures;
    bool m_continueLooping;

    void stopLooping();
    void continueLooping();

    std::string m_playerName;

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

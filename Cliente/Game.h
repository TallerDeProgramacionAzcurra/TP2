#ifndef GAME_H_
#define GAME_H_

#include "Player.h"
#include "Hud.h"
#include "Score.h"
#include "Cliente/cliente.h"
#include "Utils/Parser/ParserCliente.h"
#include "Background/Island.h"
#include "Background/Background.h"
#include "Singletons/InputHandler.h"
#include "Singletons/FontManager.h"
#include "Singletons/TextureManager.h"
#include "Singletons/GameTimeHelper.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include "DrawObject.h"
#include <map>
#include <string>
#include <pthread.h>
#include <math.h>
#include <memory>

using namespace std;
class Island;
class Background;
class Player;
class Hud;
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

    void paintbackground(int backgroundTextureID, int scrollSpeed);

    bool canContinue();
    void checkContinueConditions();

    void resetGame();
    void requestTexturesInfo();
    void addTexture(TextureInfo textureInfo);
    void loadTextures();
    void resetTextureColor(int objectID, int layer);

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

   	void updateBackground(BackgroundInfo backgroundInfo);

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

    void addPointsToScore(ScoreMessage scoreMsg);

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
    std::map<int, std::shared_ptr<DrawObject>> backgroundObjects;
    std::map<int, std::shared_ptr<DrawObject>> middlegroundObjects;
    std::map<int, std::shared_ptr<DrawObject>> foregroundObjects;

    void addDrawObject(int objectID, int layer, std::shared_ptr<DrawObject> newDrawObject);
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
    cliente* m_client;
    Hud* m_hud;
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

    int m_bgOff;

    Score m_playerScore;

	pthread_mutex_t  m_removeMutex;
	pthread_mutex_t  m_drawMsgMutex;
	pthread_mutex_t m_cleanMutex;
	pthread_mutex_t m_resetMutex;
	pthread_mutex_t m_scoreMutex;

    Game();
    ~Game();
    Game(const Game&);
	Game& operator=(const Game&);
};


#endif /* GAME_H_ */

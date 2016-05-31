#ifndef GAME_H_
#define GAME_H_


#include "Server/server.h"
#include "Server/DrawMessagesPacker.h"
#include "Utils/Parser/ParserServidor.h"
#include "Background/Level.h"

#include "Singletons/InputHandler.h"
#include "Singletons/TextureManager.h"

#include "Utils/TextureHelper.h"
#include "Utils/Parser/ParserNivel.h"


#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <pthread.h>

using namespace std;

class GameObject;
class Enemy;
class SmallEnemy;
class Player;
class Level;
class server;
class DrawMessagesPacker;
class BulletsHandler;
class CollitionHandler;

#define USE_DRAWMESSAGE_PACKAGING 0

#define DRAG_PLAYER 0

#define XML_PATH "test.xml"

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


    bool init(const char* title, int xpos, int ypos, int width, int height);

    //Funciones generales del ciclo de jueo
    void render();
    void update();
    void handleEvents();
    void clean();
    void resetGame();
    void refreshPlayersDirty();

    Vector2D getRandomPLayerCenter();

    bool createPlayer(int clientID, const std::string& playerName);
    bool validatePlayerName(const std::string& playerName);
    int  getFromNameID(const std::string& playerName);
    void disconnectPlayer(int playerId);
    void inicializarServer();
    void conectToKorea();
    void sendToAllClients(DrawMessage drawMsg);
    void addToPackage(DrawMessage drawMsg);
    void sendPackages();

    void addPointsToScore(int points, int playerID, int teamID);
    void addPointsToTeam(int points, int teamID);

    void initializeTexturesInfo();
    void setPlayersDirty();

   	void* koreaMethod(void);
   	void readFromKorea();
   	void keepListening();

   	void actualizarEstado(int id,InputMessage dataMsg);


    SDL_Renderer* getRenderer() const { return m_pRenderer; }
    SDL_Window* getWindow() const { return m_pWindow; }

    bool isRunning() { return m_running; }
    bool isResseting() { return m_reseting; }

    void quit() { m_running = false; }

    //Alto y Ancho de la ventana de juego
    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }
    void setReseting(bool state) { m_reseting = state; }

    pthread_t listenThread;
    float getScrollSpeed() { return m_scrollSpeed; }
    static void *thread_method(void *context);

private:

    std::map<int,Player*> m_listOfPlayer;
    std::map<int,std::string> m_playerNames;
    std::map<int, int> m_teamScores; //id de equipos por ahora son 0 y 1

    std::map<int,GameObject*> m_listOfGameObjects;

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    ParserNivel* m_parserNivel;
    Level* m_level;
    TextureHelper* m_textureHelper;

    Enemy* enemy;

    server* m_server;
    DrawMessagesPacker* m_drawMessagePacker;


    bool m_running;
    bool m_reseting;

    static Game* s_pInstance;

    int m_gameWidth;
    int m_gameHeight;
    float m_scrollSpeed;

    pthread_mutex_t m_resetMutex;
    pthread_mutex_t m_updatePlayerMutex;
    pthread_mutex_t m_createPlayerMutex;

    Game();
    ~Game();
    Game(const Game&);
	Game& operator=(const Game&);

	void initializeTeamScores();
};


#endif /* GAME_H_ */

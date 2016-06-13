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
#include "Utils/Parser/ParserStage.h"


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
class PowerUp;
class server;
class DrawMessagesPacker;
class PowerUpSpawner;
class EnemySpawner;
class BulletsHandler;
class CollitionHandler;

#define USE_DRAWMESSAGE_PACKAGING 0

#define DRAG_PLAYER 0

#define XML_PATH "test.xml"

enum GameMode
{
    GAMEMODE_COOPERATIVE,
    GAMEMODE_COMPETITION
};

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
    void sendScoreToClients(ScoreMessage scoreMsg);
   	void sendBackgroundInfo(BackgroundInfo backgroundInfo);

    void addPointsToScore(int points, int playerID, int teamID);
    void addPointsToTeam(int points, int teamID);

    void initializeTexturesInfo();
    void setPlayersDirty();

   	void* koreaMethod(void);
   	void readFromKorea();
   	void keepListening();

   	void actualizarEstado(int id,InputMessage dataMsg);

    void addPowerUp(PowerUp* powerUp);
    void addEnemy(Enemy* enemy);


    SDL_Renderer* getRenderer() const { return m_pRenderer; }
    SDL_Window* getWindow() const { return m_pWindow; }

    bool isRunning() { return m_running; }
    bool isResseting() { return m_reseting; }

    void quit() { m_running = false; }

    //Alto y Ancho de la ventana de juego
    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }
    Player* getPlayer(int playerID);
    int getPlayerTeam(int playerID);
    void setReseting(bool state) { m_reseting = state; }
    bool isPracticeMode();
	void setPracticeMode(bool practiceMode);

	void killAllEnemies(Player* killer);
	void loadCurrentStage();

    pthread_t listenThread;
    float getScrollSpeed() { return m_scrollSpeed; }
    static void *thread_method(void *context);

private:
    std::map<int, Player*> m_listOfPlayer;
    std::map<int,std::string> m_playerNames;
    std::map<int, int> m_teamScores; //id de equipos por ahora son 0 y 1

    std::map<int,GameObject*> m_listOfGameObjects;

    /***************Contenedores de Objetos del Stage********************/
    std::vector<PowerUp*> m_powerUps;
    std::vector<Enemy*> m_enemies;
    /*******************************************************************/

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    ParserNivel* m_parserNivel;
    ParserStage* m_parserStages;
    Level* m_level;
    TextureHelper* m_textureHelper;

    /*******Sacar********/
    PowerUp* powerUp;
    /***********************/

    server* m_server;
    DrawMessagesPacker* m_drawMessagePacker;

    /********Stages Info*********/
    int m_currentStage;

    PowerUpSpawner* m_powerUpsSpawner;
    EnemySpawner* m_enemiesSpawner;
    /****************************/

    GameMode m_currentMode;
    bool m_practiceMode;
    int m_practiceHoldTimer;


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

	void updateSpawners();
	void initializeTeamScores();
	void checkPracticeMode();

	void cleanDeadObjects();
};


#endif /* GAME_H_ */

#include "Game.h"

Game* Game::s_pInstance = 0;

Game::Game():
m_pWindow(0),
m_pRenderer(0),
 m_timeOutCounter(0),
 m_backgroundTextureID(10),
m_running(false),
m_gameStarted(false),
m_reseting(false),
m_initializingSDL(false),
m_waitingTextures(false),
m_continueLooping(false),
m_scrollSpeed(0.8),
m_gameWidth(0),
m_gameHeight(0)
{
	m_playerScore = Score();
	pthread_mutex_init(&m_removeMutex, NULL);
	pthread_mutex_init(&m_drawMsgMutex, NULL);
	pthread_mutex_init(&m_cleanMutex, NULL);
	pthread_mutex_init(&m_resetMutex, NULL);
	pthread_mutex_init(&m_scoreMutex, NULL);
}

Game::~Game()
{
    m_pRenderer = 0;
    m_pWindow = 0;
    pthread_mutex_destroy(&m_removeMutex);
    pthread_mutex_destroy(&m_drawMsgMutex);
    pthread_mutex_destroy(&m_cleanMutex);
    pthread_mutex_destroy(&m_resetMutex);
    pthread_mutex_destroy(&m_scoreMutex);
}


bool Game::init(const char* title, int xpos, int ypos, int width, int height, int SDL_WINDOW_flag)
{

    m_initializingSDL = true;

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "SDL init success\n";

        printf("%d\n",m_gameWidth);

        m_pWindow = SDL_CreateWindow("1942 - Cliente", 400, 150, m_gameWidth, m_gameHeight, SDL_WINDOW_RESIZABLE);

        if(m_pWindow != 0)
        {
            cout << "window creation success\n";
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);

            if(m_pRenderer != 0)
            {
                cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(m_pRenderer, 0,0,0,255);

                if (TTF_Init() == 0)
                {
                	cout << "ttf init success\n";
                }
                else
                {
                	cout << "ttf init fail\n";
                	return false;
                }
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

    printf("Finish Initializing\n");

    requestTexturesInfo(); // setea waiting textures en true

    //TextureManager::Instance()->init(m_pRenderer);
    FontManager::Instance()->init();

    m_hud = new Hud(m_gameWidth,m_gameHeight);

    m_backgroundTextureID = 10;


    //tudo ben
    m_initializingSDL = false;
    m_running = true;

    return true;
}

void Game::setWindowSize(int width, int height)
{
	m_gameWidth = width;
	m_gameHeight = height;
}

void Game::render()
{
    SDL_RenderClear(m_pRenderer);

    paintbackground(10);

    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = backgroundObjects.begin(); it != backgroundObjects.end(); ++it)
    {
    	if (it->second)
    		it->second->draw();

    }
    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = middlegroundObjects.begin(); it != middlegroundObjects.end(); ++it)
    {
    	if (it->second)
    		it->second->draw();
    }
    int foundOwnPlayer = -1;
    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = foregroundObjects.begin(); it != foregroundObjects.end(); ++it)
    {
    	if (it->second)
    	{
    		if (it->second->getObjectId() == m_player->getObjectId())
    		{
    			//al jugador propio lo renderiza al final para que quede adelante
    			foundOwnPlayer = it->second->getObjectId();
    			continue;
    		}
    		it->second->draw();
    	}
    }
    if (foundOwnPlayer != -1)
    {
    	foregroundObjects[foundOwnPlayer]->draw();
    }
    m_hud->draw(m_pRenderer);
    SDL_RenderPresent(m_pRenderer);
}

void Game::interpretarDrawMsg(DrawMessage drwMsg){


	/*printf("objectID: %d\n", drwMsg.objectID);
	printf("layer: %d\n", drwMsg.layer);
	printf("textureID: %d\n", drwMsg.textureID);
	printf("alive: %d\n", drwMsg.alive);*/
	if (m_initializingSDL || m_waitingTextures)
	{
		return;
	}

	 pthread_mutex_lock(&m_drawMsgMutex);

	if ( existDrawObject(drwMsg.objectID, static_cast<int>(drwMsg.layer)))
	{
		if (drwMsg.connectionStatus == false)
		{
			//printf("DrawMessage de objeto desconectado\n");
			disconnectObject(drwMsg.objectID, static_cast<int>(drwMsg.layer));
		}

		//Si existe y esta vivo lo actualia y sino lo quita del map
		if (drwMsg.alive)
		{
			updateGameObject(drwMsg);
		}
		else
		{
			//printf("Destruyendo objeto con id: %d \n", drwMsg.objectID);
			removeDrawObject(drwMsg.objectID, drwMsg.layer);
		}
	}
	else //Si no existe en el mapa
	{
		if (!drwMsg.alive)
		{
			pthread_mutex_unlock(&m_drawMsgMutex);
			return;
		}
		//printf("Creando nuevo objeto con objectID: %d y textura %d\n", drwMsg.objectID, drwMsg.textureID);

		std::shared_ptr<DrawObject> newObject = std::make_shared<DrawObject>();
		newObject->setObjectID(drwMsg.objectID);
		newObject->setLayer(static_cast<int>(drwMsg.layer));
		newObject->load(static_cast<int>(drwMsg.posX),static_cast<int>(drwMsg.posY),drwMsg.textureID);
		newObject->setCurrentRow(static_cast<int>(drwMsg.row));
		newObject->setCurrentFrame(static_cast<int>(drwMsg.column));
		newObject->setAlpha(static_cast<int>(drwMsg.alpha));
		newObject->setAngle(static_cast<double>(drwMsg.angle));

		addDrawObject(drwMsg.objectID, static_cast<int>(drwMsg.layer), newObject);
	}

	pthread_mutex_unlock(&m_drawMsgMutex);
}

void Game::addDrawObject(int objectID, int layer, std::shared_ptr<DrawObject> newDrawObject)
{
	switch(layer)
	{
	case BACKGROUND: backgroundObjects[objectID] = newDrawObject;
			break;
	case MIDDLEGROUND: middlegroundObjects[objectID] = newDrawObject;
			break;
	case FOREGROUND: foregroundObjects[objectID] = newDrawObject;
			break;

	default: middlegroundObjects[objectID] = newDrawObject;
	}
}

void Game::updateGameObject(const DrawMessage drawMessage)
{
	switch(drawMessage.layer)
	{
	case BACKGROUND:
			if ( backgroundObjects[drawMessage.objectID] != nullptr)
			{
				if (backgroundObjects[drawMessage.objectID]->getTextureId() != drawMessage.textureID)
				{
					backgroundObjects[drawMessage.objectID]->load(static_cast<int>(drawMessage.posX),static_cast<int>(drawMessage.posY),drawMessage.textureID);
				}
				else
				{
					backgroundObjects[drawMessage.objectID]->setPosition(Vector2D(drawMessage.posX,drawMessage.posY));
				}
				backgroundObjects[drawMessage.objectID]->setCurrentRow(static_cast<int>(drawMessage.row));
				backgroundObjects[drawMessage.objectID]->setCurrentFrame(static_cast<int>(drawMessage.column));
				backgroundObjects[drawMessage.objectID]->setAlpha(static_cast<int>(drawMessage.alpha));
				backgroundObjects[drawMessage.objectID]->setAngle(static_cast<double>(drawMessage.angle));
			}
			break;

	case MIDDLEGROUND:
			if ( middlegroundObjects[drawMessage.objectID] != nullptr)
			{
				if (middlegroundObjects[drawMessage.objectID]->getTextureId() != drawMessage.textureID)
				{
					middlegroundObjects[drawMessage.objectID]->load(static_cast<int>(drawMessage.posX),static_cast<int>(drawMessage.posY),drawMessage.textureID);
				}
				else
				{
					middlegroundObjects[drawMessage.objectID]->setPosition(Vector2D(drawMessage.posX,drawMessage.posY));
				}
				middlegroundObjects[drawMessage.objectID]->setCurrentRow(static_cast<int>(drawMessage.row));
				middlegroundObjects[drawMessage.objectID]->setCurrentFrame(static_cast<int>(drawMessage.column));
				middlegroundObjects[drawMessage.objectID]->setAlpha(static_cast<int>(drawMessage.alpha));
				middlegroundObjects[drawMessage.objectID]->setAngle(static_cast<double>(drawMessage.angle));
			}
			break;

	case FOREGROUND:
			if ( foregroundObjects[drawMessage.objectID] != nullptr)
			{
				if (foregroundObjects[drawMessage.objectID]->getTextureId() != drawMessage.textureID)
				{
					foregroundObjects[drawMessage.objectID]->load(static_cast<int>(drawMessage.posX),static_cast<int>(drawMessage.posY),drawMessage.textureID);
				}
				else
				{
					foregroundObjects[drawMessage.objectID]->setPosition(Vector2D(drawMessage.posX,drawMessage.posY));
				}
				foregroundObjects[drawMessage.objectID]->setCurrentRow(static_cast<int>(drawMessage.row));
				foregroundObjects[drawMessage.objectID]->setCurrentFrame(static_cast<int>(drawMessage.column));
				foregroundObjects[drawMessage.objectID]->setAlpha(static_cast<int>(drawMessage.alpha));
				foregroundObjects[drawMessage.objectID]->setAngle(static_cast<double>(drawMessage.angle));
			}
			break;
	}
}

void Game::removeDrawObject(int objectID, int layer)
{
	pthread_mutex_lock(&m_removeMutex);

	switch(layer)
	{
	case BACKGROUND:
			if (backgroundObjects[objectID] != nullptr)
			{
				backgroundObjects[objectID].reset();
				backgroundObjects.erase(objectID);
			}
			break;

	case MIDDLEGROUND:
		if (middlegroundObjects[objectID] != nullptr)
		{
			middlegroundObjects[objectID].reset();
			middlegroundObjects.erase(objectID);
		}
		break;

	case FOREGROUND:
		if (foregroundObjects[objectID] != nullptr)
		{
			foregroundObjects[objectID].reset();
			//printf ("%d \n", foregroundObjects[objectID].use_count());
			foregroundObjects.erase(objectID);
		}
		break;
	}

	pthread_mutex_unlock(&m_removeMutex);
}

bool Game::existDrawObject(int objectID, int layer)
{
	switch(layer)
	{
	case BACKGROUND:
	if (backgroundObjects.find(objectID) == backgroundObjects.end())
	{
		return false;
	}
	break;
	case MIDDLEGROUND:
	if (middlegroundObjects.find(objectID) == middlegroundObjects.end())
	{
		return false;
	}
	break;
	case FOREGROUND:
	if (foregroundObjects.find(objectID) == foregroundObjects.end())
	{
		return false;
	}
	break;

	default: return true;
	}
	return true;
}

void Game::update()
{
	m_hud->update();
	/*m_background->update(); //Provisorio
	m_island->update(); //Provisorio
	m_player->update(); // Provisorio*/
}

void Game::handleEvents()
{
	InputHandler::Instance()->update();
	//Pseudo controler
	if (m_player)
		m_player->handleInput();
}
bool Game::initializeClient()
{
		std::string	fileName = "Utils/Default/cliente.xml";

		ParserCliente* parsersito = new ParserCliente();
	    parsersito->parsearDocumento(fileName);

		LoggerInfo loggerInfo = parsersito->getLoggerInfo();
		Logger::Instance()->setLoglevel(loggerInfo.debugAvailable, loggerInfo.warningAvailable, loggerInfo.errorAvailable);

	    string ip = parsersito->getConexionInfo().ip;
	    int porto = parsersito->getConexionInfo().puerto;
	    printf("Conectando a %s : %d \n", ip.c_str(), porto);

	    m_client = new cliente(3,ip,porto, m_playerName);

	    if (parsersito)
	    	delete parsersito;

	    if (!conectToKorea())
	    	return false;

	    return true;

}

void Game::askForName()
{
    bool nombreValido = false;
    while (!nombreValido)
    {
		printf("Ingrese el nombre con el que desea conectarse \n");
		char name[24];
		cin.getline(name, 24);
		std::string playerName(name);
		if (playerName.length() <= 0)
		{
			printf("Nombre Invalido \n");
			nombreValido = false;
		}
		else
		{
		    m_playerName = playerName;
			nombreValido = true;
		}
    }
}

void Game::addPointsToScore(ScoreMessage scoreMsg)
{
	pthread_mutex_unlock(&m_scoreMutex);

	//todo Habria que contemplar equipos.
	//todo Por ahora hago que se envie a todos y que descarte si no es un score propio, para poder agregar lo otro despues

	if (static_cast<int>(scoreMsg.playerID) != m_player->getObjectId()) //posiblemente provisorio
	{
		pthread_mutex_unlock(&m_scoreMutex);
		return;
	}

	//actualiza el score del jugador
	m_playerScore.addPoints(static_cast<int>(scoreMsg.pointsacquire));
	//actualiza el hud
	m_hud->actualizarScore(m_playerScore.getScore());

	pthread_mutex_unlock(&m_scoreMutex);
}

void Game::paintbackground(int backgroundTextureID)
{
	int width = TextureManager::Instance()->getTextureInfo(backgroundTextureID).width;
	int height = TextureManager::Instance()->getTextureInfo(backgroundTextureID).height;
	int rowsAmount = ceil((float)m_gameWidth / (float) width);
	int columnsAmount = ceil((float)m_gameHeight / (float) height);
	for (int row = 0; row < rowsAmount; row++)
	{
		for (int column = 0; column < columnsAmount; column++)
		{
			int x = row * width;
			int y = column * height;
			TextureManager::Instance()->draw(backgroundTextureID, x, y, width, height, 0, m_pRenderer, SDL_FLIP_NONE);
		}
	}
}

void Game::createPlayer(int objectID, int textureID)
{
	m_player = new Player();
	m_player->setObjectID(objectID);
	m_player->setTextureID(textureID);
}

bool Game::canContinue()
{
	bool canContinue = true;
	if (!m_continueLooping || !m_gameStarted || m_reseting || m_initializingSDL || m_waitingTextures)
	{
		canContinue = false;
	}
	return canContinue;
}

void Game::checkContinueConditions()
{
	if (!m_continueLooping && !m_waitingTextures && !m_initializingSDL)
	{
		//printf("finish waiting\n");
		TextureManager::Instance()->clearTextureMap();
		loadTextures();
		m_continueLooping = true;
	}

}

void Game::stopLooping()
{
	m_continueLooping = false;
}
void Game::continueLooping()
{
	m_continueLooping = true;
}


void Game::disconnectObject(int objectID, int layer)
{
	//Armo color gris
	Uint8 r = 0xCC;
	Uint8 g = 0xCC;
	Uint8 b = 0xCC;

	switch(layer)
	{
	case BACKGROUND:
		TextureManager::Instance()->changeTextureColor(backgroundObjects[objectID]->getTextureId(), r, g, b);
		break;
	case MIDDLEGROUND:
		TextureManager::Instance()->changeTextureColor(middlegroundObjects[objectID]->getTextureId(), r, g, b);
		break;
	case FOREGROUND:
		TextureManager::Instance()->changeTextureColor(foregroundObjects[objectID]->getTextureId(), r, g, b);
		break;
	default:
		TextureManager::Instance()->changeTextureColor(foregroundObjects[objectID]->getTextureId(), r, g, b);
	}
}

void Game::resetTextureColor(int objectID, int layer)
{
	Uint8 r = 0xFF;
	Uint8 g = 0xFF;
	Uint8 b = 0xFF;

	switch(layer)
	{
	case BACKGROUND:
		TextureManager::Instance()->changeTextureColor(backgroundObjects[objectID]->getTextureId(), r, g, b);
		break;
	case MIDDLEGROUND:
		TextureManager::Instance()->changeTextureColor(middlegroundObjects[objectID]->getTextureId(), r, g, b);
		break;
	case FOREGROUND:
		TextureManager::Instance()->changeTextureColor(foregroundObjects[objectID]->getTextureId(), r, g, b);
		break;
	default:
		TextureManager::Instance()->changeTextureColor(foregroundObjects[objectID]->getTextureId(), r, g, b);
	}
}


void Game::disconnect()
{
	m_player->setControllable(false);

	//Armo color gris
	Uint8 r = 0xCC;
	Uint8 g = 0xCC;
	Uint8 b = 0xCC;

	//hardcodeado el layer del player
	TextureManager::Instance()->changeTextureColor(m_player->getTextureId(), r, g, b);

	m_running = false;
}

bool Game::conectToKorea()
{
	if (!m_client->conectar())
	{
		printf("No se pudo establecer conexión con el servidor.\n");
		return false;

	}
	else
	{
		readFromKorea();
	}
	return true;
}


void Game::sendInputMsg(InputMessage mensaje)
{
	m_client->sendInputMsg(mensaje);
}

void Game::sendNetworkMsg(NetworkMessage netMsg)
{
	m_client->sendNetworkMsg(netMsg);
}

void* Game::koreaMethod(void)
{
	std::cout << "Empece a ciclar bitches!\n";
	while (m_client->isConnected()) {
			m_client->leer();
	}
	 pthread_exit(NULL);
}

void *Game::thread_method(void *context)
{
	return ((Game *)context)->koreaMethod();
}
void Game::readFromKorea()
{
	pthread_create(&listenThread, NULL, &Game::thread_method, (void*)this);

}


bool Game::updateTimeOut()
{
	if (m_gameStarted)
	{
		 bool conectado= m_client->checkServerConnection();
		 if (!conectado)
			return false;
	}

	if (m_timeOutCounter >= TiMEOUT_MESSAGE_RATE)
	{
		NetworkMessage netMsg;
		netMsg.msg_Code[0] = 't';
		netMsg.msg_Code[1] = 'm';
		netMsg.msg_Code[2] = 'o';
		netMsg.msg_Length =  MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

		m_client->sendNetworkMsg(netMsg);
		//printf("Se envío Timeout Msg\n");
		m_timeOutCounter = 0;
	}
	else
	{
		m_timeOutCounter += GameTimeHelper::Instance()->deltaTime();
	}
	return true;
}

void Game::requestTexturesInfo()
{
	m_waitingTextures = true;
	m_continueLooping = false;
	NetworkMessage netMsg;
	netMsg.msg_Code[0] = 't';
	netMsg.msg_Code[1] = 'x';
	netMsg.msg_Code[2] = 'r';
	netMsg.msg_Length =  MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	m_client->sendNetworkMsg(netMsg);
}

void Game::addTexture(TextureInfo textureInfo)
{
	//printf("Se ha agregado la textura %d con path %s . Ultima = %d\n", textureInfo.textureID, textureInfo.path, textureInfo.lastTexture );
	TextureManager::Instance()->addTextureInfo(textureInfo);
	if (textureInfo.lastTexture)
	{
		m_waitingTextures = false;
		printf("Se agregaron todas las texturas\n");
	}
}

void Game::loadTextures()
{
	//printf("Se cargaron todas las texturas\n");
	Logger::Instance()->LOG("Cliente: Se recibieron y cargaron todas las texturas satisfactoriamente.", DEBUG);
	TextureManager::Instance()->loadTextures(m_pRenderer);
}
void Game::mrMusculo(){

		pthread_mutex_lock(&m_resetMutex);

	    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = backgroundObjects.begin(); it != backgroundObjects.end(); ++it)
	    {
	    	if (it->second)
	    	{
				it->second->clean();
				it->second.reset();
	    	}
	    }
	    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = middlegroundObjects.begin(); it != middlegroundObjects.end(); ++it)
	    {
	    	if (it->second)
	    	{
				it->second->clean();
				it->second.reset();
	    	}
	    }
	    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = foregroundObjects.begin(); it != foregroundObjects.end(); ++it)
	    {
	    	if (it->second)
	    	{
				it->second->clean();
				it->second.reset();
	    	}
	    }

	    TextureManager::Instance()->clearTextureMap();
	    backgroundObjects.clear();
	    middlegroundObjects.clear();
	    foregroundObjects.clear();
	 	InputHandler::Instance()->reset();
	 	m_playerScore.reset();

	    TTF_Quit();
	 	SDL_DestroyRenderer(m_pRenderer);
	    SDL_DestroyWindow(m_pWindow);
	    SDL_Quit();

		pthread_mutex_unlock(&m_resetMutex);
}
void Game::clean()
{
    cout << "cleaning game\n";
    pthread_mutex_lock(&m_cleanMutex);

    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = backgroundObjects.begin(); it != backgroundObjects.end(); ++it)
    {
    	if (it->second)
    	{
			it->second->clean();
			it->second.reset();
    	}
    }
    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = middlegroundObjects.begin(); it != middlegroundObjects.end(); ++it)
    {
    	if (it->second)
    	{
			it->second->clean();
			it->second.reset();
    	}
    }
    for (std::map<int, std::shared_ptr<DrawObject>>::iterator it = foregroundObjects.begin(); it != foregroundObjects.end(); ++it)
    {
    	if (it->second)
    	{
			it->second->clean();
			it->second.reset();
    	}
    }

    if (m_client)
    {
        m_client->desconectar();
        delete m_client;
    }
    if (m_player)
    	delete m_player;

    InputHandler::Instance()->clean();
    TextureManager::Instance()->clearTextureMap();
    backgroundObjects.clear();
    middlegroundObjects.clear();
    foregroundObjects.clear();

    TTF_Quit();
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();

    pthread_mutex_unlock(&m_cleanMutex);
}

void Game::resetGame()
{
//	m_reseting = true;
//	 cout << "reseting game\n";
//
//	 for (std::map<int,DrawObject*>::iterator it = backgroundObjects.begin(); it != backgroundObjects.end(); ++it)
//	 {
//		 cout << "destroying background\n";
//		it->second->clean();
//		delete it->second;
//	 }
//	 for (std::map<int,DrawObject*>::iterator it = middlegroundObjects.begin(); it != middlegroundObjects.end(); ++it)
//	 {
//		 cout << "destroying middleground\n";
//		it->second->clean();
//		delete it->second;
//	 }
//	 for (std::map<int,DrawObject*>::iterator it = foregroundObjects.begin(); it != foregroundObjects.end(); ++it)
//	 {
//		 cout << "destroying foreground\n";
//		it->second->clean();
//		delete it->second;
//	 }
//	 printf("GameObjects Destroyed");
//	 InputHandler::Instance()->reset();
//	 TextureManager::Instance()->clearTextureMap();
//	 backgroundObjects.clear();
//	 middlegroundObjects.clear();
//	 foregroundObjects.clear();


//	 SDL_DestroyWindow(m_pWindow);
	pthread_mutex_lock(&m_resetMutex);

	 printf("Se modificó el tamaño de la window\n");

	 setRunning(false);
	 setRestart(true);
	 m_playerScore.reset();
	 //TextureManager::Instance()->init(m_pRenderer);

	 cout << "Finish reseting game\n";
	 m_reseting = false;

	 pthread_mutex_unlock(&m_resetMutex);

}
int Game::createGame(int DELAY_TIME){
	//Armar un thread podria servir
	int frameStartTime, frameEndTime;

	std::cout << "Abriendo juego...\n";
	setRestart(false);
	if (Game::Instance()->init("1942 Ultraa Diesel", 400, 150, 800, 600, SDL_WINDOWPOS_CENTERED)) //flag por ejemplo: SDL_WINDOW_FULLSCREEN_DESKTOP
				{
					std::cout << "game init success!\n";

					std::cout << "Game Online!\n";

					//Bucle del juego
					while (Game::Instance()->isRunning()) {

						frameStartTime = SDL_GetTicks();

						if (!Game::Instance()->canContinue())
						{
							Game::Instance()->checkContinueConditions();
							continue;
						}

						Game::Instance()->updateTimeOut();

						Game::Instance()->handleEvents();

						Game::Instance()->update();

						Game::Instance()->render();

						frameEndTime = SDL_GetTicks() - frameStartTime;

						//tiempo a esperar = tiempo que demoro en finalizar el  frame = tiempo en que finalizó - tiempo en que inició
						if (frameEndTime < DELAY_TIME)
						{
							SDL_Delay((int) ((DELAY_TIME - frameEndTime)));
							GameTimeHelper::Instance()->updateDeltaTime(DELAY_TIME);
						}
						else
						{
							GameTimeHelper::Instance()->updateDeltaTime(frameEndTime);
						}
						//framesCount++;
						//fpsCount +=  1000/ frameEndTime;
						//printf("FPS: %d \n", (1000/ frameEndTime));

					}
				}
				else
				{
					std::cout << "game init failure - " << SDL_GetError() << "\n";
					return -1;
				}
}

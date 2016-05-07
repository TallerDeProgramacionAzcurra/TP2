#include "Game.h"

Game* Game::s_pInstance = 0;

Game::Game():
m_pWindow(0),
m_pRenderer(0),
 m_timeOutCounter(0),
m_running(false),
m_scrollSpeed(0.8)
{
	m_player = new Player();
}

Game::~Game()
{
    // we must clean up after ourselves to prevent memory leaks
    m_pRenderer= 0;
    m_pWindow = 0;
}


bool Game::init(const char* title, int xpos, int ypos, int width, int height, int SDL_WINDOW_flag)
{
	askForName();

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

    TextureManager::Instance()-> init();

    if (!setUpKorea())
    	return false;


    //tudo ben
    m_running = true;
    return true;
}

void Game::render()
{
    SDL_RenderClear(m_pRenderer);

    for (std::map<int,DrawObject*>::iterator it = backgroundObjects.begin(); it != backgroundObjects.end(); ++it)
    {
         it->second->draw();
    }
    for (std::map<int,DrawObject*>::iterator it = middlegroundObjects.begin(); it != middlegroundObjects.end(); ++it)
    {
         it->second->draw();
    }
    for (std::map<int,DrawObject*>::iterator it = foregroundObjects.begin(); it != foregroundObjects.end(); ++it)
    {
         it->second->draw();
    }

    SDL_RenderPresent(m_pRenderer);
}
void Game::interpretarDrawMsg(DrawMessage drwMsg){

	if ( existDrawObject(drwMsg.objectID, static_cast<int>(drwMsg.layer)))
	{
		if (drwMsg.connectionStatus == false)
		{
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
			return;

		//printf("Creando nuevo objeto con objectID: %d\n", drwMsg.objectID);

		DrawObject* newObject = new DrawObject();
		newObject->setObjectID(drwMsg.objectID);
		newObject->setLayer(static_cast<int>(drwMsg.layer));
		newObject->load(static_cast<int>(drwMsg.posX),static_cast<int>(drwMsg.posY),drwMsg.textureID);
		newObject->setCurrentRow(static_cast<int>(drwMsg.row));
		newObject->setCurrentFrame(static_cast<int>(drwMsg.column));
		addDrawObject(drwMsg.objectID, static_cast<int>(drwMsg.layer), newObject);
	}
	//PARA BORRAR listObjects.erase(id);
}

void Game::addDrawObject(int objectID, int layer, DrawObject* newDrawObject)
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
	case BACKGROUND: backgroundObjects[drawMessage.objectID]->setCurrentRow(static_cast<int>(drawMessage.row));
			backgroundObjects[drawMessage.objectID]->setCurrentFrame(static_cast<int>(drawMessage.column));
			backgroundObjects[drawMessage.objectID]->setPosition(Vector2D(drawMessage.posX,drawMessage.posY));
			break;

	case MIDDLEGROUND: middlegroundObjects[drawMessage.objectID]->setCurrentRow(static_cast<int>(drawMessage.row));
			middlegroundObjects[drawMessage.objectID]->setCurrentFrame(static_cast<int>(drawMessage.column));
			middlegroundObjects[drawMessage.objectID]->setPosition(Vector2D(drawMessage.posX,drawMessage.posY));
			break;

	case FOREGROUND: foregroundObjects[drawMessage.objectID]->setCurrentRow(static_cast<int>(drawMessage.row));
			foregroundObjects[drawMessage.objectID]->setCurrentFrame(static_cast<int>(drawMessage.column));
			foregroundObjects[drawMessage.objectID]->setPosition(Vector2D(drawMessage.posX,drawMessage.posY));
			break;

	default: middlegroundObjects[drawMessage.objectID]->setCurrentRow(static_cast<int>(drawMessage.row));
			middlegroundObjects[drawMessage.objectID]->setCurrentFrame(static_cast<int>(drawMessage.column));
			middlegroundObjects[drawMessage.objectID]->setPosition(Vector2D(drawMessage.posX,drawMessage.posY));
	}
}

void Game::removeDrawObject(int objectID, int layer)
{
	switch(layer)
	{
	case BACKGROUND:
			delete backgroundObjects[objectID];
			backgroundObjects.erase(objectID);
			break;

	case MIDDLEGROUND:
		delete middlegroundObjects[objectID];
		middlegroundObjects.erase(objectID);
		break;

	case FOREGROUND:
		delete foregroundObjects[objectID];
		foregroundObjects.erase(objectID);
		break;
	}
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
	/*m_background->update(); //Provisorio
	m_island->update(); //Provisorio
	m_player->update(); // Provisorio*/
}

void Game::handleEvents()
{
	InputHandler::Instance()->update();
	//Pseudo controler
	m_player->handleInput();
}
bool Game::setUpKorea()
{
		std::string	fileName = "Utils/Default/cliente.xml";

		ParserCliente* parsersito = new ParserCliente();
	    parsersito->parsearDocumento(fileName);

		LoggerInfo loggerInfo = parsersito->getLoggerInfo();
		Logger::Instance()->setLoglevel(loggerInfo.debugAvailable, loggerInfo.warningAvailable, loggerInfo.errorAvailable);

	    string ip = parsersito->getConexionInfo().ip;
	    int porto = parsersito->getConexionInfo().puerto;


	    m_client = new cliente(3,ip,porto, m_playerName);

	    if (!conectToKorea())
	    	return false;

	    return true;

}

void Game::askForName()
{
    pedirNombre:
	printf("Ingrese el nombre con el que desea conectarse \n");
    char name[24];
    cin.getline(name, 24);
    std::string playerName(name);
    if (playerName.length() <= 0)
    {
    	printf("Nombre Invalido \n");
    	goto pedirNombre;
    }
    m_playerName = playerName;
}

void Game::createPlayer(int objectID, int textureID)
{
	//m_player = new Player();
	m_player->setObjectID(objectID);
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


void Game::sendToKorea(InputMessage mensaje)
{
	m_client->sendInputMsg(mensaje);
}
void* Game::koreaMethod(void)
{
	std::cout << "Empece a ciclar bitches!\n";
	while (Game::Instance()->isRunning()) {
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
	if (m_client->checkServerConnection() == false)
		return false;

	if (m_timeOutCounter >= TiMEOUT_MESSAGE_RATE)
	{
		NetworkMessage netMsg;
		netMsg.msg_Code[0] = 't';
		netMsg.msg_Code[1] = 'm';
		netMsg.msg_Code[2] = 'o';
		m_client->sendNetworkMsg(netMsg);
		netMsg.msg_Length =  MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

		m_timeOutCounter = 0;
	}
	else
	{
		m_timeOutCounter += GameTimeHelper::Instance()->deltaTime();
	}
	return true;
}

void Game::clean()
{
    cout << "cleaning game\n";

    for (std::map<int,DrawObject*>::iterator it = backgroundObjects.begin(); it != backgroundObjects.end(); ++it)
    {
    	it->second->clean();
		delete it->second;
    }
    for (std::map<int,DrawObject*>::iterator it = middlegroundObjects.begin(); it != middlegroundObjects.end(); ++it)
    {
    	it->second->clean();
		delete it->second;
    }
    for (std::map<int,DrawObject*>::iterator it = foregroundObjects.begin(); it != foregroundObjects.end(); ++it)
    {
    	it->second->clean();
		delete it->second;
    }

    //delete m_player; //Provisorio

    InputHandler::Instance()->clean();
    TextureManager::Instance()->clearTextureMap();
    backgroundObjects.clear();
    middlegroundObjects.clear();
    foregroundObjects.clear();

    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

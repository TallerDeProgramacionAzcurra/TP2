/*
 * Level.cpp
 *
 *  Created on: May 9, 2016
 *      Author: gonzalo
 */

#include "Level.h"

Level::Level():m_scrollingToNextStage(false),
			   m_nextStageScrollAmount(600),
			   m_scrollSpeed(5)
{
	m_parserNivel = new ParserNivel();
	m_textureHelper = new TextureHelper();
}

Level::~Level()
{
}

void Level::loadFromXML(const std::string& xmlFileName)
{
	m_parserNivel->parsearDocumento(xmlFileName);
	Escenario scene = m_parserNivel->getEscenario();
	m_levelHeight = scene.alto - Game::Instance()->getGameHeight();
	m_levelWidth = scene.ancho;

	if (m_levelWidth < Game::Instance()->getGameWidth())
		m_levelWidth = Game::Instance()->getGameWidth();

	if (m_levelHeight < Game::Instance()->getGameHeight())
		m_levelHeight = Game::Instance()->getGameHeight();

	m_virtualPosition = m_levelHeight;

	m_backgroundTextureID = m_textureHelper->stringToInt(scene.fondo.spriteId);

	m_scrollSpeed = scene.velScroll;

	 std::stringstream ss;

	for (std::vector<Elemento>::iterator it = scene.listaDeElementos.begin() ; it !=  scene.listaDeElementos.end(); ++it)
	{
		//GameObjectFactory::createGameObject((*it).spriteId);
		printf("Cargando %s en posicion %d, %d \n",(*it).spriteId.c_str(), (*it).posicion.x, (*it).posicion.y );
		int textureID = m_textureHelper->stringToInt((*it).spriteId);

		TextureInfo texInfo = TextureManager::Instance()->getTextureInfo(textureID);
		//printf("texture int = %d \n", texInfo.textureID);
		GameObject* gameObject = GameObjectFactory::createGameObject((*it).spriteId);
		gameObject->setLayer(MIDDLEGROUND);
		gameObject->setHeight(static_cast<int>(texInfo.height));
		gameObject->setTextureID((texInfo.textureID));

		addObject(gameObject, (*it).posicion.x, (*it).posicion.y);

		 ss <<"Juego: Se cargó el objeto " << (*it).spriteId.c_str() << " en la posición x = " << (*it).posicion.x << ", y = " << (*it).posicion.y;
		 Logger::Instance()->LOG(ss.str(), LogTypeDebug);
		 ss.clear();
	}

	/*GameObject* gameObject = GameObjectFactory::createGameObject("isla");
	gameObject->setLayer(MIDDLEGROUND);
	gameObject->setTextureID(20);
	gameObject->setHeight(150);
	addObject(gameObject, 0, 0);*/

}

void Level::loadLevel(int level)
{
	 for (std::vector<GameObject*>::iterator it = m_levelObjects.begin() ; it != m_levelObjects.end();)
	 {
		 if ((*it))
		 {
			 (*it)->setObsolet(true);
		 }
		 if ((*it) && ((*it)->canRecycle()))
		 {
			 delete (*it);
			 it = m_levelObjects.erase(it);
		 }
		 else
		 {
			 ++it;
		 }
	 }
	std::stringstream ss;
	ss << "test" << level << ".xml";
	loadFromXML(ss.str());
	printf("Cargando el nivel %d de %s \n", level, ss.str().c_str());
}

void Level::scrollToNextStage()
{
	m_virtualPosition = m_levelHeight;
	m_scrollingToNextStage = true;
	m_nextStageScrollAmount = Game::Instance()->getGameHeight();
}

void Level::update()
{
	//printf("offset %d \n", m_virtualPosition);
	 if ((m_virtualPosition <= 0) && (!m_scrollingToNextStage) && (!Game::Instance()->isFinishingLevel()))
	 {
		 Game::Instance()->finishStage();
		 //resetPositions();
	 }

	 if (m_scrollingToNextStage)
	 {
		 //printf("%d\n", m_nextStageScrollAmount);
		 if (m_nextStageScrollAmount <= 0)
		 {
			 m_virtualPosition = m_levelHeight;
			 m_scrollingToNextStage = false;
			 m_virtualPosition = m_levelHeight + Game::Instance()->getGameHeight();
		 }
		 else
		 {
			 m_nextStageScrollAmount -= m_scrollSpeed;
		 }
	 }

	 for (std::vector<GameObject*>::iterator it = m_levelObjects.begin() ; it != m_levelObjects.end(); ++it)
	 {
		if ((Game::Instance()->isLevelStarted() && (!Game::Instance()->isFinishingLevel())) || (m_scrollingToNextStage))
		{
		 (*it)->scroll(m_scrollSpeed);
		}
		(*it)->update();
	 }

	 if ((Game::Instance()->isLevelStarted() && (!Game::Instance()->isFinishingLevel())) || (m_scrollingToNextStage))
	{
		m_virtualPosition -= m_scrollSpeed;
	}


}

void Level::addObject(GameObject* gameObject, int x, int y)
{
	Vector2D virtualPosition;
	virtualPosition.setX(x);
	//El cero esta en la pos alto de ventana e incrementa hacia arriba
	virtualPosition.setY(Game::Instance()->getGameHeight() - y);

	gameObject->setPosition(virtualPosition);
	m_levelObjects.push_back(gameObject);

}

void Level::resetPositions()
{
	 for (std::vector<GameObject*>::iterator it = m_levelObjects.begin() ; it != m_levelObjects.end(); ++it)
	 {
		 int newPosY =  (*it)->getPosition().getY() - m_levelHeight - Game::Instance()->getGameHeight();
		 int newPosX = (*it)->getPosition().getX();
		 (*it)->setPosition(Vector2D (newPosX, newPosY));
	 }
	 m_virtualPosition = m_levelHeight + Game::Instance()->getGameHeight();
}

/*void Level::paintBackground()
{
}*/

void Level::clean()
{
	 for (std::vector<GameObject*>::iterator it = m_levelObjects.begin() ; it != m_levelObjects.end(); ++it)
	 {
		 if ((*it))
			 delete (*it);
	 }
	 m_levelObjects.clear();


	 delete m_parserNivel;
	 delete m_textureHelper;
}



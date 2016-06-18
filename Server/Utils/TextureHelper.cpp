/*
 * TextureHelper.cpp
 *
 *  Created on: May 10, 2016
 *      Author: gonzalo
 */

#include "TextureHelper.h"


TextureHelper::TextureHelper()
{
	initialize();
}

TextureHelper::~TextureHelper()
{
	m_textureIDMap.clear();
}

void TextureHelper::initialize()
{
	m_textureIDMap["default"] = 999;

	//Escenario
	m_textureIDMap["agua"] = 10;

	//Jugadores
	m_textureIDMap["avion"] = 0;
	m_textureIDMap["player1"] = 0;
	m_textureIDMap["player2"] = 1;
	m_textureIDMap["player3"] = 2;
	m_textureIDMap["player4"] = 3;


	//Enemigos
	m_textureIDMap["AvionPequeño"] = 30;
	m_textureIDMap["AvionGrande"] = 31;
	m_textureIDMap["AvionFormacion"] = 32;
	m_textureIDMap["MediumPlane"] = 33;

	//Explosiones
	m_textureIDMap["ExplosionPequeña"] = 40;
	m_textureIDMap["ExplosionJugador"] = 41;

	//ELEMENTOS DE BACKGROUND
	m_textureIDMap["isla"] = 20;
	m_textureIDMap["piedra"] = 21;
	m_textureIDMap["Portaviones"] = 25;

	//Disparos
	m_textureIDMap["disparo"] = 50;
	m_textureIDMap["disparoRedondo"] = 51;
	m_textureIDMap["dobledisparo"] = 52;

	//PowerUps
	m_textureIDMap["ExtraPoints"] = 70;
	m_textureIDMap["BombPU"] = 71;
	m_textureIDMap["DualWeaponPU"] = 72;

	//Otros
	m_textureIDMap["MagicAppearance"] = 90;
	//Boss
	m_textureIDMap["bossMati"] = 666;

}

int TextureHelper::stringToInt(const std::string& stringID)
{
	if (m_textureIDMap.find(stringID) == m_textureIDMap.end())
		return 999;

	return m_textureIDMap[stringID];
}

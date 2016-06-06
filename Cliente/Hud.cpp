#include "Hud.h"

//using namespace std;

Hud::Hud()
{
	m_playerScore = 0;

	m_score->text = "0";
	m_score->height = 5;
	m_score->width = 5;
	m_score->texture = FontManager::Instance()->drawtext(1,1,1,0,0,0,0,0,m_score->text,blended);

	m_title->text = "SCORE";
	m_title->height = 5;
	m_title->width = 5;
	m_title->texture = FontManager::Instance()->drawtext(1,1,1,0,0,0,0,0,m_title->text,blended);
}

void Hud::actualizarScore(long int score)
{
	m_playerScore = score;
	std::string s = std::to_string(score);
	const char *pchar = s.c_str();
	m_score->text = pchar;
	m_score->texture = FontManager::Instance()->drawtext(1,1,1,0,0,0,0,0,m_title->text,blended);
}

void Hud::draw()
{

}

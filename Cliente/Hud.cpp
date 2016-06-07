#include "Hud.h"
using namespace std;

Hud::Hud(int gameWidth, int gameHeight)
{
	m_title.text = "SCORE";
	m_title.height = gameHeight/30;
	m_title.width = gameWidth/10;
	m_title.x = gameWidth/2-m_title.width;
	m_title.y = 0;
	m_title.texture = FontManager::Instance()->drawtext(206,220,9,0,0,0,0,0,m_title.text,blended);

	actualizarScore(0);
	m_score.height = gameHeight/30;
	m_score.width = gameWidth/10;
	m_score.x = gameWidth/2-m_score.width;
	m_score.y = m_title.height;
}

void Hud::actualizarScore(long int score)
{
	m_playerScore = score;
	ostringstream oss;
	oss << setfill('0') << setw(6) << score;
	string s = oss.str();
	const char *pchar = s.c_str();
	m_score.text = pchar;
	m_score.texture = FontManager::Instance()->drawtext(255,255,255,0,0,0,0,0,m_score.text,blended);
}

void Hud::draw(SDL_Renderer* renderer)
{
	SDL_Rect destRect;

	//Drawing title
	destRect.w = m_title.width;
	destRect.h = m_title.height;
	destRect.x = m_title.x;
	destRect.y = m_title.y;
	SDL_RenderCopy(renderer, m_title.texture, NULL, &destRect);

	//Drawing score
	destRect.w = m_score.width;
	destRect.h = m_score.height;
	destRect.x = m_score.x;
	destRect.y = m_score.y;
	SDL_RenderCopy(renderer, m_score.texture, NULL, &destRect);




}

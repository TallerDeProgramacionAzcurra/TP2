#include "Hud.h"
using namespace std;

Hud::Hud(int gameWidth, int gameHeight, int id, int cantHuds, bool teamMode): m_playerScore(0),
										 m_newScore(0),
										 m_lerping(false)
{
	const char *title;
	if (teamMode)
		title = "TEAM ";
	else
		title = "PLAYER ";
	stringstream ss;
	ss << title << id+1;
	string s = ss.str();
	const char *pchar = s.c_str();
	m_title.text = pchar;
	char r,g,b;
	getColorById(id,&r,&g,&b);
	m_title.texture = FontManager::Instance()->drawtext(r,g,b,0,0,0,0,0,m_title.text,blended);
	int h,w;
	FontManager::Instance()->textSize(m_title.text,&h,&w);
	m_title.height = h*TEXT_SIZE_FACTOR;
	m_title.width = w*TEXT_SIZE_FACTOR;
	m_title.x = (id)*(gameWidth/cantHuds) +((gameWidth/cantHuds) - m_title.width)/2;
	m_title.y = 0;


	actualizarScore(0);
	FontManager::Instance()->textSize(m_score.text,&h,&w);
	m_score.height = h*TEXT_SIZE_FACTOR;
	m_score.width = w*TEXT_SIZE_FACTOR;
	m_score.x = (id)*(gameWidth/cantHuds) +((gameWidth/cantHuds) - m_score.width)/2;
	m_score.y = m_title.height;

	updateScoreTexture(m_playerScore);
}

void Hud::getColorById(int id, char* r, char* g, char* b)
{
	switch (id)
	{
		case 1: //Red
			*r = 195;
			*g = 16;
			*b = 16;
			break;
		case 2: //Green
			*r = 17;
			*g = 202;
			*b = 17;
			break;
		case 3: //Purple
			*r = 186;
			*g = 0;
			*b = 215;
			break;
		case 4: //Dark Blue
			*r = 10;
			*g = 10;
			*b = 110;
			break;
		case 5: //Orange
			*r = 255;
			*g = 120;
			*b = 20;
			break;
		default: //Yellow
			*r = 206;
			*g = 220;
			*b = 9;
	}
}

void Hud::actualizarScore(int score)
{
	m_newScore = (int) score;
	m_lerping = true;
}

void Hud::updateScoreTexture(int score)
{
	ostringstream oss;
	oss << setfill('0') << setw(6) << score;
	string s = oss.str();
	const char *pchar = s.c_str();
	m_score.text = pchar;
	m_score.texture = FontManager::Instance()->drawtext(255,255,255,0,0,0,0,0, m_score.text, blended);
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

void Hud::update()
{
	if (m_playerScore != m_newScore)
	{
		if (m_lerping)
		{
			float difference = m_newScore - m_playerScore;
			m_playerScore = m_playerScore + ((difference) * LERP_WEIGHT);

			if (difference <= 5.0f)
			{
				m_playerScore = m_newScore;
				m_lerping = false;
			}
			updateScoreTexture((int) m_playerScore);
		}
		else
		{
			//Paso algo extraño y no esta lerpeando pero el score esta desactualizado, actualiza en forma forzada
			m_playerScore = m_newScore;
			updateScoreTexture((int) m_playerScore);
		}
	}
}

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

	int h,w;

	m_title.text = pchar;
	m_title.texture = FontManager::Instance()->drawtext(206,220,9,0,0,0,0,0,m_title.text,blended);
	FontManager::Instance()->textSize(m_title.text,&h,&w);
	m_title.height = h*TEXT_SIZE_FACTOR;
	m_title.width = w*TEXT_SIZE_FACTOR;
	m_title.x = (id+1)*(gameWidth-m_title.width)/(cantHuds+1);
	m_title.y = 0;


	actualizarScore(0);
	FontManager::Instance()->textSize(m_score.text,&h,&w);
	m_score.height = h*TEXT_SIZE_FACTOR;
	m_score.width = w*TEXT_SIZE_FACTOR;
	m_score.x = (id+1)*(gameWidth-m_score.width)/(cantHuds+1);
	m_score.y = m_title.height;

	updateScoreTexture(m_playerScore);
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

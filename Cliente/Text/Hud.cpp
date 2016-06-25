#include "Hud.h"
using namespace std;

Hud::~Hud(){

}

Hud::Hud(int gameWidth, int gameHeight, int id, int cantHuds, bool teamMode): m_playerScore(0),
										 m_newScore(0),
										 m_lerping(false)
{
	FontTexture *title = new FontTexture();
	const char *titleText;
	if (teamMode)
		titleText = "TEAM ";
	else
		titleText = "PLAYER ";
	stringstream ss;
	ss << titleText << id+1;
	string s = ss.str();
	const char *pchar = s.c_str();
	title->text = pchar;
	char r,g,b;
	getColorById(id,&r,&g,&b);
	title->texture = FontManager::Instance()->drawtext(r,g,b,0,0,0,0,0,title->text,blended);
	int h,w;
	FontManager::Instance()->textSize(title->text,&h,&w);

	title->height = h*TEXT_SIZE_FACTOR;
	title->width = w*TEXT_SIZE_FACTOR;
	title->x = (id)*(gameWidth/cantHuds) +((gameWidth/cantHuds) - title->width)/2;
	title->y = 0;

	m_texts.push_back(title);

	updateScoreTexture(m_playerScore);
	FontManager::Instance()->textSize(m_score.text,&h,&w);
	m_score.height = h*TEXT_SIZE_FACTOR;
	m_score.width = w*TEXT_SIZE_FACTOR;
	m_score.x = (id)*(gameWidth/cantHuds) +((gameWidth/cantHuds) - m_score.width)/2;
	m_score.y = title->height;

	m_texts.push_back(&m_score);
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

void Hud::reset()
{
	actualizarScore(0);
}

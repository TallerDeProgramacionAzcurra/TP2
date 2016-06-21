#include "Results.h"
using namespace std;

Results::Results(int gameWidth, int gameHeight, FinishGameInfo info)
{
    int h,w;
    FontTexture *title = new FontTexture();
    const char *pchar;
    if (info.isVictory)
	{
		stringstream ss;
		ss << "TEAM " << info.winnerID + 1 << " WINS";
		string s = ss.str();
		pchar = s.c_str();
	}
	else
	{
		stringstream ss;
		ss << "HIGHSCORE: PLAYER " << info.winnerID + 1;
		string s = ss.str();
		pchar = s.c_str();
	}
    title->text = pchar;
    title->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0,title->text,blended);
    FontManager::Instance()->textSize(title->text,&h,&w);
    title->height = h*TEXT_SIZE_FACTOR;
    title->width = w*TEXT_SIZE_FACTOR;
    title->x = (gameWidth-title->width)/2;
    title->y = (gameHeight-title->height)/2;
    m_texts.push_back(title);

    FontTexture *points = new FontTexture();
    createFontTexture(info.points,"POINTS: ",points);
    FontManager::Instance()->textSize(points->text,&h,&w);
    points->height = h*TEXT_SIZE_FACTOR;
    points->width = w*TEXT_SIZE_FACTOR;
    points->x = (gameWidth-points->width)/2;
    points->y = title->y + title->height;
    m_texts.push_back(points);
}

Results::~Results()
{

}

void Results::createFontTexture(short value, const char* text, FontTexture* font)
{
    stringstream ss;
    ss << text << value;
    string s = ss.str();
    const char *pchar = s.c_str();
    font->text = pchar;
    font->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0, font->text, blended);
}

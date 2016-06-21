#include "Statistics.h"
using namespace std;

Statistics::Statistics(int gameWidth, int gameHeight, StageStatistics stats)
{
    int h,w;
    
    m_title.text = "STATS";
    m_title.texture = FontManager::Instance()->drawtext(145,17,17,0,0,0,0,0,m_title.text,blended);
    
    FontManager::Instance()->textSize(m_title.text,&h,&w);
    
    m_title.height = h*TEXT_SIZE_FACTOR;
    m_title.width = w*TEXT_SIZE_FACTOR;
    m_title.x = (gameWidth-m_title.width)/2;
    m_title.y = 1*gameHeight/4;
    
    createFontTexture(stats.points,"Points: ",&m_points);
    FontManager::Instance()->textSize(m_points.text,&h,&w);
    m_points.height = h*TEXT_SIZE_FACTOR;
    m_points.width = w*TEXT_SIZE_FACTOR;
    m_points.x = (gameWidth-m_points.width)/2;
    m_points.y = m_title.y + m_title.height;
    
    createFontTexture(stats.enemiesKilled,"Enemies Killed: ",&m_enemiesKilled);
    FontManager::Instance()->textSize(m_enemiesKilled.text,&h,&w);
    m_enemiesKilled.height = h*TEXT_SIZE_FACTOR;
    m_enemiesKilled.width = w*TEXT_SIZE_FACTOR;
    m_enemiesKilled.x = (gameWidth-m_enemiesKilled.width)/2;
    m_enemiesKilled.y = m_points.y + m_points.height;
    
    createFontTexture(stats.accuracy,"Accuracy: ",&m_accuracy);
    FontManager::Instance()->textSize(m_accuracy.text,&h,&w);
    m_accuracy.height = h*TEXT_SIZE_FACTOR;
    m_accuracy.width = w*TEXT_SIZE_FACTOR;
    m_accuracy.x = (gameWidth-m_accuracy.width)/2;
    m_accuracy.y = m_enemiesKilled.y + m_enemiesKilled.height;
}

Statistics::~Statistics()
{
    //SDL_DestroyTexture(m_title.texture);
    //SDL_DestroyTexture(m_points.texture);
    //SDL_DestroyTexture(m_enemiesKilled.texture);
    //SDL_DestroyTexture(m_accuracy.texture);
}


void Statistics::createFontTexture(short value, const char* text, FontTexture* font)
{
    stringstream ss;
    ss << text << value;
    string s = ss.str();
    const char *pchar = s.c_str();
    font->text = pchar;
    font->texture = FontManager::Instance()->drawtext(190,100,25,0,0,0,0,0, font->text, blended);
}

void Statistics::draw(SDL_Renderer* renderer)
{
    SDL_Rect destRect;
    
    //Drawing title
    destRect.w = m_title.width;
    destRect.h = m_title.height;
    destRect.x = m_title.x;
    destRect.y = m_title.y;
    SDL_RenderCopy(renderer, m_title.texture, NULL, &destRect);
    
    //Drawing points
    destRect.w = m_points.width;
    destRect.h = m_points.height;
    destRect.x = m_points.x;
    destRect.y = m_points.y;
    SDL_RenderCopy(renderer, m_points.texture, NULL, &destRect);
    
    //Drawing enemiesKilled
    destRect.w = m_enemiesKilled.width;
    destRect.h = m_enemiesKilled.height;
    destRect.x = m_enemiesKilled.x;
    destRect.y = m_enemiesKilled.y;
    SDL_RenderCopy(renderer, m_enemiesKilled.texture, NULL, &destRect);
    
    //Drawing accuracy
    destRect.w = m_accuracy.width;
    destRect.h = m_accuracy.height;
    destRect.x = m_accuracy.x;
    destRect.y = m_accuracy.y;
    SDL_RenderCopy(renderer, m_accuracy.texture, NULL, &destRect);
}

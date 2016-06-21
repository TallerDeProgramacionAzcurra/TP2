#ifndef STATISTICS_H_
 #define STATISTICS_H_

 #include "Utils/TiposDefinidos.h"
 #include "Singletons/FontManager.h"
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_ttf.h>
 #include <sstream>
 #include <string>
 #include <iomanip>

 #define TEXT_SIZE_FACTOR 1.5f

 class Statistics
 {
 public:

 	//constructor sin argumento: setea m_controllable siempre en true
     Statistics(int gameWidth,int gameHeight,StageStatistics stats);
     ~Statistics();
     void draw(SDL_Renderer* renderer);

 private:


     FontTexture m_title;
     FontTexture m_points;
     FontTexture m_enemiesKilled;
     FontTexture m_accuracy;

     void createFontTexture(short value, const char* text, FontTexture* font);


 };


 #endif /* STATISTICS_H_ */

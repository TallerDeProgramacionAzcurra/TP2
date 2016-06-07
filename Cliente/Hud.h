#ifndef HUD_H_
#define HUD_H_

#include "Utils/TiposDefinidos.h"
#include "Singletons/FontManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <string>
#include <iomanip>

#define TEXT_SIZE_FACTOR 1

class Hud
{
public:

	//constructor sin argumento: setea m_controllable siempre en true
    Hud(int gameWidth,int gameHeight);
    ~Hud();
    void actualizarScore(long int score);
    void draw(SDL_Renderer* renderer);

private:

    FontTexture m_score;
    FontTexture m_title;
    long int m_playerScore;

};


#endif /* HUD_H_ */

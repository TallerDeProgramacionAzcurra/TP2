#ifndef HUD_H_
#define HUD_H_

#include "Utils/TiposDefinidos.h"
#include "Singletons/FontManager.h"
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <sstream>
#include <string>
#include <iomanip>

#define TEXT_SIZE_FACTOR 1.5f
#define LERP_WEIGHT 0.1f

class Hud
{
public:

	Hud(int gameWidth,int gameHeight, int id, int cantHuds, bool teamMode);
    ~Hud();
    void actualizarScore(int score);
    void draw(SDL_Renderer* renderer);
    void update();

private:

    FontTexture m_score;
    FontTexture m_title;
    float m_playerScore;

    float m_newScore;
    bool m_lerping;

    void updateScoreTexture(int score);
    void getColorById(int id, char* r, char* g, char* b);

};


#endif /* HUD_H_ */

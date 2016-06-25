#ifndef HUD_H_
#define HUD_H_

#include "ScreenText.h"
#include "../Utils/TiposDefinidos.h"
#include "../Singletons/FontManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>
#include <sstream>
#include <string>
#include <iomanip>

#define TEXT_SIZE_FACTOR 1.5f
#define LERP_WEIGHT 0.1f

class Hud: public ScreenText
{
public:

	Hud(int gameWidth,int gameHeight, int id, int cantHuds, bool teamMode);
    ~Hud();
    void actualizarScore(int score);
    //void draw(SDL_Renderer* renderer);
    void update();
    void reset();

private:


    //FontTexture m_title;
    FontTexture m_score;
    float m_playerScore;

    float m_newScore;
    bool m_lerping;

    void updateScoreTexture(int score);
    void getColorById(int id, char* r, char* g, char* b);

};


#endif /* HUD_H_ */

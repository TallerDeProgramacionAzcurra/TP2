#ifndef HUD_H_
#define HUD_H_

#include "Utils/TiposDefinidos.h"
#include "Singletons/FontManager.h"

class Hud
{
public:

	//constructor sin argumento: setea m_controllable siempre en true
    Hud();
    void actualizarScore(long int score);
    void draw();

private:
    FontTexture* m_score;
    FontTexture* m_title;
    long int m_playerScore;

};


#endif /* HUD_H_ */

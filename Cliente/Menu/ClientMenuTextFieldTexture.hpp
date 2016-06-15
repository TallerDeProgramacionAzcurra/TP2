//
//  ClientMenuTextFieldTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTextFieldTexture_hpp
#define ClientMenuTextFieldTexture_hpp

#include <stdio.h>
#include <time.h>

#include "ClientMenuTextTexture.hpp"

class ClientMenuTextFieldTexture: public ClientMenuTextTexture {
public:
    ClientMenuTextFieldTexture(SDL_Window *menuWindow);
    virtual ~ClientMenuTextFieldTexture();
    
    void menuTextureRender(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void menuTextureTextFieldHandlerMourEvent(SDL_Event *mouseEvent);
    
private:
    clock_t startTime;
    bool drawLine;
    
    bool menuTextureTextFieldEventInside(SDL_Event *mouseEvent);
};

#endif /* ClientMenuTextFieldTexture_hpp */

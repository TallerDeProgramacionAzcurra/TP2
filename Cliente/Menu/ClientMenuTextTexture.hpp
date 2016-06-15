//
//  ClientMenuTextTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTextTexture_hpp
#define ClientMenuTextTexture_hpp

#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>

#include "ClientMenuTexture.hpp"

class ClientMenuTextTexture : public ClientMenuTexture {
public:
    ClientMenuTextTexture(SDL_Window *menuWindow);
    virtual ~ClientMenuTextTexture();
    
    //Loads image at specified path
    void menuTextureSetTextFont(const char *filePath, int fontSize);
    void menuTextureSetText(const char *textureText);
    
protected:
    bool menuTextureSelected;
    
private:
    TTF_Font *menuTextureFont;
};

#endif /* ClientMenuTextTexture_hpp */

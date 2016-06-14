//
//  ClientMenuTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTexture_hpp
#define ClientMenuTexture_hpp

#include <SDL2/SDL.h>
#include <stdio.h>

class ClientMenuTexture {
public:
    ClientMenuTexture(SDL_Renderer *menuTextureRenderer);
    virtual ~ClientMenuTexture();
    
    //Renders texture at given point.
    void menuTextureRender( int x, int y );
    
    //Gets image dimensions.
    int menuTextureGetWidth();
    int menuTextureGetHeight();
    
    SDL_Texture *menuTextureGetTexture();
    
protected:
    //Image dimensions.
    int menuTextureWidth;
    int menuTextureHeight;
    
    //The actual hardware texture.
    SDL_Texture *menuTexture;
    SDL_Renderer *menuTextureRenderer;
};

#endif /* ClientMenuTexture_hpp */

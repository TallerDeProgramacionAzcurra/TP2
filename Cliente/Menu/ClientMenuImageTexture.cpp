//
//  ClientMenuImageTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <SDL2_image/SDL_image.h>

#include "ClientMenuImageTexture.hpp"

ClientMenuImageTexture::ClientMenuImageTexture(SDL_Renderer *menuTextureRenderer)  : ClientMenuTexture(menuTextureRenderer) {
    
}

ClientMenuImageTexture::~ClientMenuImageTexture() {
    
}

bool ClientMenuImageTexture::menuTextureLoadFromFile(const char *filePath) {
    //The final texture
    SDL_Texture *newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(filePath);
    
    if (loadedSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", filePath, IMG_GetError());
        return false;
    } else {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF ));
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(this->menuTextureRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf( "Unable to create texture from %s! SDL Error: %s\n", filePath, SDL_GetError());
            SDL_FreeSurface(loadedSurface);
            return false;
        } else {
            //Get image dimensions
            this->menuTextureWidth = loadedSurface->w;
            this->menuTextureHeight = loadedSurface->h;
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    
    //Return success
    this->menuTexture = newTexture;
    
    return this->menuTexture != NULL;
}

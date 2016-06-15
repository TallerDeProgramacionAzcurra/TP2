//
//  ClientMenuTextTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuTextTexture.hpp"

ClientMenuTextTexture::ClientMenuTextTexture(SDL_Window *menuWindow) : ClientMenuTexture(menuWindow) {
    if (TTF_Init() == -1) {
        printf("ClientMenuTextTexture.cpp - SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    }
}

ClientMenuTextTexture::~ClientMenuTextTexture() {
    if (this->menuTextureFont != NULL) {
        TTF_CloseFont(this->menuTextureFont);
    }
    
    if (this->menuTexture != NULL) {
        SDL_DestroyTexture(this->menuTexture);
    }
    
    TTF_Quit();
}

//Loads image at specified path
void ClientMenuTextTexture::menuTextureLoadFromFile(const char *filePath) {
    this->menuTextureFont = TTF_OpenFont(filePath, 28);
    this->menuTextureLoaded = true;
    
    if (this->menuTextureFont == NULL) {
        printf("ClientMenuTextTexture.cpp - Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    }
}

void ClientMenuTextTexture::menuTextureSetTextProperties(const char *textureText, SDL_Color textColor) {
    //Render text surface
    SDL_Surface *textSurface = TTF_RenderText_Solid(this->menuTextureFont, textureText, textColor);
    if (textSurface == NULL) {
        printf("ClientMenuTextTexture.cpp - Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    } else {
        //Create texture from surface pixels
        SDL_DestroyTexture(this->menuTexture);
        this->menuTexture = SDL_CreateTextureFromSurface(this->menuTextureRenderer, textSurface);
        
        if (this->menuTexture == NULL) {
            printf("ClientMenuTextTexture.cpp - Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            this->menuTextureLoaded = false;
        } else {
            //Get image dimensions
            this->menuTextureWidth = textSurface->w;
            this->menuTextureHeight = textSurface->h;
        }
        
        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    
    //Return success
    this->menuTextureLoaded =  this->menuTexture != NULL;
}

//
//  ClientMenuTextTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuTextTexture.hpp"
#include "ClientMenuUtil.hpp"

ClientMenuTextTexture::ClientMenuTextTexture(SDL_Window *menuWindow) : ClientMenuTexture(menuWindow) {
    this->menuTextureLoaded = true;
    this->menuTextureSelected = false;
    this->menuTextureFont = NULL;
    
    if (TTF_Init() == -1) {
        printf("ClientMenuTextTexture.cpp - SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    }
}

ClientMenuTextTexture::~ClientMenuTextTexture() {
    TTF_CloseFont(this->menuTextureFont);
    
    SDL_DestroyTexture(this->menuTexture);
    
    TTF_Quit();
}

//Loads image at specified path
void ClientMenuTextTexture::menuTextureSetTextFont(const char *filePath, int fontSize) {
    TTF_CloseFont(this->menuTextureFont);
    this->menuTextureFont = TTF_OpenFont(filePath, fontSize);
    this->menuTextureLoaded = true;
    
    if (this->menuTextureFont == NULL) {
        printf("ClientMenuTextTexture.cpp - Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    }
}

void ClientMenuTextTexture::menuTextureSetText(const char *textureText) {
    //Render text surface
    SDL_Color textColor = this->menuTextureSelected? ClientMenuUtils::clientMenuTextColorSelected() : ClientMenuUtils::clientMenuTextColor();
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

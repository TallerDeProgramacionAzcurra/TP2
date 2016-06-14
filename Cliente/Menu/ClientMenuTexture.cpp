//
//  ClientMenuTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <SDL2_image/SDL_image.h>

#include "ClientMenuTexture.hpp"

ClientMenuTexture::ClientMenuTexture(SDL_Renderer *menuTextureRenderer) {
    //Initialize
    this->menuTexture = NULL;
    this->menuTextureRenderer = menuTextureRenderer;
    this->menuTextureWidth = 0;
    this->menuTextureHeight = 0;
}

ClientMenuTexture::~ClientMenuTexture() {
    if (this->menuTexture != NULL) {
        SDL_DestroyTexture(this->menuTexture);
    }
}

void ClientMenuTexture::menuTextureRender(int x, int y) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, this->menuTextureWidth, this->menuTextureHeight };
    SDL_RenderCopy(this->menuTextureRenderer, this->menuTexture, NULL, &renderQuad );
}

int ClientMenuTexture::menuTextureGetWidth()
{
    return this->menuTextureWidth;
}

int ClientMenuTexture::menuTextureGetHeight()
{
    return this->menuTextureHeight;
}

SDL_Texture *ClientMenuTexture::menuTextureGetTexture() {
    return this->menuTexture;
}
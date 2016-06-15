//
//  ClientMenuTextFieldTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuTextFieldTexture.hpp"
#include "ClientMenuUtil.hpp"

ClientMenuTextFieldTexture::ClientMenuTextFieldTexture(SDL_Window *menuWindow) : ClientMenuTextTexture(menuWindow) {
    this->startTime = clock();
    this->drawLine = true;
}

ClientMenuTextFieldTexture::~ClientMenuTextFieldTexture() {
}

void ClientMenuTextFieldTexture::menuTextureRender(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    clock_t endTime = clock();
    clock_t clockTicksTaken = endTime - this->startTime;
    double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
    
    if (timeInSeconds > 0.5) {
        this->startTime = endTime;
        this->drawLine = !this->drawLine;
    }
    
    if (this->menuTextureSelected == true) {
        if (this->drawLine == true) {
            // Draw the text.
            ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
            
            //Draw blue horizontal line
            SDL_Color textColor =  ClientMenuUtils::clientMenuTextColorSelected();
            SDL_SetRenderDrawColor(this->menuTextureRenderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_RenderDrawLine(this->menuTextureRenderer, x, y + this->menuTextureHeight, x + this->menuTextureWidth, y + this->menuTextureHeight);
        }
    } else {
        ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
        
        if (this->drawLine == true) {
            //Draw blue horizontal line
            SDL_Color textColor =  ClientMenuUtils::clientMenuTextColor();
            
            SDL_SetRenderDrawColor(this->menuTextureRenderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_RenderDrawLine(this->menuTextureRenderer, x, y + this->menuTextureHeight, x + this->menuTextureWidth, y + this->menuTextureHeight);
        }
    }
}

bool ClientMenuTextFieldTexture::menuTextureTextFieldEventInside(SDL_Event *mouseEvent) {
    int mouseEventX = mouseEvent->motion.x;
    int mouseEventY = mouseEvent->motion.y;
    
    int textureMinX = this->menuTextureX;
    int textureMaxX = this->menuTextureX + this->menuTextureWidth;
    
    int textureMinY = this->menuTextureY;
    int textureMaxY = this->menuTextureY + this->menuTextureHeight;
    
    if (mouseEventX > textureMinX && mouseEventX < textureMaxX &&
        mouseEventY > textureMinY && mouseEventY < textureMaxY) {
        return true;
    }
    
    return false;
}

void ClientMenuTextFieldTexture::menuTextureTextFieldHandlerMourEvent(SDL_Event *mouseEvent) {
    bool mouseEventInside = this->menuTextureTextFieldEventInside(mouseEvent);
    
    if (mouseEventInside == true && this->menuTextureSelected == false) {
        this->menuTextureSelected = true;
    } else {
        this->menuTextureSelected = false;
    }
}

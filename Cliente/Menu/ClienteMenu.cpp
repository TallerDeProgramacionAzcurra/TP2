//
//  ClienteMenu.cpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuTextFieldTexture.hpp"
#include "ClientMenuImageTexture.hpp"
#include "ClientMenuTextTexture.hpp"
#include "ClientMenuTexture.hpp"
#include "ClientMenuUtil.hpp"
#include "ClienteMenu.hpp"

// Constructor and Destructor.
ClientMenu::ClientMenu(const char *menuTitle, const int menuWidth, const int menuHeight) {
    // The window we'll be rendering to.
    this->clientMenuWindow = NULL;
    
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ClienteMenu.cpp - SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        //Create window.
        this->clientMenuWindow = SDL_CreateWindow(menuTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, menuWidth, menuHeight, SDL_WINDOW_SHOWN);
        
        if (this->clientMenuWindow == NULL) {
            printf("ClienteMenu.cpp - SDL Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            //Create renderer for window
            this->clientMenuRender = SDL_CreateRenderer(this->clientMenuWindow, -1, SDL_RENDERER_ACCELERATED);
            if (this->clientMenuRender == NULL) {
                printf("ClienteMenu.cpp - Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            }
        }
    }
}

ClientMenu::~ClientMenu() {
    // Free screen surface.
    SDL_Surface *screenSurface = SDL_GetWindowSurface(this->clientMenuWindow);
    SDL_FreeSurface(screenSurface);
    
    //Destroy window
    SDL_DestroyRenderer(this->clientMenuRender);
    SDL_DestroyWindow(this->clientMenuWindow);
    
    //Quit SDL subsystems
    SDL_Quit();
}

// Render functions.
void ClientMenu::clientMenuClear() {
    // Clear screen.
    SDL_SetRenderDrawColor(this->clientMenuRender, 255, 255, 255, 255);
    SDL_RenderClear(this->clientMenuRender);
}

void ClientMenu::clientMenuShow() {    
    // Update screen.
    SDL_RenderPresent(this->clientMenuRender);
}

// Events methods.
bool ClientMenu::clientMenuHandleQuitEvent() {
    //Event handler
    SDL_Event e;
    
    while(SDL_PollEvent(&e) != 0) {
        //User requests quit
        if (e.type == SDL_QUIT) {
            return true;
        }
    }
    
    return false;
}

// Main function.
void ClientMenu::clientMenuRun() {
    std::list<ClientMenuTexture> textures = std::list<ClientMenuTexture>();
    
    ClientMenuImageTexture backgrounImage = ClientMenuImageTexture(this->clientMenuWindow);
    backgrounImage.menuTextureLoadFromFile("Bomber.JPG");
    backgrounImage.menuTextureSetModulation(ClientMenuUtils::clientMenuModulationColor());
    textures.push_back(backgrounImage);
    
    ClientMenuTextFieldTexture promptText = ClientMenuTextFieldTexture(this->clientMenuWindow);
    textures.push_back(promptText);
    
    bool loadingTextureOK = true;
    
    std::list<ClientMenuTexture>::iterator iterator = textures.begin();
    for (*iterator; iterator != textures.end(); ++iterator) {
        if (iterator->menuTextureGetLoaded() == false) {
            loadingTextureOK = false;
            break;
        }
    }
    
    if (loadingTextureOK == true) {
        while (this->clientMenuHandleQuitEvent() == false) {
            SDL_Rect imageRect;
            SDL_GetWindowSize(this->clientMenuWindow, &imageRect.w, &imageRect.h);
            
            // Background Image.
            backgrounImage.menuTextureRender(0, 0, &imageRect);
            
            // Prompt text.
            promptText.menuTextureSetTextFont("bummer3d.ttf", imageRect.w / 20);
            promptText.menuTextureSetTextProperties("Nombre del jugador:", ClientMenuUtils::clientMenuTextColor());
            promptText.menuTextureRender((imageRect.w - promptText.menuTextureGetWidth()) / 2, (imageRect.h - promptText.menuTextureGetHeight()) / 2);
            
            this->clientMenuShow();
        }
    }
}

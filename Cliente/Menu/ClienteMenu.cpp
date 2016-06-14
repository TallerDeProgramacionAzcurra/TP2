//
//  ClienteMenu.cpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuImageTexture.hpp"
#include "ClientMenuTexture.hpp"
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
    IMG_Quit();
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
    std::list<ClientMenuTexture *> textures = std::list<ClientMenuTexture *>();
    
    ClientMenuImageTexture backgrounImage = ClientMenuImageTexture(this->clientMenuRender);
    backgrounImage.menuTextureLoadFromFile("TP2/Cliente/Menu/Bomber.JPG");
    textures.push_back(&backgrounImage);
    
    bool loadingTextureOK = true;
    
    std::list<ClientMenuTexture *>::iterator iterator = textures.begin();
    for (*iterator; iterator != textures.end(); ++iterator) {
        if ((*iterator)->menuTextureGetLoaded() == false) {
            loadingTextureOK = false;
            break;
        }
    }
    
    if (loadingTextureOK == true) {
        while (this->clientMenuHandleQuitEvent() == false) {
            backgrounImage.menuTextureRender(0, 0);
            this->clientMenuShow();
        }
    }
}

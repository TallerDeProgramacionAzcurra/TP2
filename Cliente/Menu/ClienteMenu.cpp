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
bool ClientMenu::clientMenuHandleQuitEvent(SDL_Event *e) {
    //Event handler
    while(SDL_PollEvent(e) != 0) {
        //User requests quit
        if (e->type == SDL_QUIT) {
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
    
    ClientMenuTextTexture promptPlayerText = ClientMenuTextTexture(this->clientMenuWindow);
    promptPlayerText.menuTextureSetTextFont("bummer3d.ttf", 2);
    textures.push_back(promptPlayerText);
    
    ClientMenuTextFieldTexture inputPlayerText = ClientMenuTextFieldTexture(this->clientMenuWindow);
    inputPlayerText.menuTextureSetTextFont("bummer3d.ttf", 2);
    textures.push_back(inputPlayerText);
    
    bool loadingTextureOK = true;
    
    std::list<ClientMenuTexture>::iterator iterator = textures.begin();
    for (*iterator; iterator != textures.end(); ++iterator) {
        if (iterator->menuTextureGetLoaded() == false) {
            loadingTextureOK = false;
            break;
        }
    }
    
    if (loadingTextureOK == true) {
        SDL_Event event;
        while (this->clientMenuHandleQuitEvent(&event) == false) {
            this->clientMenuClear();
            
            SDL_Rect imageRect;
            imageRect.x = 0;
            imageRect.y = 0;
            SDL_GetWindowSize(this->clientMenuWindow, &imageRect.w, &imageRect.h);
            
            // Background Image.
            backgrounImage.menuTextureRender(0, 0, &imageRect);
            
            // Prompt text.
            promptPlayerText.menuTextureSetTextFont("bummer3d.ttf", imageRect.w / 20);
            promptPlayerText.menuTextureSetText("Nombre del jugador:");
            promptPlayerText.menuTextureRender((imageRect.w - promptPlayerText.menuTextureGetWidth()) / 2,
                                               imageRect.h / 2 - promptPlayerText.menuTextureGetHeight());
            
            // Prompt text.
            inputPlayerText.menuTextureSetTextFont("bummer3d.ttf", imageRect.w / 20);
            inputPlayerText.menuTextureSetText("Jugador #");
            inputPlayerText.menuTextureRender((imageRect.w - inputPlayerText.menuTextureGetWidth()) / 2,
                                              imageRect.h / 2);
            
            // Handler mouse touch up event.
            if (event.type == SDL_MOUSEBUTTONUP) {
                inputPlayerText.menuTextureTextFieldHandlerMourEvent(&event);
            }

            
            this->clientMenuShow();
        }
    }
}

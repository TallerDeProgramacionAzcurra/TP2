//
//  ClienteMenu.cpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <string>

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
bool ClientMenu::clientMenuHandleQuitEvent(SDL_Event *event) {
    //Event handler
    while (SDL_PollEvent(event) != 0) {
        //User requests quit
        if (event->type == SDL_QUIT) {
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
    promptPlayerText.menuTextureSetText("Nombre del jugador:");
    textures.push_back(promptPlayerText);
    
    std::string inputText = "Jugador #";
    ClientMenuTextFieldTexture inputPlayerText = ClientMenuTextFieldTexture(this->clientMenuWindow);
    inputPlayerText.menuTextureSetTextFont("bummer3d.ttf", 2);
    inputPlayerText.menuTextureSetText(inputText.c_str());
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
            
            // Handler mouse touch up event.
            if (event.type == SDL_MOUSEBUTTONUP) {
                inputPlayerText.menuTextureTextFieldHandlerMouseEvent(&event);
                while (SDL_PollEvent(&event) == 0) {}
            }
            
            if (inputPlayerText.menuTextureTextFieldIsSelected() == true && (event.type == SDL_KEYDOWN || event.type == SDL_TEXTINPUT)) {
                inputPlayerText.menuTextureTextFieldHanldlerInputEvent(&event, &inputText);
                while (SDL_PollEvent(&event) == 0) {}
            }
            
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
            inputPlayerText.menuTextureSetText(inputText.c_str());
            inputPlayerText.menuTextureRender((imageRect.w - inputPlayerText.menuTextureGetWidth()) / 2,
                                              imageRect.h / 2);
            
            this->clientMenuShow();
            
        }
    }
}

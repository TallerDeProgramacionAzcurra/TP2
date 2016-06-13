//
//  ClienteMenu.cpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClienteMenu.hpp"

// Constructor and Destructor.
ClientMenu::ClientMenu(const char *menuTitle, const int menuWidth, const int menuHeight) {
    printf("ClienteMenu.cpp - Constructor.\n");
    
    // The window we'll be rendering to.
    this->clientMenuWindow = NULL;
    
    this->clientMenuSurfaces = new std::list<SDL_Surface *>;
    
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ClienteMenu.cpp - SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        //Create window.
        this->clientMenuWindow = SDL_CreateWindow(menuTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, menuWidth, menuHeight, SDL_WINDOW_SHOWN);
        
        if (this->clientMenuWindow == NULL) {
            printf("ClienteMenu.cpp - SDL Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
    }
}

ClientMenu::~ClientMenu() {
    printf("ClienteMenu.cpp - Destructor.\n");

    // Free used surfaces.
    std::list<SDL_Surface *>::iterator iterator = this->clientMenuSurfaces->begin();
    for (iterator; iterator != this->clientMenuSurfaces->end(); ++iterator) {
        SDL_FreeSurface(*iterator);
    }
    
    delete this->clientMenuSurfaces;
    
    // Free screen surface.
    SDL_Surface *screenSurface = SDL_GetWindowSurface(this->clientMenuWindow);
    SDL_FreeSurface(screenSurface);
    
    //Destroy window
    SDL_DestroyWindow(this->clientMenuWindow);
    
    //Quit SDL subsystems
    SDL_Quit();
}

// Public functions.
void ClientMenu::clientMenuShow() {
    printf("ClienteMenu.cpp - clientMenuShow.\n");
    
    // Update the surface to show the changes made to the windows surface.
    SDL_UpdateWindowSurface(this->clientMenuWindow);
    
    SDL_Delay(2000);
}

void ClientMenu::clientMenuFillWithColor(const int red, const int green, const int blue) {
    printf("ClienteMenu.cpp - clientMenuFillWithColor.\n");
    
    // Get window surface.
    SDL_Surface *screenSurface = SDL_GetWindowSurface(this->clientMenuWindow);
    
    // Fill the surface with a color.
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, red, green, blue));
}

bool ClientMenu::clientMenuAddImage(const char *imageName) {
    printf("ClienteMenu.cpp - clientMenuShowImage.\n");
    
    // Get window surface.
    SDL_Surface *screenSurface = SDL_GetWindowSurface(this->clientMenuWindow);
    
    //The image we will load and show on the screen
    SDL_Surface* sdlImage = SDL_LoadBMP(imageName);
    
    if (sdlImage == NULL ) {
        printf("ClienteMenu.cpp - Unable to load image %s! SDL Error: %s\n", imageName, SDL_GetError());
        return false;
    }
    
    SDL_BlitSurface(sdlImage, NULL, screenSurface, NULL );
    this->clientMenuSurfaces->push_back(sdlImage);
    
    return true;
}

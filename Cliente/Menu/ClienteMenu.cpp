//
//  ClienteMenu.cpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClienteMenu.hpp"

static int const kScreenWidth = 640;
static int const kScreenheight = 480;

// Constructor and Destructor.
ClientMenu::ClientMenu(const char *menuTitle, const int menuWidth, const int menuHeight) {
    printf("ClienteMenu.cpp - Constructor.\n");
    
    // The window we'll be rendering to.
    this->clientMenuWindow = NULL;
    
    this->clientMenuTexture = NULL;
    
    this->clientMenuSurfaces = new std::list<SDL_Surface *>;
    
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
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            }
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
    
    SDL_DestroyTexture(this->clientMenuTexture);
    
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

// Public functions.
void ClientMenu::clientMenuShow() {
    printf("ClienteMenu.cpp - clientMenuShow.\n");
    
    // Clear screen.
    SDL_SetRenderDrawColor(this->clientMenuRender, 255, 255, 255, 255);
    SDL_RenderClear(this->clientMenuRender);
    
	// Render texture to screen.
	SDL_RenderCopy(this->clientMenuRender, this->clientMenuTexture, NULL, NULL );
    
    // Update screen.
    SDL_RenderPresent(this->clientMenuRender);
}

// Drawing methods.
void ClientMenu::clientMenuFillWithColor(const int red, const int green, const int blue) {
    printf("ClienteMenu.cpp - clientMenuFillWithColor.\n");
    
    // Get window surface.
    SDL_Surface *screenSurface = SDL_GetWindowSurface(this->clientMenuWindow);
    
    // Fill the surface with a color.
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, red, green, blue));
}

bool ClientMenu::clientMenuOptimizeImageSurface(SDL_Surface* sdlImage, SDL_Rect stretchRect, const char *imageName) {
    printf("ClienteMenu.cpp - clientMenuOptimizeImageSurface.\n");
    
    // Get window surface.
    SDL_Surface *screenSurface = SDL_GetWindowSurface(this->clientMenuWindow);
    
    SDL_Surface *optimizedImage = SDL_ConvertSurface(sdlImage, screenSurface->format, NULL);
    if (optimizedImage == NULL) {
        printf("ClienteMenu.cpp - Unable to optimize image %s! SDL Error: %s\n", imageName, SDL_GetError());
        SDL_FreeSurface(sdlImage);
        return false;
    }
    
    SDL_BlitScaled(optimizedImage, NULL, screenSurface, &stretchRect);
    this->clientMenuSurfaces->push_back(optimizedImage);
    SDL_FreeSurface(sdlImage);
    
    return true;
}

bool ClientMenu::clientMenuAddBMPImage(const char *imageName, const int xPost, const int yPost, const int width, const int height) {
    printf("ClienteMenu.cpp - clientMenuAddBMPImage.\n");
    
    //The image we will load and show on the screen
    SDL_Surface* sdlImage = SDL_LoadBMP(imageName);
    
    if (sdlImage == NULL ) {
        printf("ClienteMenu.cpp - Unable to load image %s! SDL Error: %s\n", imageName, SDL_GetError());
        return false;
    }
    
    SDL_Rect stretchRect;
    stretchRect.x = xPost;
    stretchRect.y = yPost;
    stretchRect.w = width;
    stretchRect.h = height;
    
    return this->clientMenuOptimizeImageSurface(sdlImage, stretchRect, imageName);
}

bool ClientMenu::clientMenuAddJPGImage(const char *imageName, const int xPost, const int yPost, const int width, const int height) {
    printf("ClienteMenu.cpp - clientMenuAddJPGImage.\n");
    
    //Initialize JPG loading
    int imgFlags = IMG_INIT_JPG;
    
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("ClienteMenu.cpp - SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    
    //Load image at specified path
    SDL_Surface *jpgImage = IMG_Load(imageName);
    
    if (jpgImage == NULL) {
        printf("ClienteMenu.cpp - Unable to load image %s! SDL_image Error: %s\n", imageName, IMG_GetError());
        return false;
    }
    
    SDL_Rect stretchRect;
    stretchRect.x = xPost;
    stretchRect.y = yPost;
    stretchRect.w = width;
    stretchRect.h = height;
    
    return this->clientMenuOptimizeImageSurface(jpgImage, stretchRect, imageName);
}

bool ClientMenu::clientMenuLoadTexture(const char *imageName) {
    printf("ClienteMenu.cpp - clientMenuLoadTexture.\n");
    
    //Load image at specified path
    SDL_Surface *loadedImage = IMG_Load(imageName);
    
    if (loadedImage == NULL) {
        printf("ClienteMenu.cpp - Unable to load image %s! SDL_image Error: %s\n", imageName, IMG_GetError());
        return false;
    } else {
        //Create texture from surface pixels
        this->clientMenuTexture = SDL_CreateTextureFromSurface(this->clientMenuRender, loadedImage);
        
        if (this->clientMenuTexture == NULL) {
            printf("ClienteMenu.cpp - Unable to create texture from %s! SDL Error: %s\n", imageName, SDL_GetError());
            SDL_FreeSurface(loadedImage);
            return false;
        }
    }
    
    SDL_FreeSurface(loadedImage);
    
    return true;
}

void ClientMenu::clientMenuAddViewPort() {
    //Top left corner viewport
    SDL_Rect topLeftViewport;
    topLeftViewport.x = kScreenWidth / 4;
    topLeftViewport.y = 0;
    topLeftViewport.w = kScreenWidth / 2;
    topLeftViewport.h = kScreenheight;
    SDL_RenderSetViewport(this->clientMenuRender, &topLeftViewport);
				
    //Render texture to screen.
    SDL_RenderCopy(this->clientMenuRender, this->clientMenuTexture, NULL, NULL);
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
    if (this->clientMenuLoadTexture("TP2/Cliente/Menu/Bomber.JPG") == true) {
        while (this->clientMenuHandleQuitEvent() == false) {
            this->clientMenuShow();
        }
    }
}

//
//  ClienteMenu.hpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClienteMenu_hpp
#define ClienteMenu_hpp

#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>

class ClientMenu {
public:
    // Constructor and Destructor.
    ClientMenu(const char *menuTitle, const int menuWidth, const int menuHeight);
    virtual ~ClientMenu();
    
    // Main function.
    void clientMenuRun();
    
private:
    SDL_Window *clientMenuWindow;
    SDL_Renderer *clientMenuRender;
    SDL_Texture *clientMenuTexture;
    
    std::list<SDL_Surface *> *clientMenuSurfaces;
    
    // Drawing methods.
    bool clientMenuLoadTexture(const char *imageName);
    void clientMenuFillWithColor(const int red, const int green, const int blue);
    bool clientMenuOptimizeImageSurface(SDL_Surface* sdlImage, SDL_Rect stretchRect, const char *imageName);
    bool clientMenuAddBMPImage(const char *imageName, const int xPost, const int yPost, const int width, const int height);
    bool clientMenuAddJPGImage(const char *imageName, const int xPost, const int yPost, const int width, const int height);
    void clientMenuAddViewPort();
    
    // Showing functions.
    void clientMenuShow();
    
    // Events methods.
    bool clientMenuHandleQuitEvent();
};

#endif /* ClienteMenu_hpp */

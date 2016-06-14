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
    
    // Public functions.
    void clientMenuShow();
    
    // Drawing methods.
    void clientMenuFillWithColor(const int red, const int green, const int blue);
    
    bool clientMenuOptimizeImageSurface(SDL_Surface* sdlImage, SDL_Rect stretchRect, const char *imageName);
    bool clientMenuAddBMPImage(const char *imageName, const int xPost, const int yPost, const int width, const int height);
    bool clientMenuAddJPGImage(const char *imageName, const int xPost, const int yPost, const int width, const int height);
    void clientMenuLoadTexture(const char *imageName);
    
    // Events methods.
    bool clientMenuHandleQuitEvent();
    
private:
    SDL_Window *clientMenuWindow;
    SDL_Renderer *clientMenuRender;
    
    std::list<SDL_Surface *> *clientMenuSurfaces;
};

#endif /* ClienteMenu_hpp */

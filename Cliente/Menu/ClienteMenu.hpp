//
//  ClienteMenu.hpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClienteMenu_hpp
#define ClienteMenu_hpp

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
    bool clientMenuAddImage(const char *imageName);
    
    // Events methods.
    void clientMenuHandleEvents();
    
private:
    SDL_Window *clientMenuWindow;
    
    std::list<SDL_Surface *> *clientMenuSurfaces;
    
    bool clientMenuQuit;
};

#endif /* ClienteMenu_hpp */

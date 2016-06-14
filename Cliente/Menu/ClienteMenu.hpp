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
    
    // Main function.
    void clientMenuRun();
    
private:
    SDL_Window *clientMenuWindow;
    SDL_Renderer *clientMenuRender;
    
    // Drawing methods.
    bool clientMenuLoadTexture(const char *imageName);

    // Showing functions.
    void clientMenuClear();
    void clientMenuShow();
    
    // Events methods.
    bool clientMenuHandleQuitEvent();
};

#endif /* ClienteMenu_hpp */

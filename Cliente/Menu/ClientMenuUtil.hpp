//
//  ClientMenuUtil.hpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuUtil_hpp
#define ClientMenuUtil_hpp

#include <SDL2/SDL.h>
#include <stdio.h>

class ClientMenuUtils {
public:
    static SDL_Color clientMenuModulationColor();
    static SDL_Color clientMenuTextColor();
    static SDL_Color clientMenuTextColorSelected();
};

#endif /* ClientMenuUtil_hpp */

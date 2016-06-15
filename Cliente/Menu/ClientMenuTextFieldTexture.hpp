//
//  ClientMenuTextFieldTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTextFieldTexture_hpp
#define ClientMenuTextFieldTexture_hpp

#include <stdio.h>

#include "ClientMenuTextTexture.hpp"

class ClientMenuTextFieldTexture: public ClientMenuTextTexture {
public:
    ClientMenuTextFieldTexture(SDL_Window *menuWindow);
    virtual ~ClientMenuTextFieldTexture();
};

#endif /* ClientMenuTextFieldTexture_hpp */

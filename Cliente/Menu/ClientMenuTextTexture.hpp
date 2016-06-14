//
//  ClientMenuTextTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTextTexture_hpp
#define ClientMenuTextTexture_hpp

#include <stdio.h>

#include "ClientMenuTexture.hpp"

class ClientMenuTextTexture : public ClientMenuTexture {
public:
    ClientMenuTextTexture(SDL_Renderer *menuTextureRenderer);
    virtual ~ClientMenuTextTexture();
    
    //Loads image at specified path
    void menuTextureLoadFromFile(const char *filePath);
};

#endif /* ClientMenuTextTexture_hpp */

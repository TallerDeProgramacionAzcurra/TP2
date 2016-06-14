//
//  ClientMenuTest.cpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <stdio.h>

#include "ClienteMenu.hpp"

static int const kScreenWidth = 640;
static int const kScreenheight = 480;

int main(int argc, char **argv) {
    ClientMenu clientMenu = ClientMenu("Menú", kScreenWidth, kScreenheight);
    
    while (clientMenu.clientMenuHandleQuitEvent() == false) {
        clientMenu.clientMenuLoadTexture("TP2/Cliente/Menu/Bomber.JPG");
        clientMenu.clientMenuShow();
    }
}
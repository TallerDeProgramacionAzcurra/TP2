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
    
    clientMenu.clientMenuFillWithColor(0, 0, 100);
    clientMenu.clientMenuShow();
    
    if (clientMenu.clientMenuAddImage("TP2/Cliente/Menu/Bomber.bmp") == true) {
        clientMenu.clientMenuShow();
    }
}
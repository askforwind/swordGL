/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-23 11:56
#
# Filename: SDLSupport.h
#
# Description: 
#
==================================================================*/
#ifndef SWORD_SDLSUPPORT_H_
#define SWORD_SDLSUPPORT_H_
#include<string>
#include"Platform.h"

class SDL_Window;

SWORD_BEGIN

void InitSDLEverything();

std::string GetSDLWinHandle(SDL_Window* win);

SWORD_END

#endif

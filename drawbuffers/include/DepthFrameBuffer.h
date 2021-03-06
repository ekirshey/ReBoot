/*
* FrameBuffer is part of the ReBoot distribution (https://github.com/octopusprime314/ReBoot.git).
* Copyright (c) 2017 Peter Morley.
*
* ReBoot is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* ReBoot is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/**
*  DepthFrameBuffer class. Stores framebuffer depth
   into a texture format for post processing and offscreen rendering
*/

#pragma once
#include "GLIncludes.h"
#include "RenderTexture.h"
#include <iostream>

class DepthFrameBuffer {

    GLuint        _frameBufferContext;
    RenderTexture _renderTexture;

public:
    DepthFrameBuffer(unsigned int width, unsigned int height);
    ~DepthFrameBuffer();
    GLuint        getFrameBufferContext();
    GLuint        getTextureContext();
    GLuint        getWidth();
    GLuint        getHeight();
};
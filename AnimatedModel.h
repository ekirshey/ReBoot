/*
* AnimatedModel is part of the ReBoot distribution (https://github.com/octopusprime314/ReBoot.git).
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
*  AnimatedModel class. Derives from Model class and is used for Models
*  that have animation skinning transforms.  
*/

#pragma once
#include "Model.h"
#include "Animation.h"
#include "AnimationBuilder.h"

class AnimatedModel : public Model {

    std::vector<Animation*> _animations;

    void updateKeyboard(unsigned char key, int x, int y); //Do stuff based on keyboard update
    void updateMouse(int button, int state, int x, int y); //Do stuff based on mouse update

public:
    AnimatedModel(std::string name, ViewManagerEvents* eventWrapper);
    ~AnimatedModel();
	void addAnimation(Animation* animation);
};
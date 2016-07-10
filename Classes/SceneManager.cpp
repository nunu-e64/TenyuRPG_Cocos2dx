//
//  SceneManager.cpp
//  TenyuCocos2dx
//
//  Created by Amano Fumiya on 2016/07/10.
//
//

#include "SceneManager.hpp"
#include "Title.h"

USING_NS_CC;

Scene* SceneManager::getFirstScene() {
    return Title::createScene();
}
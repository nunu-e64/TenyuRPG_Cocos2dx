//
//  SceneManager.hpp
//  TenyuCocos2dx
//
//  Created by Amano Fumiya on 2016/07/10.
//
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include <stdio.h>
#include "cocos2d.h"

// シーンの切り替え担当
class SceneManager {
public:
    static cocos2d::Scene getFirstScene();
};

#endif /* SceneManager_hpp */

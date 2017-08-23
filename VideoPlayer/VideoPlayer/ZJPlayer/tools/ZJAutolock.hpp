//
//  ZJAutolock.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef ZJAutolock_hpp
#define ZJAutolock_hpp

#include "ZJMutex.hpp"
#include <stdio.h>

class ZJAutolock{
public:
    ZJAutolock(ZJMutex* mutex);
    ~ZJAutolock();
private:
    ZJMutex* pmutex;
};
#endif /* ZJAutolock_hpp */

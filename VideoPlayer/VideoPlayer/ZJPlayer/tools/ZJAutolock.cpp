//
//  ZJAutolock.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "ZJAutolock.hpp"
ZJAutolock::ZJAutolock(ZJMutex* mutex):pmutex(mutex)
{
    pmutex->lock();
}
ZJAutolock::~ZJAutolock()
{
    pmutex->unlock();
}

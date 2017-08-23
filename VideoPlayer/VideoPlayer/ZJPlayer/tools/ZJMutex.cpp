//
//  ZJMutex.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "ZJMutex.hpp"
#include "ZJType.h"
ZJMutex::ZJMutex()
{
    pthread_mutex_init(&mutext, NULL);
}
ZJMutex::~ZJMutex()
{
    pthread_mutex_destroy(&mutext);
}
void ZJMutex::lock()
{
    pthread_mutex_lock(&mutext);
}
void ZJMutex::unlock()
{
    pthread_mutex_unlock(&mutext);
}

//
//  ZJMutex.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef ZJMutex_hpp
#define ZJMutex_hpp

#include <stdio.h>
#include <pthread.h>
class ZJMutex{
public:
    ZJMutex();
    ~ZJMutex();
    void lock();
    void unlock();
private:
    pthread_mutex_t mutext;
};
#endif /* ZJMutex_hpp */

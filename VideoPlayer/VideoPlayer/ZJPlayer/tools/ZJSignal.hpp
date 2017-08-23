//
//  ZJSignal.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef ZJSignal_hpp
#define ZJSignal_hpp

#include <stdio.h>
#include <pthread.h>
#include "ZJType.h"
class ZJSignal{
public:
    ZJSignal();
    ~ZJSignal();
    void send();
    void wait();
    void timeWait(ZJ_U32 ntimeout);
private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
#endif /* ZJSignal_hpp */

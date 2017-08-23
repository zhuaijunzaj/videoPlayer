//
//  ZJSignal.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "ZJSignal.hpp"

ZJSignal::ZJSignal()
{
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
}
ZJSignal::~ZJSignal()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void ZJSignal::send()
{
    pthread_cond_signal(&cond);
}
void ZJSignal::wait()
{
    pthread_cond_wait(&cond, &mutex);
}
void ZJSignal::timeWait(ZJ_U32 ntimeout)
{
    struct timespec timeout;
    timeout.tv_nsec = 0;
    timeout.tv_sec = time(NULL) + ntimeout;
    pthread_cond_timedwait(&cond, &mutex, &timeout);
}

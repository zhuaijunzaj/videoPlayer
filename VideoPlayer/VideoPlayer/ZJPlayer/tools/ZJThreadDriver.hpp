//
//  ZJThreadDriver.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef ZJThreadDriver_hpp
#define ZJThreadDriver_hpp

#include <stdio.h>
#include "ZJSignal.hpp"
#include "ZJThreadWorker.hpp"
#include <pthread.h>

typedef enum
{
    ZJThreadDriverStatus_Init,
    ZJThreadDriverStatus_Idle,
    ZJThreadDriverStatus_Running,
    ZJThreadDriverStatus_Stopped,
    ZJThreadDriverStatus_Unknown = ZJ_U16_MAX,
} ZJThreadDriverStatus;

class ZJThreadDriver{
public:
    ZJThreadDriver(ZJThreadWorker* worker,const char* driverName = NULL);
    ~ZJThreadDriver();
    
    void Run();
    void Pause();
    void Exit();
    
    void RunSteps(ZJ_U32 nSteps);
private:
    static void* driverEntry(void* args);
    
    ZJ_U32                  m_runStreps;
    const char*             dirverName;
    ZJSignal               pauseSig;
    ZJSignal               provokerSig;
    pthread_t              threadHandle;
    ZJThreadWorker*         threadWorker;
    ZJThreadDriverStatus    m_status;
};
#endif /* ZJThreadDriver_hpp */

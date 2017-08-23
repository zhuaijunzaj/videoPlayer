//
//  ZJThreadDriver.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "ZJThreadDriver.hpp"

ZJThreadDriver::ZJThreadDriver(ZJThreadWorker* worker,const char* driverName)
:threadWorker(worker),m_runStreps(0),m_status(ZJThreadDriverStatus_Init)
{
    this->dirverName = driverName;
    pthread_create(&threadHandle, NULL, driverEntry, this);
    m_status = ZJThreadDriverStatus_Idle;
}
ZJThreadDriver::~ZJThreadDriver()
{
    pthread_join(threadHandle, NULL);
}

void ZJThreadDriver::Run()
{
    if (ZJThreadDriverStatus_Init == m_status || ZJThreadDriverStatus_Idle == m_status){
        m_status = ZJThreadDriverStatus_Running;
        provokerSig.send();
    }
}
void ZJThreadDriver::Pause()
{
    if (ZJThreadDriverStatus_Running == m_status){
        m_status = ZJThreadDriverStatus_Idle;
        pauseSig.wait();
        provokerSig.send();
    }
}
void ZJThreadDriver::Exit()
{
    if (ZJThreadDriverStatus_Stopped != m_status){
        m_status = ZJThreadDriverStatus_Stopped;
        provokerSig.send();
        pthread_join(threadHandle, NULL);
    }
}
void ZJThreadDriver::RunSteps(ZJ_U32 nSteps)
{
    if (ZJThreadDriverStatus_Idle == m_status){
        m_runStreps = nSteps;
        m_status = ZJThreadDriverStatus_Running;
        provokerSig.send();
    }
}

void* ZJThreadDriver::driverEntry(void *args)
{
    if (args == NULL) return NULL;
    ZJThreadDriver *drivier = (ZJThreadDriver*)args;
    if (drivier->threadWorker == NULL) return NULL;
    while (1) {
        switch (drivier->m_status) {
            case ZJThreadDriverStatus_Init:
                drivier->provokerSig.wait();
                break;
            case ZJThreadDriverStatus_Idle:
                drivier->pauseSig.send();
                drivier->provokerSig.wait();
                break;
            case ZJThreadDriverStatus_Running:
                drivier->threadWorker->DoRunning();
                if(drivier->m_runStreps > 0){
                    drivier->m_runStreps -- ;
                    if (drivier->m_runStreps == 0){
                        drivier->m_runStreps = ZJThreadDriverStatus_Idle;
                    }
                }
                break;
            case ZJThreadDriverStatus_Stopped:
                drivier->threadWorker->DoExit();
                return NULL;
                break;
            default:
                return NULL;
                break;
        }
    }
    return NULL;
}

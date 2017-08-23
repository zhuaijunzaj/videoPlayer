//
//  ZJThreadWorker.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "ZJThreadWorker.hpp"
#include <unistd.h>
#include <sys/time.h>
void ZJThreadWorker::DoIdle()
{
    usleep(200*1000);
}
void ZJThreadWorker::DoExit()
{
    
}

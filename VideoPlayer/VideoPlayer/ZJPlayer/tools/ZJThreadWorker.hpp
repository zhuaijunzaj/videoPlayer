//
//  ZJThreadWorker.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef ZJThreadWorker_hpp
#define ZJThreadWorker_hpp

#include <stdio.h>

class ZJThreadWorker
{
public:
    virtual ~ZJThreadWorker(){};
    virtual void DoRunning() = 0;
    virtual void DoIdle();
    virtual void DoExit();
};
#endif /* ZJThreadWorker_hpp */

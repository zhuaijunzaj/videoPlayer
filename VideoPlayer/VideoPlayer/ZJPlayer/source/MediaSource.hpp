//
//  MediaSource.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef MediaSource_hpp
#define MediaSource_hpp

#define Source_Err_None            0x00000000
#define Source_Err_ReadEOS         0x0000F001
#define Source_Err_OpenFaild       0x0000F002
#define Source_Err_SetPosFaild     0x0000F003
#define Source_Err_ReadAudioPkt    0x0000F004
#define Source_Err_ReadVideoPkt    0x0000F005
#define Source_Err_NoStreamInfo    0x0000F006
#define Source_Err_NoMediaTrack    0x0000F007
#define Source_Err_ReadPacketFaild 0x0000F008

#define SRC_WAIT_TIME               5
#define SRC_MAX_SEEK_TRY (512+256+100)

#include <stdio.h>
#include "FFmpegReader.hpp"
#include "ZJThreadDriver.hpp"
#include "ZJAutolock.hpp"
class MediaSource:public ZJThreadWorker{
public:
    MediaSource();
    ~MediaSource();
    int openMedia(const char* path);
    void closeMedia();
    MediaContext* getMediaCtx();
private:
    virtual void DoRunning();
    
private:
    FFmpegReader    *reader;
    ZJThreadDriver  *driver;
    ZJMutex         mutex;
    ZJ_U32          duration;
};
#endif /* MediaSource_hpp */

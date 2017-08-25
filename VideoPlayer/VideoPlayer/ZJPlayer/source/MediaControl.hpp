//
//  MediaControl.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef MediaControl_hpp
#define MediaControl_hpp

#include <stdio.h>
#include "VideoDecoder.hpp"
#include "MediaSource.hpp"
#include "VideoRender.hpp"

typedef enum
{
    MediaCtrlStatus_Play,
    MediaCtrlStatus_Pause,
    MediaCtrlStatus_NoAct,
    MediaCtrlStatus_Unknown = -1,
} MediaCtrlStatus;

class MediaControl{
    
public:
    MediaControl();
    ~MediaControl();
    int openMedia(const char* path);
    void play(ZJ_U32 startPos);
    MediaContext* getMediaCtx();
private:
    bool hasVideo;
    bool hasAudio;
    MediaCtrlStatus status;
    MediaSource *mediaSource;
    VideoDecoder *videoDecoder;
    VideoRender *videoRender;
};
#endif /* MediaControl_hpp */

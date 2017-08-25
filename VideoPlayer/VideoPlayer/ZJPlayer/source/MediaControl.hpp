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
    MediaContext* getMediaCtx();
private:
    MediaCtrlStatus status;
    MediaSource *mediaSource;
    VideoDecoder *videoDecoder;
};
#endif /* MediaControl_hpp */

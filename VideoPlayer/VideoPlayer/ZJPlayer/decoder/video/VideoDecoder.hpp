//
//  VideoDecoder.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef VideoDecoder_hpp
#define VideoDecoder_hpp

#include <stdio.h>
#include "FFmpegReader.hpp"
#include "ZJAutolock.hpp"
#include "SourceMediaPort.hpp"


#define VIDEO_DEC_WAIT_TIME  10

class FFmpegVideoDecoder;

class VideoDecoder{
public:
    VideoDecoder();
    ~VideoDecoder();
    int openDecoder(MediaContext* ctx,SourceMediaPort *Port);
    void closeDecoder();
    ZJ_U32 getVideoFrame(AVFrame** videoFrame);
private:
    FFmpegVideoDecoder *videoDecoder;
    SourceMediaPort *mediaPort;
    ZJMutex mutex;
    AVPacket *currentPkt;
};
#endif /* VideoDecoder_hpp */

//
//  FFmpegReader.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef FFmpegReader_hpp
#define FFmpegReader_hpp

#include <stdio.h>
#include "ZJType.h"
#include <iostream>

extern "C"{
#include <libavutil/display.h>
#include <libavformat/avformat.h>
}

#define TIME_UNIT              1000
#define MAX_NOTIFY_WAIT        2000
#define MAX_NET_WORK_WAIT   1000*30
#define MAX_READ_PACKET_RETRY    32

typedef struct __MediaContext{
    bool   seekable;
    bool   hasVideo;
    bool   hasAudio;
    ZJ_U32 nChannels;
    ZJ_U32 nDuration;
    ZJ_U32 nAudioIndex;
    ZJ_U32 nVideoIndex;
    ZJ_U32 nVideoWidth;
    ZJ_U32 nVideoHeight;
    int    videoRotation;
    ZJ_U32 nSampleRate;
    ZJ_U32 nFrameSize;
    ZJ_U32 nSampleFormat;
    AVFormatContext* pFormatCtx;
    AVCodecContext* pAudioCodecCtx;
    AVCodecContext* pVideoCodecCtx;
    AVCodecParameters* pAudioDecParam;
    AVCodecParameters* pVideoDecParam;
}MediaContext;

class FFmpegReader{
public:
    FFmpegReader();
    ~FFmpegReader();
    ZJ_U32 openMedia(const char *path);
    ZJ_U32 setPlayerbackPos(ZJ_U32 pos);
    ZJ_U32 readPacket(AVPacket* packet);
    static AVPacket* allocPacket();
    static void releasePacket(AVPacket** pkt);
    static void releasePacketBuffer(AVPacket* pkt);
    void closeMedia();
    MediaContext mediaCtx;
    
    int videoIndex;
    int audioIndex;
    
};
#endif /* FFmpegReader_hpp */

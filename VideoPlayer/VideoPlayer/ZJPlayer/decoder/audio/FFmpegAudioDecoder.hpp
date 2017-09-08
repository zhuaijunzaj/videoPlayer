//
//  FFmpegAudioDecoder.hpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/8/31.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#ifndef FFmpegAudioDecoder_hpp
#define FFmpegAudioDecoder_hpp

#include <stdio.h>
#include "AudioDecoder.hpp"
#include "SDL.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
#include <libavutil/avstring.h>
#include <libavutil/pixfmt.h>
#include <libavutil/mem.h>
};

class FFmpegAudioDecoder{
public:
    FFmpegAudioDecoder();
    ~FFmpegAudioDecoder();
    int openDecoder(MediaContext* ctx);
    void closeDecoder();
    void flushDecoder();
    ZJ_U32 setInputPacket(AVPacket* inputPkt);
    ZJ_U32 getOutputFrame(AudioFrame* outputFrame,bool rawFFPCMBuf = false);
private:
    
private:
    AVCodecContext *codecCtx;
    AVCodecParameters *codecParam;
    AVFrame         *audio_frame;
    bool               m_PlanarPCM;
    double             m_TimeBase;
    AudioFrameBuffer   m_AudioFrameBuf;
    ZJ_U32             m_nMaxPCMBufSize;
};
#endif /* FFmpegAudioDecoder_hpp */

//
//  FFmpegVideoDecoder.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef FFmpegVideoDecoder_hpp
#define FFmpegVideoDecoder_hpp

#include <stdio.h>
#include "VideoDecoder.hpp"

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavcodec/videotoolbox.h>
};

#define Video_Dec_Err_None            0x00000000
#define Video_Dec_Err_ReadEOS         0x000F1001
#define Video_Dec_Err_OpenFaild       0x000F1002
#define Video_Dec_Err_SkipPkt         0x000F1003
#define Video_Dec_Err_KeepPkt         0x000F1004
#define Video_Dec_Err_NeedNewPkt      0x000F1005
#define Video_Dec_Err_DecFrameFaild   0x000F1007


class FFmpegVideoDecoder{
public:
    FFmpegVideoDecoder();
    ~FFmpegVideoDecoder();
    
    int openDecoder(MediaContext* mediaCtx);
    void closeDecoder();
    ZJ_U32 setInputPacket(AVPacket* inputPacket);
    ZJ_U32 getOutputFrame(AVFrame* outPutFrame);
private:
    static AVPixelFormat getDecoderFormat(AVCodecContext* contex,AVPixelFormat const formats[]);
    
private:
    AVFrame *frame;
    AVCodecContext *codecCtx;
    AVCodecParameters *codecParam;
    double timeBase;
};
#endif /* FFmpegVideoDecoder_hpp */

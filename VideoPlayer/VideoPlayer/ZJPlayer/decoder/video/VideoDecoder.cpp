//
//  VideoDecoder.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "VideoDecoder.hpp"
#include "FFmpegVideoDecoder.hpp"

VideoDecoder::VideoDecoder()
{
    videoDecoder = new FFmpegVideoDecoder();
}
VideoDecoder::~VideoDecoder()
{
    if (videoDecoder) delete videoDecoder;
}

int VideoDecoder::openDecoder(MediaContext *ctx)
{
    if (ctx == NULL) return -1;
    ZJAutolock lock(&mutex);
    int ret = videoDecoder->openDecoder(ctx);
    return ret;
}
void VideoDecoder::closeDecoder()
{
    ZJAutolock lock(&mutex);
    videoDecoder->closeDecoder();
}

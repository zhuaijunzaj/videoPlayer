//
//  VideoDecoder.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "VideoDecoder.hpp"
#include "FFmpegVideoDecoder.hpp"
#include <unistd.h>
#include <sys/time.h>
VideoDecoder::VideoDecoder():currentPkt(NULL)
{
    videoDecoder = new FFmpegVideoDecoder();
}
VideoDecoder::~VideoDecoder()
{
    if (videoDecoder) delete videoDecoder;
}

int VideoDecoder::openDecoder(MediaContext *ctx,SourceMediaPort *Port)
{
    if (ctx == NULL || Port == NULL) return -1;
    ZJAutolock lock(&mutex);
    mediaPort = Port;
    int ret = videoDecoder->openDecoder(ctx);
    return ret;
}
void VideoDecoder::closeDecoder()
{
    ZJAutolock lock(&mutex);
    if (currentPkt){
        mediaPort->returnVideoEmptyPacket(currentPkt);
        currentPkt = NULL;
    }
    videoDecoder->closeDecoder();
}

ZJ_U32 VideoDecoder::getVideoFrame(AVFrame **videoFrame)
{
    ZJ_U32 nret = 0;
    if (videoFrame){
        ZJAutolock lock(&mutex);
        nret = videoDecoder->getOutputFrame(videoFrame);
        if (nret != Video_Dec_Err_None){
            if (currentPkt == NULL){
                nret = mediaPort->getVideoDataPacket(&currentPkt);
            }
            if (nret == 0 && currentPkt){
                nret = videoDecoder->setInputPacket(currentPkt);
                if (nret != Video_Dec_Err_KeepPkt){
                    mediaPort->returnVideoEmptyPacket(currentPkt);
                    currentPkt = NULL;
                }
                nret = videoDecoder->getOutputFrame(videoFrame);
            }else{
                usleep(VIDEO_DEC_WAIT_TIME*1000);
            }
        }
    }
    return nret;
}

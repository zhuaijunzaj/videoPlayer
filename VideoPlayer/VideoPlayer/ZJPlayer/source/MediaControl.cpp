//
//  MediaControl.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "MediaControl.hpp"

MediaControl::MediaControl():status(MediaCtrlStatus_NoAct),hasAudio(false),hasVideo(false)
{
    mediaSource = new MediaSource();
    videoDecoder = new VideoDecoder();
    videoRender = new VideoRender();
}
MediaControl::~MediaControl()
{
    if (mediaSource) delete mediaSource;
    if (videoRender) delete videoRender;
    if (videoDecoder) delete videoDecoder;
}

int MediaControl::openMedia(const char *path)
{
    int ret = mediaSource->openMedia(path);
    if (ret == 0){
        MediaContext *ctx = mediaSource->getMediaCtx();
        if (!ctx || (!ctx->hasAudio && !ctx->hasVideo)){
            mediaSource->closeMedia();
            return -1;
        }
        if (ctx->hasVideo){
            ret = videoDecoder->openDecoder(ctx);
            if (ret == 0){
                // open render
                ret = videoRender->openDevice(ctx);
                if (ret != 0){
                    videoDecoder->closeDecoder();
                    videoRender->closeDevice();
                    return -1;
                }else{
                    hasVideo = true;
                }
            }else{
                videoDecoder->closeDecoder();
            }
            
        }else{
            hasVideo =false;
        }
        if (ctx->hasAudio){
            
        }else{
            
        }
        
    }
    return 0;
}

void MediaControl::play(ZJ_U32 startPos)
{
    mediaSource->Run();
    if (startPos > 0){
        
    }
    if (hasVideo && videoRender) videoRender->Run();
}




































































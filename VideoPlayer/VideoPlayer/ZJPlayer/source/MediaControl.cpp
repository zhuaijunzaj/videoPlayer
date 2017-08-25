//
//  MediaControl.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "MediaControl.hpp"

MediaControl::MediaControl():status(MediaCtrlStatus_NoAct)
{
    mediaSource = new MediaSource();
    videoDecoder = new VideoDecoder();
}
MediaControl::~MediaControl()
{
    if (mediaSource) delete mediaSource;
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
            if (ret != 0){
                videoDecoder->closeDecoder();
                return -1;
            }
            // open render
        }
        
    }
    return 0;
}

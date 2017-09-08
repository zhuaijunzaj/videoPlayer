//
//  MediaControl.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "MediaControl.hpp"
#include "SourceMediaPort.hpp"
MediaControl::MediaControl():status(MediaCtrlStatus_NoAct),hasAudio(false),hasVideo(false)
{
    mediaSource = new MediaSource();
    videoDecoder = new VideoDecoder();
    videoRender = new VideoRender();
    audioDecoder = new  AudioDecoder();
    audioRender = new AudioRender();
}
MediaControl::~MediaControl()
{
    if (mediaSource) delete mediaSource;
    if (videoRender) delete videoRender;
    if (videoDecoder) delete videoDecoder;
    if (audioDecoder) delete audioDecoder;
    if (audioRender) delete audioRender;
}

int MediaControl::openMedia(void* window,const char* path,int width,int height)
{
    int ret = mediaSource->openMedia(path);
    if (ret == 0){
        MediaContext *ctx = mediaSource->getMediaCtx();
        if (!ctx || (!ctx->hasAudio && !ctx->hasVideo)){
            mediaSource->closeMedia();
            return -1;
        }
        if (ctx->hasVideo){
            SourceMediaPort *port = mediaSource->getVideoSourcePort();
            ret = videoDecoder->openDecoder(ctx,port);
            if (ret == 0){
                // open render
                ret = videoRender->openDevice(window,ctx,videoDecoder,width,height);
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
            SourceMediaPort *port = mediaSource->getVideoSourcePort();
            ret = audioDecoder->openDecoder(ctx, port);
            if (ret == 0){
                // open audio render
                ret = audioRender->OpenDevice(ctx, port,audioDecoder);
                if (ret != 0){
                    audioDecoder->closeDecoder();
                    audioRender->closeDevice();
                    return -1;
                }else{
                    hasAudio = true;
                }
            }else{
                audioDecoder->closeDecoder();
            }
        }else{
            hasAudio = false;
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
    if (hasAudio && audioRender) audioRender->Run();
    status =  MediaCtrlStatus_Play;
}

MediaContext* MediaControl::getMediaCtx()
{
    MediaContext *context = NULL;
    if (mediaSource){
        context =  mediaSource->getMediaCtx();
    }
    return context;
}



































































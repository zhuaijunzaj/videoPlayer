//
//  AudioDecoder.cpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/8/31.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#include "AudioDecoder.hpp"
#include "FFmpegAudioDecoder.hpp"


AudioDecoder::AudioDecoder():audioDecoder(NULL)
{
    audioDecoder = new FFmpegAudioDecoder();
}
AudioDecoder::~AudioDecoder()
{
    if (audioDecoder) delete audioDecoder;
}
int AudioDecoder::openDecoder(MediaContext *ctx, SourceMediaPort *Port)
{
    if (ctx == NULL || Port == NULL) return -1;
    mediaPort = Port;
    ZJAutolock lock(&mutex);
    int ret = audioDecoder->openDecoder(ctx);
    return ret;
}

void AudioDecoder::closeDecoder()
{
    ZJAutolock lock(&mutex);
    if (sourcePacket){
        mediaPort->returnAudioEmptyPacket(sourcePacket);
        sourcePacket = NULL;
    }
    audioDecoder->closeDecoder();
}
void AudioDecoder::flush()
{
    ZJAutolock lock(&mutex);
    if (sourcePacket){
        mediaPort->returnAudioEmptyPacket(sourcePacket);
        sourcePacket = NULL;
    }
    audioDecoder->flushDecoder();
}
ZJ_U32 AudioDecoder::getAudioFrame(AudioFrame *pFrame,bool rawFFPCMBuf)
{
    ZJAutolock lock(&mutex);
    ZJ_U32 ret = audioDecoder->getOutputFrame(pFrame,rawFFPCMBuf);
    if (ret != Audio_Dec_Err_None && pFrame){
        if (sourcePacket == NULL){
            ret = mediaPort->getAudioDataPacket(&sourcePacket);
        }
        if (ret == Source_Err_None && sourcePacket){
            ret = audioDecoder->setInputPacket(sourcePacket);
            if (ret != Audio_Dec_Err_KeepPkt){
                mediaPort->returnAudioEmptyPacket(sourcePacket);
                sourcePacket = NULL;
            }
            ret = audioDecoder->getOutputFrame(pFrame,rawFFPCMBuf);
        }
    }
    return ret;
}

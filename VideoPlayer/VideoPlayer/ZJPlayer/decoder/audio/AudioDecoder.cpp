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
    
}

//
//  AudioDecoder.hpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/8/31.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#ifndef AudioDecoder_hpp
#define AudioDecoder_hpp

#include <stdio.h>
#include "ZJAutolock.hpp"
#include "SourceMediaPort.hpp"

class FFmpegAudioDecoder;

class AudioDecoder{
public:
    AudioDecoder();
    ~AudioDecoder();
    int openDecoder(MediaContext* ctx,SourceMediaPort *Port);
    void closeDecoder();
private:
    SourceMediaPort *mediaPort;
    FFmpegAudioDecoder *audioDecoder;
    ZJMutex mutex;
};
#endif /* AudioDecoder_hpp */

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

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

class FFmpegAudioDecoder{
public:
    FFmpegAudioDecoder();
    ~FFmpegAudioDecoder();
    int openDecoder(MediaContext* ctx);
    
};
#endif /* FFmpegAudioDecoder_hpp */

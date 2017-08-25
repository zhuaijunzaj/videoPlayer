//
//  VideoDecoder.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef VideoDecoder_hpp
#define VideoDecoder_hpp

#include <stdio.h>
#include "FFmpegReader.hpp"
#include "ZJAutolock.hpp"

class FFmpegVideoDecoder;

class VideoDecoder{
public:
    VideoDecoder();
    ~VideoDecoder();
    int openDecoder(MediaContext* ctx);
    void closeDecoder();
private:
    FFmpegVideoDecoder *videoDecoder;
    ZJMutex mutex;
};
#endif /* VideoDecoder_hpp */

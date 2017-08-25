//
//  VideoRender.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef VideoRender_hpp
#define VideoRender_hpp

#include <stdio.h>
#include "SDL.h"
#include "ZJThreadDriver.hpp"
#include "FFmpegReader.hpp"
class VideoRender:public ZJThreadWorker{
public:
    VideoRender();
    ~VideoRender();
    int openDevice(MediaContext *mediaCtx);
    
private:
    virtual void DoRunning();
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bmp;
    SDL_Rect rect;
    
};
#endif /* VideoRender_hpp */

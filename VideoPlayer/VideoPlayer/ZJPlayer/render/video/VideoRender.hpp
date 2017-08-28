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
#include "ZJAutolock.hpp"
extern "C"{
    #import <libavutil/imgutils.h>
}

#define Video_Render_Err_None            0x00000000
#define Video_Render_Err_DeviceErr       0x0F000001

class VideoDecoder;

class VideoRender:public ZJThreadWorker{
public:
    VideoRender();
    ~VideoRender();
    int openDevice(MediaContext *mediaCtx,VideoDecoder *decoder);
    void closeDevice();
    void Run();
private:
    virtual void DoRunning();
    void drawVideoLayer();
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bmp;
    SDL_Rect rect;
    struct SwsContext *img_convert_ctx;
    MediaContext *mediaContext;
    VideoDecoder *m_decoder;
    AVFrame *frameYUV;
    AVFrame *frame;
    ZJThreadDriver *driver;
    ZJMutex videoMutex;
};
#endif /* VideoRender_hpp */

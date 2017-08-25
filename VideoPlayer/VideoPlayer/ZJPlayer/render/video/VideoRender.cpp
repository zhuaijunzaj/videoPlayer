//
//  VideoRender.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "VideoRender.hpp"
extern "C"{
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}
VideoRender::VideoRender()
{
    driver = new ZJThreadDriver(this);
    frame = av_frame_alloc();
    frameYUV = av_frame_alloc();
}
VideoRender::~VideoRender()
{
    if (driver) {
        driver->Exit();
        delete driver;
    }
    if (frameYUV) av_free(frameYUV);
    if (frame) av_free(frame);
}

int VideoRender::openDevice(MediaContext *mediaCtx)
{
    float width = mediaCtx->nVideoWidth;
    float height = mediaCtx->nVideoHeight;
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)){
        printf("SDL initialize failed:%s",SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow("FFmpeg Decode", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                            width, height);
    rect.x = 0;
    rect.y = 0;
    rect.w = width;
    rect.h = height;
    
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P,width, height,1);
    uint8_t *buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, buffer, AV_PIX_FMT_YUV420P, width, height, 1);
    return 0;
}

void VideoRender::closeDevice()
{
    if (window) SDL_DestroyWindow(window);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (bmp) SDL_DestroyTexture(bmp);
}

void VideoRender::Run()
{
    
}














































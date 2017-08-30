//
//  VideoRender.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "VideoRender.hpp"
#include "VideoDecoder.hpp"
extern "C"{
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}
VideoRender::VideoRender()
{
    driver = new ZJThreadDriver(this);
    frame = av_frame_alloc();
//    frameYUV = NULL;
}
VideoRender::~VideoRender()
{
    if (driver) {
        driver->Exit();
        delete driver;
    }
//    if (frameYUV) av_free(frameYUV);
    if (frame) av_free(frame);
}

int VideoRender::openDevice(MediaContext *mediaCtx,VideoDecoder *decoder)
{
    float width = mediaCtx->nVideoWidth;
    float height = mediaCtx->nVideoHeight;
    m_decoder = decoder;
    
    rect.x = 0;
    rect.y = 0;
    rect.w = width;
    rect.h = height;
    
    mediaContext = (MediaContext*)malloc(sizeof(MediaContext));
    memcpy(mediaContext, mediaCtx, sizeof(MediaContext));
    
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P,width, height,1);
    uint8_t *buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(frame->data, frame->linesize, buffer, AV_PIX_FMT_YUV420P, width, height, 1);
    img_convert_ctx = sws_getContext(rect.w,rect.h,
                                     (AVPixelFormat)mediaContext->pVideoDecParam->format,
                                     rect.w, rect.h,
                                     AV_PIX_FMT_YUV420P,
                                     SWS_BICUBIC, NULL, NULL, NULL);
    
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
    driver->Run();
}

void VideoRender::DoRunning()
{
//    ZJ_U32 waitTime = 35;
//    bool renderWait = false;
    ZJ_U32 nret = Video_Render_Err_None;
    
    ZJAutolock lock(&videoMutex);
    nret = m_decoder->getVideoFrame(&frameYUV);
    if (nret == Video_Render_Err_None){
//        if (&frameYUV){
//            drawVideoLayer();
//        }
        drawVideoLayer();
    }
}


void VideoRender::drawVideoLayer()
{
    sws_scale(img_convert_ctx, (const uint8_t* const*)frameYUV.data, frameYUV.linesize, 0, rect.h, frame->data, frame->linesize);
//    sws_freeContext(img_convert_ctx);
    
    SDL_UpdateYUVTexture(bmp, &rect, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bmp, &rect, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(20);
    
}










































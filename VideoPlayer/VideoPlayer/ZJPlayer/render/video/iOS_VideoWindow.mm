//
//  iOS_VideoWindow.cpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#include "iOS_VideoWindow.h"
#include "VideoWindow.h"

iOS_VideoWindow::iOS_VideoWindow():videoWindow(NULL),m_nVideoWitdh(0),m_nVideoHeight(0),m_nUserWindowWidth(0),m_nUserWindowHeight(0)
{
    
}
iOS_VideoWindow::~iOS_VideoWindow()
{
    
}
int iOS_VideoWindow::init(void *pWindow, int width, int height, MediaContext *ctx)
{
    videoWindow = (__bridge VideoWindow*)pWindow;
    m_nVideoWitdh = ctx->nVideoWidth;
    m_nVideoHeight = ctx->nVideoHeight;
    m_nUserWindowWidth = width;
    m_nUserWindowHeight = height;
    if (ctx->videoRotation == -90){
        [videoWindow setRotation:VideoRotation_Right_90];
    }else{
        [videoWindow setRotation:VideoRotation_None];
    }
    if (videoWindow != nil){
        [videoWindow setRenderParam:m_nVideoWitdh videoHeight:m_nVideoHeight userWindowWidth:m_nUserWindowWidth userWindowHeight:m_nUserWindowHeight];
    }
    return 0;
}

void iOS_VideoWindow::unInit()
{
    [videoWindow clearWindow];
    m_nVideoWitdh = 0;
    m_nVideoHeight = 0;
    m_nUserWindowWidth = 0;
    m_nUserWindowHeight = 0;
}
void iOS_VideoWindow::Flush()
{
    
}
void iOS_VideoWindow::DrawFrame(AVFrame *pFrame)
{
    if (pFrame && videoWindow){
        if(pFrame->format == AV_PIX_FMT_YUV420P)
        {
            [videoWindow drawYUV:pFrame->data[0]
                              U:pFrame->data[1]
                              V:pFrame->data[2]
                        strideY:pFrame->linesize[0]
                       strideUV:pFrame->linesize[1]
                     frameWidth:pFrame->width
                    frameHeight:pFrame->height];
        }
        else if(pFrame->format == AV_PIX_FMT_VIDEOTOOLBOX)
        {
            CVPixelBufferRef imgBuf = (CVImageBufferRef)((void*)pFrame->data[3]);
            [videoWindow drawPixelBuffer:imgBuf width:pFrame->width height:pFrame->height];
        }
    }
}
void iOS_VideoWindow::upadteVideoWindow(void *pWindow, int width, int height)
{
    videoWindow = (__bridge VideoWindow*)pWindow;
    m_nUserWindowWidth = width;
    m_nUserWindowHeight = height;
    if (videoWindow != nil){
        [videoWindow setRenderParam:m_nVideoWitdh videoHeight:m_nVideoHeight userWindowWidth:m_nUserWindowWidth userWindowHeight:m_nUserWindowHeight];
    }
}

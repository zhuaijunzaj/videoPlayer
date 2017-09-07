//
//  iOS_VideoWindow.hpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#ifndef iOS_VideoWindow_hpp
#define iOS_VideoWindow_hpp



#include <stdio.h>
#include "ZJType.h"
#include "FFmpegReader.hpp"

@class VideoWindow;

class iOS_VideoWindow{
public:
    iOS_VideoWindow();
    ~iOS_VideoWindow();
    int init(void *pWindow,int width,int height,MediaContext* ctx);
    void unInit();
    void Flush();
    void DrawFrame(AVFrame* pFrame);
    void upadteVideoWindow(void *pWindow,int width,int height);
private:
    void clipVideoDrawRect();
private:
    VideoWindow* videoWindow;
    ZJ_U32       m_nVideoWitdh;
    ZJ_U32       m_nVideoHeight;
    ZJ_U32       m_nUserWindowWidth;
    ZJ_U32       m_nUserWindowHeight;
};
#endif /* iOS_VideoWindow_hpp */

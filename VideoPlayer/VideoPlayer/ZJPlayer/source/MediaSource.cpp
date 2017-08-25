//
//  MediaSource.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "MediaSource.hpp"

MediaSource::MediaSource():running(false)
{
    reader = new FFmpegReader();
    driver = new ZJThreadDriver(this);
}

MediaSource::~MediaSource()
{
    if (reader) delete reader;
    if (driver) {
        driver->Exit();
        delete driver;
    }
}

int MediaSource::openMedia(const char *path)
{
    if (path == NULL) return -1;
    running = false;
    ZJAutolock lock(&mutex);
    int ret = reader->openMedia(path);
    if (ret == 0){
        MediaContext* ctx =reader->getMediaCtx();
        duration = (ZJ_U32)ctx->pFormatCtx->duration/1000;
    }
    return 0;
}

void MediaSource::closeMedia()
{
    running = false;
    driver->Pause();
    ZJAutolock lock(&mutex);
    reader->closeMedia();
}
MediaContext* MediaSource::getMediaCtx()
{
    return reader->getMediaCtx();
}

void MediaSource::Run()
{
    running = true;
    driver->Run();
}
int MediaSource::Seek(ZJ_U32 pos)
{
    seekPos = pos;
    ZJAutolock lock(&mutex);
    int ret = DoSeek();
    seekPos = -1;
    return ret;
}

int MediaSource::DoSeek()
{
    int ret = reader->setPlayerbackPos(seekPos);
    if (ret < 0)
        return -1;
    
    return 0;
}















































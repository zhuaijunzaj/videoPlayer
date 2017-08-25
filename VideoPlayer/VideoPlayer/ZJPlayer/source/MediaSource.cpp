//
//  MediaSource.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "MediaSource.hpp"

MediaSource::MediaSource()
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
    driver->Pause();
    ZJAutolock lock(&mutex);
    reader->closeMedia();
}
MediaContext* MediaSource::getMediaCtx()
{
    return reader->getMediaCtx();
}

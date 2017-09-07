//
//  MediaSource.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "MediaSource.hpp"
#include <unistd.h>
#include <sys/time.h>
#include "SourceMediaPort.hpp"
MediaSource::MediaSource():running(false),mEos(false),audiosourcePort(NULL),videosourcePort(NULL)
{
    reader = new FFmpegReader();
    driver = new ZJThreadDriver(this);
    pktManager = new PacketManager();
    audiosourcePort = new SourceMediaPort(this);
    videosourcePort = new SourceMediaPort(this);
}

MediaSource::~MediaSource()
{
    if (reader) delete reader;
    if (driver) {
        driver->Exit();
        delete driver;
    }
    if (pktManager) delete pktManager;
}

int MediaSource::openMedia(const char *path)
{
    if (path == NULL) return -1;
    running = false;
    mEos = false;
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
    mEos = false;
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
    mEos = false;
    ZJAutolock lock(&mutex);
    int ret = DoSeek();
    seekPos = -1;
    return ret;
}

int MediaSource::DoSeek()
{
    pktManager->clear();
    int ret = reader->setPlayerbackPos(seekPos);
    if (ret < 0)
        return -1;
    int maxtry = SRC_MAX_SEEK_TRY;
    do {
        maxtry --;
        if (pktManager->PacketQueueIsFull()) break;
        AVPacket *pkt = NULL;
        pktManager->PopEmptyDataPacket(&pkt);
        if (pkt == NULL){
            ZJ_U32 ret = reader->readPacket(pkt);
            if (ret == Source_Err_ReadAudioPkt){
                pktManager->PushAudioDataPacket(pkt);
            }else if (ret == Source_Err_ReadVideoPkt){
                pktManager->PushVideoDataPacket(pkt);
            }else if (ret == Source_Err_ReadEOS){
                running = false;
                mEos = true;
                break;
            }
        }
    } while (maxtry >= 0);
    return 0;
}

void MediaSource::DoPlay()
{
    ZJAutolock lock(&mutex);
    if (pktManager->PacketQueueIsFull()){
        usleep(SRC_WAIT_TIME*1000);
        return;
    }
    AVPacket *pkt = av_packet_alloc();
    av_init_packet(pkt);
    pktManager->PopEmptyDataPacket(&pkt);
    if (pkt != NULL){
        printf("packet is not null!\n");
        ZJ_U32 ret = reader->readPacket(pkt);
        if (ret == Source_Err_ReadAudioPkt){
            pktManager->PushAudioDataPacket(pkt);
        }else if (ret == Source_Err_ReadVideoPkt){
            pktManager->PushVideoDataPacket(pkt);
        }else if (ret == Source_Err_ReadEOS){
            running = false;
            mEos = true;
        }else{
            pktManager->PushEmptyDataPacket(pkt);
        }
    }else{
        printf("packet is null!\n");
        usleep(SRC_WAIT_TIME*1000);
    }
}
void MediaSource::DoRunning()
{
    if (running){
        DoPlay();
    }else{
        usleep(SRC_WAIT_TIME*1000);
    }
    if (mEos){
        running = false;
    }
}
ZJ_U32 MediaSource::getVideoPacket(AVPacket **pkt)
{
    ZJ_U32 ret = pktManager->PopVideoDataPacket(pkt);
    if (ret != 0){
        if (!mEos){
            
        }else{
            ret = Source_Err_ReadEOS;
        }
    }
    return ret;
}

ZJ_U32 MediaSource::getAudioPacket(AVPacket **pkt)
{
    ZJ_U32 ret = pktManager->PopAudioDataPacket(pkt);
    if (ret != 0){
        if (!mEos){
            
        }else{
            ret = Source_Err_ReadEOS;
        }
    }
    return ret;
}


SourceMediaPort* MediaSource::getAudioSourcePost()
{
    return audiosourcePort;
}

SourceMediaPort* MediaSource::getVideoSourcePort()
{
    return videosourcePort;
}







































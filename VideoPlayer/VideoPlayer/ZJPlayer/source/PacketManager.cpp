//
//  PacketManager.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "PacketManager.hpp"

PacketManager::PacketManager()
{
    ZJ_U32 audioQueueSize = AUDIO_QUEUE_MAX_SIZE;
    ZJ_U32 videoQueueSize = VIDEO_QUEUE_MAX_SIZE;
    
    audioQueue = new PacketQueue<AVPacket*>(audioQueueSize);
    videoQueue = new PacketQueue<AVPacket*>(videoQueueSize);
    emptyQueue = new PacketQueue<AVPacket*>(audioQueueSize+videoQueueSize);
    
    for (ZJ_U32 i = 0;i < audioQueueSize+videoQueueSize;++i){
        emptyQueue->Push(FFmpegReader::allocPacket());
    }
    
}
PacketManager::~PacketManager()
{
    AVPacket *pkt = NULL;
    while (emptyQueue->Pop(&pkt) == 0) {
        if (pkt){
            FFmpegReader::releasePacket(&pkt);
        }
    }
    while (audioQueue->Pop(&pkt) == 0) {
        if (pkt){
            FFmpegReader::releasePacketBuffer(pkt);
            FFmpegReader::releasePacket(&pkt);
        }
    }
    while (videoQueue->Pop(&pkt) == 0) {
        if (pkt){
            FFmpegReader::releasePacketBuffer(pkt);
            FFmpegReader::releasePacket(&pkt);
        }
    }
    if (emptyQueue) delete emptyQueue;
    if (audioQueue) delete audioQueue;
    if (videoQueue) delete videoQueue;
    
    
}
void PacketManager::clear()
{
    ZJAutolock autolock(&emptyMutex);
    AVPacket *pkt = NULL;
    ZJAutolock audiolock(&audioMutex);
    while (audioQueue->Pop(&pkt) == 0) {
        if (pkt){
            FFmpegReader::releasePacketBuffer(pkt);
            emptyQueue->Push(pkt);
        }
    }
    pkt = NULL;
    ZJAutolock videolock(&videoMutex);
    while (videoQueue->Pop(&pkt) == 0) {
        if (pkt){
            FFmpegReader::releasePacketBuffer(pkt);
            emptyQueue->Push(pkt);
        }
    }
    
}

bool PacketManager::PacketQueueIsFull() const
{
    return (videoQueue->isFull() || audioQueue->isFull());
}

bool PacketManager::PacketQueueIsEmpty() const
{
    return (videoQueue->isEmpty() && audioQueue->isEmpty());
}

int PacketManager::PushEmptyDataPacket(AVPacket *pkt)
{
    if (pkt){
        ZJAutolock lock(&emptyMutex);
        return  emptyQueue->Push(pkt);
    }
    return -1;
}
int PacketManager::PopEmptyDataPacket(AVPacket **pkt)
{
    if (pkt){
        ZJAutolock lock(&emptyMutex);
        return PopEmptyDataPacket(pkt);
    }
    return -1;
}

int PacketManager::PushAudioDataPacket(AVPacket *pkt)
{
    if (pkt){
        ZJAutolock lock(&audioMutex);
        return audioQueue->Push(pkt);
    }
    return -1;
}
int PacketManager::PopAudioDataPacket(AVPacket **pkt)
{
    if (pkt){
        ZJAutolock lock(&audioMutex);
        return audioQueue->Pop(pkt);
    }
    return -1;
}

int PacketManager::PushVideoDataPacket(AVPacket *pkt)
{
    if (pkt){
        ZJAutolock lock(&videoMutex);
        return videoQueue->Push(pkt);
    }
    return -1;
}

int PacketManager::PopVideoDataPacket(AVPacket **pkt)
{
    if (pkt){
        ZJAutolock lock(&videoMutex);
        return videoQueue->Pop(pkt);
    }
    return -1;
}





































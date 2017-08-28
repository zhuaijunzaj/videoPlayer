//
//  PacketManager.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef PacketManager_hpp
#define PacketManager_hpp

#include <stdio.h>
#include "PacketQueue.hpp"
#include "ZJAutolock.hpp"
#include "FFmpegReader.hpp"


#define AUDIO_QUEUE_MAX_SIZE  256
#define VIDEO_QUEUE_MAX_SIZE  512

class PacketManager{
public:
    PacketManager();
    ~PacketManager();
    
    void clear();
    bool PacketQueueIsFull() const;
    bool PacketQueueIsEmpty() const;
    
    int PushEmptyDataPacket(AVPacket* pkt);
    int PopEmptyDataPacket(AVPacket** pkt);
    
    int PushAudioDataPacket(AVPacket* pkt);
    int PopAudioDataPacket(AVPacket** pkt);

    int PushVideoDataPacket(AVPacket* pkt);
    int PopVideoDataPacket(AVPacket** pkt);

private:
    ZJMutex                    emptyMutex;
    PacketQueue<AVPacket*>     *emptyQueue;
    
    ZJMutex                     audioMutex;
    PacketQueue<AVPacket*>      *audioQueue;
    
    ZJMutex                     videoMutex;
    PacketQueue<AVPacket*>      *videoQueue;
};
#endif /* PacketManager_hpp */

//
//  SourceMediaPort.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/28.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "SourceMediaPort.hpp"

SourceMediaPort::SourceMediaPort(MediaSource* source):m_Source(source)
{
    
}
SourceMediaPort::~SourceMediaPort()
{
    
}

ZJ_U32 SourceMediaPort::getVideoDataPacket(AVPacket **pkt)
{
    return m_Source->getVideoPacket(pkt);
}

ZJ_U32 SourceMediaPort::returnVideoEmptyPacket(AVPacket *pkt)
{
    FFmpegReader::releasePacketBuffer(pkt);
    return m_Source->pktManager->PushEmptyDataPacket(pkt);
}

ZJ_U32 SourceMediaPort::getAudioDataPacket(AVPacket **pkt)
{
    return m_Source->getAudioPacket(pkt);
}

ZJ_U32 SourceMediaPort::returnAudioEmptyPacket(AVPacket *pkt)
{
    FFmpegReader::releasePacketBuffer(pkt);
    return m_Source->pktManager->PushEmptyDataPacket(pkt);
}

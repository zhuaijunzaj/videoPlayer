//
//  SourceMediaPort.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/28.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef SourceMediaPort_hpp
#define SourceMediaPort_hpp

#include <stdio.h>
#include "MediaSource.hpp"
class SourceMediaPort{
public:
    SourceMediaPort(MediaSource* source);
    ~SourceMediaPort();
    
    ZJ_U32 getVideoDataPacket(AVPacket** pkt);
    ZJ_U32 returnVideoEmptyPacket(AVPacket* pkt);
    
    
    ZJ_U32 getAudioDataPacket(AVPacket** pkt);
    ZJ_U32 returnAudioEmptyPacket(AVPacket* pkt);
    
private:
    MediaSource* m_Source;
};
#endif /* SourceMediaPort_hpp */

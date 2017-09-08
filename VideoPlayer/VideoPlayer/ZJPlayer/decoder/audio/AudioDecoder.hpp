//
//  AudioDecoder.hpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/8/31.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#ifndef AudioDecoder_hpp
#define AudioDecoder_hpp

#include <stdio.h>
#include "ZJAutolock.hpp"

#include "SourceMediaPort.hpp"

#define Audio_Dec_Err_None            0x00000000
#define Audio_Dec_Err_ReadEOS         0x000F0001
#define Audio_Dec_Err_OpenFaild       0x000F0002
#define Audio_Dec_Err_SkipPkt         0x000F0003
#define Audio_Dec_Err_KeepPkt         0x000F0004
#define Audio_Dec_Err_NeedNewPkt      0x000F0005
#define Audio_Dec_Err_PCMBufSmall     0x000F0006
#define Audio_Dec_Err_ReadPCMBufFaild 0x000F0007

#define AUDIO_DEC_WAIT_TIME  10

typedef struct
{
    char* pPCMBuf;
    ZJ_U32 nSamples;
    ZJ_U32 nBufSize;
    ZJ_U32 nDataSize;
    ZJ_U64 nTimestamp;
} AudioFrameBuffer;

typedef struct
{
    char* pPCMData;
    ZJ_U32 nDataSize;
    ZJ_U32 nSamples;
    ZJ_U64 nTimestamp;
} AudioFrame;


class FFmpegAudioDecoder;
class SourceMediaPort;

class AudioDecoder{
public:
    AudioDecoder();
    ~AudioDecoder();
    int openDecoder(MediaContext* ctx,SourceMediaPort *Port);
    void closeDecoder();
    void flush();
    ZJ_U32 getAudioFrame(AudioFrame* pFrame,bool rawFFPCMBuf = false);
    SourceMediaPort* getAudioPort(){return mediaPort;};
private:
    SourceMediaPort *mediaPort;
    FFmpegAudioDecoder *audioDecoder;
    AVPacket *sourcePacket;
    ZJMutex mutex;
};
#endif /* AudioDecoder_hpp */

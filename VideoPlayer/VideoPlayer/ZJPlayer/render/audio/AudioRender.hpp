//
//  AudioRender.hpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#ifndef AudioRender_hpp
#define AudioRender_hpp

#include <stdio.h>
#include "ZJAutolock.hpp"
#include "AudioDecoder.hpp"

class iOS_AudioRender;

typedef enum
{
    AudioSampleFormat_U8,
    AudioSampleFormat_S16,
    AudioSampleFormat_S32,
    AudioSampleFormat_Float,
    AudioSampleFormat_Double,
    AudioSampleFormat_U8P,
    AudioSampleFormat_S16P,
    AudioSampleFormat_S32P,
    AudioSampleFormat_FloatP,
    AudioSampleFormat_DoubleP,
    AudioSampleFormat_S64,
    AudioSampleFormat_S64P,
    AudioSampleFormat_NB,
    AudioSampleFormat_Error = -1,
} AudioSampleFormat;

#define Audio_Render_Err_None            0x00000000
#define Audio_Render_Err_DeviceErr       0x00F00001

class AudioRender{
public:
    AudioRender();
    ~AudioRender();
    
    void Run();
    void Pause();
    void Flush();
    ZJ_U32 Seek(ZJ_U32 pos,bool fastSeek = false);
    ZJ_U32 OpenDevice(MediaContext* ctx,SourceMediaPort *port,AudioDecoder *decoder);
    void closeDevice();
    void getPCMBuffer(char** poutBuf,ZJ_U32* pOutSize,ZJ_U32* pOutSamples, bool rawFFPCMBuf = false);
    AudioSampleFormat SampleFmtSwitch(int nFmtIn);
private:
    bool          m_bEOS;
    bool          m_bRunning;
    bool          m_bRawFFPcm;
    ZJ_U32        m_nLastRndTime;
    AudioDecoder  *audioDecoder;
    SourceMediaPort *m_devicePort;
    iOS_AudioRender *render;
    ZJMutex       audioMutex;
};
#endif /* AudioRender_hpp */

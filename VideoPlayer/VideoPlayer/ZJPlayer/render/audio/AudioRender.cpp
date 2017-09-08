//
//  AudioRender.cpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#include "AudioRender.hpp"
#include "iOS_AudioRender.hpp"

AudioRender::AudioRender()
{
    render = new iOS_AudioRender(this);
}
AudioRender::~AudioRender()
{
    if (render) delete render;
}
void AudioRender::Run()
{
    render->Run();
}
void AudioRender::Pause()
{
    render->Pause();
}
void AudioRender::Flush()
{
    render->Flush();
}
ZJ_U32 AudioRender::OpenDevice(MediaContext* ctx,SourceMediaPort *port,AudioDecoder *decoder)
{
    m_bEOS = false;
    m_bRunning =false;
    m_bRawFFPcm = false;
    m_devicePort = port;
    audioDecoder = decoder;
    return render->Init(ctx);
}
void AudioRender::closeDevice()
{
    m_bEOS = false;
    m_bRunning =false;
    m_bRawFFPcm = false;
    render->Uninit();
}
AudioSampleFormat AudioRender::SampleFmtSwitch(int nFmtIn)
{
    switch (nFmtIn)
    {
        case AV_SAMPLE_FMT_U8:   return AudioSampleFormat_U8;
        case AV_SAMPLE_FMT_S16:  return AudioSampleFormat_S16;
        case AV_SAMPLE_FMT_S32:  return AudioSampleFormat_S32;
        case AV_SAMPLE_FMT_FLT:  return AudioSampleFormat_Float;
        case AV_SAMPLE_FMT_DBL:  return AudioSampleFormat_Double;
        case AV_SAMPLE_FMT_U8P:  return AudioSampleFormat_U8P;
        case AV_SAMPLE_FMT_S16P: return AudioSampleFormat_S16P;
        case AV_SAMPLE_FMT_S32P: return AudioSampleFormat_S32P;
        case AV_SAMPLE_FMT_FLTP: return AudioSampleFormat_FloatP;
        case AV_SAMPLE_FMT_DBLP: return AudioSampleFormat_DoubleP;
            //        case AV_SAMPLE_FMT_S64:  return AudioSampleFormat_S64;
            //        case AV_SAMPLE_FMT_S64P: return AudioSampleFormat_S64P;
        case AV_SAMPLE_FMT_NB:   return AudioSampleFormat_NB;
    }
    return AudioSampleFormat_Error;
}


void AudioRender::getPCMBuffer(char **poutBuf, ZJ_U32 *pOutSize, ZJ_U32 *pOutSamples,bool rawFFPCMBuf)
{
    if (poutBuf && pOutSize){
        ZJ_U32 nRet = 0;
        ZJ_U32 nTrytimes = 0;
        AudioFrame audioFrame;
        m_bRawFFPcm = rawFFPCMBuf;
        do {
            audioFrame.nSamples = 0;
            audioFrame.nDataSize = 0;
            audioFrame.nTimestamp = 0;
            audioFrame.pPCMData =NULL;
            nRet = audioDecoder->getAudioFrame(&audioFrame,rawFFPCMBuf);
            if (nRet == Audio_Dec_Err_None){
                (*poutBuf) = audioFrame.pPCMData;
                (*pOutSize) = audioFrame.nDataSize;
                (*pOutSamples) = audioFrame.nSamples;
                if (!m_bRunning){
                    m_bRunning = true;
                }else if (nRet == Source_Err_ReadEOS){
                    if (!m_bEOS){
                        m_bEOS = true;
                        
                    }
                }else{
                    nTrytimes++;
                }
            }
        } while (nTrytimes < SRC_MAX_SEEK_TRY);
    }
}





























//
//  iOS_AudioRender.cpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#include "iOS_AudioRender.hpp"

iOS_AudioRender::iOS_AudioRender(AudioRender* audioRender)
{
    m_pAudioRender = audioRender;
    memset(&m_sAudioDescription, 0, sizeof(m_sAudioDescription));
    
}
iOS_AudioRender::~iOS_AudioRender()
{
    
}
int iOS_AudioRender::Init(MediaContext *ctx)
{
    m_Running = false;
    AudioSampleFormat nSampleFmt = m_pAudioRender->SampleFmtSwitch(ctx->nSampleFormat);
    m_sAudioDescription.mFramesPerPacket = 1;
    m_sAudioDescription.mSampleRate = ctx->nSampleRate;
    m_sAudioDescription.mChannelsPerFrame = ctx->nChannels;
    m_sAudioDescription.mFormatID = kAudioFormatLinearPCM;
    m_sAudioDescription.mFormatFlags = SampleFmtSwitch(nSampleFmt);
    m_sAudioDescription.mBitsPerChannel = SmpleBitSwitch(nSampleFmt);
    m_sAudioDescription.mBytesPerFrame = m_sAudioDescription.mBitsPerChannel / 8 * m_sAudioDescription.mChannelsPerFrame;
    m_sAudioDescription.mBytesPerPacket = m_sAudioDescription.mFramesPerPacket * m_sAudioDescription.mBytesPerFrame;
    
    AudioQueueNewOutput(&m_sAudioDescription, PlaySoundCallback, this, nil, nil, 0, &m_rAudioQueue);
    for(int i=0;i<QUEUE_BUFFER_COUNT;i++)
    {
        AudioQueueAllocateBuffer(m_rAudioQueue, MAX_BUFFER_SIZE, &m_sAudioQueueBuffers[i]);
    }
    for(int i = 0; i < QUEUE_BUFFER_COUNT; i++)
    {
        AudioQueueBufferRef aRef = m_sAudioQueueBuffers[i];
        aRef->mAudioDataByteSize = MUTE_SOUND_SIZE;
        memset(aRef->mAudioData, 0, MUTE_SOUND_SIZE);
        AudioQueueEnqueueBuffer(m_rAudioQueue, aRef, 0, NULL);
    }
    return Audio_Render_Err_None;
}
void iOS_AudioRender::Uninit()
{
    m_Running = false;
    memset(&m_sAudioDescription, 0, sizeof(m_sAudioDescription));
    for(int i=0;i<QUEUE_BUFFER_COUNT;i++)
    {
        AudioQueueFreeBuffer(m_rAudioQueue, m_sAudioQueueBuffers[i]);
    }
}
void iOS_AudioRender::Run()
{
    m_Running =true;
    AudioQueueStart(m_rAudioQueue, NULL);
}
void iOS_AudioRender::Pause()
{
    m_Running = false;
    AudioQueuePause(m_rAudioQueue);
}

void iOS_AudioRender::Flush()
{

    AudioQueueReset(m_rAudioQueue);
    for(int i = 0; i < QUEUE_BUFFER_COUNT; i++)
    {
        AudioQueueBufferRef aRef = m_sAudioQueueBuffers[i];
        aRef->mAudioDataByteSize = 1;
        memset(aRef->mAudioData, 0, 1);
        AudioQueueEnqueueBuffer(m_rAudioQueue, aRef, 0, NULL);
    }
}
UInt32 iOS_AudioRender::SmpleBitSwitch(AudioSampleFormat nFmtIn)
{
    switch (nFmtIn)
    {
        case AudioSampleFormat_U8:
        case AudioSampleFormat_U8P:    return 8;
        case AudioSampleFormat_S16:
        case AudioSampleFormat_S16P:   return 16;
        case AudioSampleFormat_S32:
        case AudioSampleFormat_S32P:   return 32;
        case AudioSampleFormat_Float:
        case AudioSampleFormat_Double:
        case AudioSampleFormat_FloatP: return 32;
        default: return 32;
    }
}




UInt32 iOS_AudioRender::SampleFmtSwitch(AudioSampleFormat nFmtIn)
{
    UInt32 nFormat;
    switch (nFmtIn)
    {
        case AudioSampleFormat_U8:
        case AudioSampleFormat_U8P:
            nFormat =  kAppleLosslessFormatFlag_16BitSourceData;
            break;
        case AudioSampleFormat_S16:
        case AudioSampleFormat_S16P:
            nFormat =  kLinearPCMFormatFlagIsSignedInteger;
            break;
        case AudioSampleFormat_S32:
        case AudioSampleFormat_S32P:
            nFormat = kLinearPCMFormatFlagIsSignedInteger;
            break;
        case AudioSampleFormat_Float:
        case AudioSampleFormat_FloatP:
        case AudioSampleFormat_Double:
        case AudioSampleFormat_DoubleP:
            nFormat = kLinearPCMFormatFlagIsFloat;
            break;
        default:
            nFormat = kLinearPCMFormatFlagIsFloat;
    }
    return nFormat | kAudioFormatFlagIsPacked;
}

void iOS_AudioRender::PlaySoundCallback(void* pUserData, AudioQueueRef inQueue, AudioQueueBufferRef outQueueBuf)
{
    iOS_AudioRender *pSelf = (iOS_AudioRender*)pUserData;
    
    char* pPCM = NULL;
    unsigned int nOutSize = 0;
    unsigned int nOutSamples = 0;
    Byte *pAudioQueueRndBuf = (Byte*)outQueueBuf->mAudioData;
    if(pSelf->m_Running)
    {
        pSelf->m_pAudioRender->getPCMBuffer(&pPCM, &nOutSize, &nOutSamples);
        if (pPCM && (nOutSize > 0))
        {
            memcpy(pAudioQueueRndBuf, pPCM, nOutSize);
        }
    }
    else
    {
        nOutSize = WAIT_BUF_SOUND_SIZE;
        memset(pAudioQueueRndBuf, 0, nOutSize);
    }
    outQueueBuf->mAudioDataByteSize = nOutSize;
    AudioQueueEnqueueBuffer(pSelf->m_rAudioQueue, outQueueBuf, 0, NULL);
}






















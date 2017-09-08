//
//  iOS_AudioRender.hpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#ifndef iOS_AudioRender_hpp
#define iOS_AudioRender_hpp

#define MAX_BUFFER_SIZE     352800
#define MUTE_SOUND_SIZE        128
#define QUEUE_BUFFER_COUNT       2
#define WAIT_BUF_SOUND_SIZE     64

#include <stdio.h>
#include <AudioToolbox/AudioToolbox.h>

#include "AudioRender.hpp"

class AudioRender;
class iOS_AudioRender{
public:
    iOS_AudioRender(AudioRender* audioRender);
    ~iOS_AudioRender();
    int Init(MediaContext* ctx);
    void Uninit();
    void Run();
    void Pause();
    void Flush();
private:
    UInt32 SmpleBitSwitch(AudioSampleFormat nFmtIn);
    UInt32 SampleFmtSwitch(AudioSampleFormat nFmtIn);
    static void PlaySoundCallback(void* pUserData,
                                  AudioQueueRef inQueue,
                                  AudioQueueBufferRef outQueueBuf);
private:
    bool                        m_Running;
    AudioQueueRef               m_rAudioQueue;
    AudioStreamBasicDescription m_sAudioDescription;
    AudioQueueBufferRef         m_sCurrAudioQueueBuf;
    AudioQueueBufferRef         m_sAudioQueueBuffers[QUEUE_BUFFER_COUNT];
    AudioRender*  m_pAudioRender;
};
#endif /* iOS_AudioRender_hpp */

//
//  Player.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "MediaControl.hpp"
#include "ZJAutolock.hpp"

#define Player_Err_None                 0x00000000
#define Player_Err_NotInited            0x0000A001
#define Player_Err_NoMediaOpened        0x0000A002
#define Player_Err_NoMediaPlaying       0x0000A003
#define Player_Err_NoMeidaActive        0x0000A004
#define Player_Err_MediaStillAct        0x0000A005
#define Player_Err_MediaPlaying         0x0000A006
#define Player_Err_MediaSeeking         0x0000A007
#define Player_Err_OpenFail             0x0000A008
#define Player_Err_SeekFailed           0x0000A009
#define Player_Err_OutOfDuraion         0x0000A00A
#define Player_Err_NoMediaContent       0x0000A00B
#define Player_Err_OpenAudioDecFail     0x0000A00C
#define Player_Err_OpenVideoDecFail     0x0000A00D
#define Player_Err_OpenAudioDevFail     0x0000A00E
#define Player_Err_OpenVideoDevFail     0x0000A00F
#define Player_Err_UnKnown              0xF000AFFF

typedef enum
{
    PlayerStatus_Init,
    PlayerStatus_Opened,
    PlayerStatus_Playing,
    PlayerStatus_Seeking,
    PlayerStatus_Paused,
    PlayerStatus_Closed,
    PlayerStatus_Unknown = -1,
} PlayerStatus;

class Player{
public:
    static Player* sharedInstance();
    ~Player();
    int openMedia(const char* path,int width,int height);
    void play(ZJ_U32 startPos = 0);
    
private:
    Player();
    int checkPlayerStatus(PlayerStatus nextStatus);
private:
    static Player* minstance;
    MediaControl *mediaCtl;
    PlayerStatus status;
    ZJMutex mutex;
    MediaContext mediaCtx;
};
#endif /* Player_hpp */

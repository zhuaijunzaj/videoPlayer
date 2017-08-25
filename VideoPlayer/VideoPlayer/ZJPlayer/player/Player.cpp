//
//  Player.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "Player.hpp"

Player*  Player::minstance = NULL;

Player* Player::sharedInstance()
{
    if (minstance == NULL){
        minstance = new Player();
    }
    return minstance;
}

Player::Player():status(PlayerStatus_Init)
{
    mediaCtl = new MediaControl();
}
Player::~Player()
{
    if (mediaCtl) delete mediaCtl;
    minstance = NULL;
}
int Player::openMedia(const char *path)
{
    int ret = checkPlayerStatus(PlayerStatus_Opened);
    if (ret == Player_Err_None){
        ZJAutolock lock(&mutex);
        ret = mediaCtl->openMedia(path);
        if (ret == 0){
            MediaContext *ctx = mediaCtl->getMediaCtx();
            status = PlayerStatus_Opened;
            memcpy(&mediaCtx, ctx, sizeof(MediaContext));
        }
    }
    return 0;
}
void Player::play(ZJ_U32 startPos)
{
    int ret = checkPlayerStatus(PlayerStatus_Playing);
    if (ret == Player_Err_None){
        ZJAutolock lock(&mutex);
        mediaCtl->play(startPos);
        status = PlayerStatus_Playing;
    }
}
int Player::checkPlayerStatus(PlayerStatus nextStatus)
{
    int ret = Player_Err_UnKnown;
    switch (nextStatus)
    {
        case PlayerStatus_Opened:
        {
            if((status == PlayerStatus_Init) ||
               (status == PlayerStatus_Closed) ){
                ret = Player_Err_None;
            }else if(status == PlayerStatus_Unknown){
                ret = Player_Err_NotInited;
            }else{
                ret = Player_Err_MediaStillAct;
            }
        }
            break;
        case PlayerStatus_Playing:
        {
            if((status == PlayerStatus_Opened) ||
               (status == PlayerStatus_Paused) ){
                ret = Player_Err_None;
            }else if(status == PlayerStatus_Playing){
                ret = Player_Err_MediaPlaying;
            }else{
                ret = Player_Err_NoMediaOpened;
            }
        }
            break;
        case PlayerStatus_Seeking:
        {
            if((status == PlayerStatus_Paused) ||
               (status == PlayerStatus_Playing) ){
                ret = Player_Err_None;
            }else if(status == PlayerStatus_Seeking){
                ret = Player_Err_MediaSeeking;
            }else{
                ret = Player_Err_NoMeidaActive;
            }
        }
            break;
        case PlayerStatus_Paused:
        {
            if((status == PlayerStatus_Seeking)||
               (status == PlayerStatus_Playing)||
               (status == PlayerStatus_Paused) ){
                ret = Player_Err_None;
            }else{
                ret = Player_Err_NoMediaPlaying;
            }
        }
            break;
        case PlayerStatus_Closed:
        {
            if((status == PlayerStatus_Opened) ||
               (status == PlayerStatus_Paused) ||
               (status == PlayerStatus_Seeking)||
               (status == PlayerStatus_Playing) ){
                ret = Player_Err_None;
            }else{
                ret = Player_Err_NoMeidaActive;
            }
        }
            break;
        default: break;
    }
    return ret;
}

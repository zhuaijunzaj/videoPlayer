//
//  FFmpegReader.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "FFmpegReader.hpp"

FFmpegReader::FFmpegReader():videoIndex(-1),audioIndex(-1)
{
    av_register_all();
    avformat_network_init();
    memset(&mediaCtx, 0, sizeof(MediaContext));
}

FFmpegReader::~FFmpegReader()
{
    
}

ZJ_U32 FFmpegReader::openMedia(const char *path)
{
    mediaCtx.pFormatCtx = avformat_alloc_context();
    mediaCtx.pFormatCtx->opaque = this;
    
    int ret = avformat_open_input(&mediaCtx.pFormatCtx, path, NULL, NULL);
    if (ret != 0){
        printf("open file error");
        return -1;
    }
    ret = avformat_find_stream_info(mediaCtx.pFormatCtx, NULL);
    if (ret!= 0){
        printf("find stream error");
        return -1;
    }
    videoIndex = av_find_best_stream(mediaCtx.pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (videoIndex >= 0){
        AVCodecParameters *pcodeParam = mediaCtx.pFormatCtx->streams[videoIndex]->codecpar;
        AVCodec *codec = avcodec_find_decoder(pcodeParam->codec_id);
        AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
        mediaCtx.hasVideo = true;
        mediaCtx.nVideoIndex = videoIndex;
        mediaCtx.pVideoCodecCtx = codecCtx;
        mediaCtx.pVideoDecParam = pcodeParam;
        mediaCtx.nVideoWidth = pcodeParam->width;
        mediaCtx.nVideoHeight = pcodeParam->height;
    }else{
        printf("find video error");
    }
    
    audioIndex = av_find_best_stream(mediaCtx.pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audioIndex >= 0){
        AVCodecParameters *pcodeParam = mediaCtx.pFormatCtx->streams[audioIndex]->codecpar;
        AVCodec *codec = avcodec_find_decoder(pcodeParam->codec_id);
        AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
        mediaCtx.hasAudio = true;
        mediaCtx.nAudioIndex = audioIndex;
        mediaCtx.pAudioCodecCtx = codecCtx;
        mediaCtx.pAudioDecParam = pcodeParam;
        mediaCtx.nSampleRate = pcodeParam->sample_rate;
        mediaCtx.nChannels = pcodeParam->channels;
        mediaCtx.nFrameSize = pcodeParam->frame_size;
        mediaCtx.nSampleFormat = pcodeParam->format;
    }else{
        printf("find audio error");
    }
    return 0;
}

MediaContext* FFmpegReader::getMediaCtx()
{
    return &mediaCtx;
}

void FFmpegReader::closeMedia()
{
    videoIndex = -1;
    audioIndex = -1;
    avformat_close_input(&mediaCtx.pFormatCtx);
    avcodec_free_context(&mediaCtx.pAudioCodecCtx);
    avcodec_free_context(&mediaCtx.pVideoCodecCtx);
    avformat_free_context(mediaCtx.pFormatCtx);
    memset(&mediaCtx, 0, sizeof(MediaContext));
    
}

int FFmpegReader::setPlayerbackPos(ZJ_U32 pos)
{
    if (pos > mediaCtx.nDuration) return -1;
    int ret = 0;
    int64_t timestamp = (int64_t)pos*1000;
    avformat_flush(mediaCtx.pFormatCtx);
    ret = av_seek_frame(mediaCtx.pFormatCtx, -1, timestamp, AVSEEK_FLAG_BACKWARD);
    if (ret >= 0)
        return 0;
    return -1;
}






































































//
//  FFmpegReader.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "FFmpegReader.hpp"
#include "MediaSource.hpp"
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
        AVStream *pStream = mediaCtx.pFormatCtx->streams[videoIndex];
        avcodec_parameters_to_context(codecCtx, pcodeParam);
        mediaCtx.hasVideo = true;
        mediaCtx.videoRotation = 0;
        mediaCtx.nVideoIndex = videoIndex;
        mediaCtx.pVideoCodecCtx = codecCtx;
        mediaCtx.pVideoDecParam = pcodeParam;
        mediaCtx.nVideoWidth = pcodeParam->width;
        mediaCtx.nVideoHeight = pcodeParam->height;
        
        for (ZJ_U32 i = 0;i<pStream->nb_side_data;i++){
            AVPacketSideData sd = pStream->side_data[i];
            if (sd.type == AV_PKT_DATA_DISPLAYMATRIX){
                mediaCtx.videoRotation = av_display_rotation_get((int32_t*)sd.data);
            }
        }
    }else{
        printf("find video error");
    }
    
    audioIndex = av_find_best_stream(mediaCtx.pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audioIndex >= 0){
        AVCodecParameters *pcodeParam = mediaCtx.pFormatCtx->streams[audioIndex]->codecpar;
        AVCodec *codec = avcodec_find_decoder(pcodeParam->codec_id);
        AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
        avcodec_parameters_to_context(codecCtx, pcodeParam);
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


AVPacket* FFmpegReader::allocPacket()
{
    AVPacket *pkt = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(pkt);
    return pkt;
}

void FFmpegReader::releasePacket(AVPacket **pkt)
{
    if (pkt){
        av_free(*pkt);
        pkt = NULL;
    }
}
void FFmpegReader::releasePacketBuffer(AVPacket *pkt)
{
    if (pkt){
        av_packet_unref(pkt);
        pkt = NULL;
    }
}



ZJ_U32 FFmpegReader::readPacket(AVPacket *packet)
{
    ZJ_U32 ret = Source_Err_None;
    if (packet){
        ZJ_U32 tryTimes = 0;
        while (tryTimes < MAX_READ_PACKET_RETRY) {
            int nret = av_read_frame(mediaCtx.pFormatCtx, packet);
            if (nret >= 0){
                if (packet->stream_index == audioIndex){
                    return Source_Err_ReadAudioPkt;
                }else if (packet->stream_index == videoIndex){
                    return Source_Err_ReadVideoPkt;
                }else{
                    releasePacketBuffer(packet);
                }
            }else if (nret == AVERROR_EOF){
                ret = Source_Err_ReadEOS;
                break;
            }else{
                ret = Source_Err_ReadPacketFaild;
            }
            tryTimes++;
        }
        if (tryTimes >= MAX_READ_PACKET_RETRY){
            ret = Source_Err_ReadPacketFaild;
        }
    }else{
        ret = Source_Err_ReadPacketFaild;
    }
    return ret;
}






























































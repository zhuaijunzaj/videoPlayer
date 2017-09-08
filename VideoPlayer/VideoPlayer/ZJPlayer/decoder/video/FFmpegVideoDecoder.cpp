//
//  FFmpegVideoDecoder.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/25.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "FFmpegVideoDecoder.hpp"

FFmpegVideoDecoder::FFmpegVideoDecoder()
{
    
}
FFmpegVideoDecoder::~FFmpegVideoDecoder()
{
    
}
int FFmpegVideoDecoder::openDecoder(MediaContext *mediaCtx)
{
    if (mediaCtx == NULL) return -1;
    codecCtx = mediaCtx->pVideoCodecCtx;
    codecParam = mediaCtx->pVideoDecParam;
    codecCtx->get_format = FFmpegVideoDecoder::getDecoderFormat;
    AVStream *stream = mediaCtx->pFormatCtx->streams[mediaCtx->nVideoIndex];
    timeBase = av_q2d(stream->time_base);
    AVCodec *pcodec = avcodec_find_decoder(codecCtx->codec_id);
    av_codec_set_pkt_timebase(codecCtx, codecCtx->time_base);
    if (pcodec== NULL) return -1;
    if (avcodec_open2(codecCtx, pcodec, NULL) < 0)
        return -1;
    frame = av_frame_alloc();
    if (frame == NULL)
        return -1;
    return 0;
}
void FFmpegVideoDecoder::closeDecoder()
{
    if (frame){
        av_free(frame);
        frame = NULL;
    }
    avcodec_close(codecCtx);
}
AVPixelFormat FFmpegVideoDecoder::getDecoderFormat(AVCodecContext *contex, const AVPixelFormat *formats)
{
    ZJ_U32 i = 0;
    for (i = 0; formats[i] != AV_PIX_FMT_NONE; ++i)
    {
        printf("ffmpeg supported format[ %d ]: %s", i, av_get_pix_fmt_name(formats[i]));
    }

    for (auto j = 0; formats[j] != AV_PIX_FMT_NONE; ++j)
    {
        if (formats[j] == AV_PIX_FMT_VIDEOTOOLBOX)
        {
            auto result = av_videotoolbox_default_init(contex);
            if (result < 0)
            {
                printf("av_videotoolbox_default_init failed: %s", av_err2str(result));
                return AV_PIX_FMT_YUV420P;
            }
            else
            {
                printf("hw decoder: selecting fromat AV_PIX_FMT_VIDEOTOOLBOX");
                return AV_PIX_FMT_VIDEOTOOLBOX;
            }
        }
    }

    return formats[i-1];
}

ZJ_U32 FFmpegVideoDecoder::setInputPacket(AVPacket *inputPacket)
{
    ZJ_U32 ret = avcodec_send_packet(codecCtx, inputPacket);
    if (ret == 0){
        return Video_Dec_Err_None;
    }
    if (ret == AVERROR(EAGAIN)){
        return Video_Dec_Err_KeepPkt;
    }else{
        return Video_Dec_Err_SkipPkt;
    }
}

ZJ_U32 FFmpegVideoDecoder::getOutputFrame(AVFrame **outPutFrame)
{
    ZJ_U32 ret = avcodec_receive_frame(codecCtx, frame);
    if (ret == 0){
        
        ZJ_U64 timeStamp = 0;
        if (frame->pts != AV_NOPTS_VALUE){
            timeStamp = frame->pts;
        }else if (frame->pkt_dts != AV_NOPTS_VALUE){
            timeStamp = frame->pkt_dts;
        }
        *outPutFrame = frame;
        timeStamp = timeStamp*timeBase*TIME_UNIT;
        (*outPutFrame)->opaque = &timeStamp;
    }else if (ret == AVERROR(EAGAIN)){
        ret = Video_Dec_Err_NeedNewPkt;
    }
    return ret;
}

















































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

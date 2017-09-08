//
//  FFmpegAudioDecoder.cpp
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/8/31.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#include "FFmpegAudioDecoder.hpp"

FFmpegAudioDecoder::FFmpegAudioDecoder()
{
    memset(&m_AudioFrameBuf, 0, sizeof(AudioFrame));
    
}
FFmpegAudioDecoder::~FFmpegAudioDecoder()
{
    if (m_AudioFrameBuf.nBufSize && m_AudioFrameBuf.pPCMBuf){
        free(m_AudioFrameBuf.pPCMBuf);
    }
}
int FFmpegAudioDecoder::openDecoder(MediaContext* ctx)
{
    if (ctx == NULL) return -1;
    codecCtx = ctx->pAudioCodecCtx;
    codecParam = ctx->pAudioDecParam;
    m_PlanarPCM = av_sample_fmt_is_planar(codecCtx->sample_fmt);
    m_TimeBase = av_q2d(codecCtx->time_base);
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    av_codec_set_pkt_timebase(codecCtx, codecCtx->time_base);
    if (codec == NULL) return -1;
    if (avcodec_open2(codecCtx, codec, NULL) < 0) return -1;
    audio_frame =av_frame_alloc();
    return 0;
}
void FFmpegAudioDecoder::closeDecoder()
{
    if (audio_frame){
        av_free(audio_frame);
    }
    avcodec_close(codecCtx);
    m_PlanarPCM = false;
    m_AudioFrameBuf.nSamples = 0;
    m_AudioFrameBuf.nDataSize = 0;
    m_AudioFrameBuf.nTimestamp = 0;
}

void FFmpegAudioDecoder::flushDecoder()
{
    int ret = 0;
    AVFrame *frame =av_frame_alloc();
    AVPacket *flushPkt = av_packet_alloc();
    flushPkt->size = 0;
    flushPkt->data = NULL;
    
    avcodec_send_packet(codecCtx, flushPkt);
    do {
        ret = avcodec_receive_frame(codecCtx, frame);
    } while (ret == 0);
    
    avcodec_flush_buffers(codecCtx);
    av_frame_free(&frame);
    av_packet_free(&flushPkt);
    
    m_AudioFrameBuf.nSamples = 0;
    m_AudioFrameBuf.nDataSize = 0;
    m_AudioFrameBuf.nTimestamp = 0;
}

ZJ_U32 FFmpegAudioDecoder::setInputPacket(AVPacket *inputPkt)
{
    int ret = avcodec_send_packet(codecCtx, inputPkt);
    if (ret == 0)
        return Audio_Dec_Err_None;
    if (ret == AVERROR(EAGAIN))
        return Audio_Dec_Err_KeepPkt;
    return Audio_Dec_Err_SkipPkt;
}
ZJ_U32 FFmpegAudioDecoder::getOutputFrame(AudioFrame *outputFrame,bool rawFFPCMBuf)
{
    int ret = avcodec_receive_frame(codecCtx, audio_frame);
    if (ret < 0) return -1;
    if (ret == Audio_Dec_Err_None){
        int i,j;
        int nChannels = codecParam->channels;
        int nSamples = codecParam->frame_size;
        int nSampleSize = av_get_bytes_per_sample(codecCtx->sample_fmt);
        if  (nSamples <= 0){
            nSamples = audio_frame->nb_samples;
        }
        int nFrameSize = nChannels * nSamples *nSampleSize;
        if (rawFFPCMBuf){
            if (m_AudioFrameBuf.nBufSize && m_AudioFrameBuf.pPCMBuf){
                m_AudioFrameBuf.nBufSize = 0;
                free(m_AudioFrameBuf.pPCMBuf);
                m_AudioFrameBuf.pPCMBuf = NULL;
            }
            m_AudioFrameBuf.nDataSize = nFrameSize;
            m_AudioFrameBuf.pPCMBuf = (char*)audio_frame->data;
            
        }else{
            if (!m_PlanarPCM){
                if (m_AudioFrameBuf.nBufSize && m_AudioFrameBuf.pPCMBuf){
                    m_AudioFrameBuf.nBufSize = 0;
                    free(m_AudioFrameBuf.pPCMBuf);
                    m_AudioFrameBuf.pPCMBuf = NULL;
                }
                m_AudioFrameBuf.pPCMBuf = (char*)audio_frame->data[0];
                m_AudioFrameBuf.nDataSize = audio_frame->linesize[0];
            }else{
                int nFreeSize = m_AudioFrameBuf.nBufSize - m_AudioFrameBuf.nDataSize;
                if (nFreeSize < nFrameSize){
                    int nNewBufferSize = m_AudioFrameBuf.nBufSize + (nFrameSize * 2);
                    char *pNewBuf = (char*)malloc(nNewBufferSize);
                    if (m_AudioFrameBuf.nDataSize > 0){
                        memcpy(pNewBuf, m_AudioFrameBuf.pPCMBuf, m_AudioFrameBuf.nDataSize);
                        free(m_AudioFrameBuf.pPCMBuf);
                    }
                    m_AudioFrameBuf.pPCMBuf = pNewBuf;
                    m_AudioFrameBuf.nBufSize = nNewBufferSize;
                    m_AudioFrameBuf.nDataSize = m_AudioFrameBuf.nDataSize;
                }
                for (i = 0;i<nSampleSize;i++){
                    for (j = 0;j<nChannels;j++){
                        uint8_t *dataPtr = audio_frame->data[j] + nSampleSize * i;
                        uint8_t *buffPtr = (uint8_t*)m_AudioFrameBuf.pPCMBuf + m_AudioFrameBuf.nDataSize;
                        
                        memcpy(buffPtr, dataPtr, nSampleSize);
                        m_AudioFrameBuf.nDataSize = m_AudioFrameBuf.nDataSize + nSampleSize;
                    }
                }
            }
        }
        m_AudioFrameBuf.nSamples = nSamples;
        ZJ_U64 nTimeStamp = 0;
        if (audio_frame->pts != AV_NOPTS_VALUE){
            nTimeStamp = audio_frame->pts;
        }else if (audio_frame->pkt_dts != AV_NOPTS_VALUE){
            nTimeStamp = audio_frame->pkt_dts;
        }
        m_AudioFrameBuf.nTimestamp = m_TimeBase*nTimeStamp*TIME_UNIT;
        outputFrame->pPCMData = m_AudioFrameBuf.pPCMBuf;
        outputFrame->nDataSize = m_AudioFrameBuf.nDataSize;
        outputFrame->nSamples = m_AudioFrameBuf.nSamples;
        outputFrame->nTimestamp = m_AudioFrameBuf.nTimestamp;
        
        m_AudioFrameBuf.nSamples = 0;
        m_AudioFrameBuf.nDataSize = 0;
    }
    return 0;
}


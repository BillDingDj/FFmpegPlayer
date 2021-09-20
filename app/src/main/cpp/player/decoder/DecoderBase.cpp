//
// Created by admin on 2021/9/17.
//

#include "DecoderBase.h"
#include <ctime>

void DecoderBase::Start() {
    if (m_Thread == nullptr) {
        StartDecodingThread();
    } else {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        m_Cond.notify_all();
    }
}

void DecoderBase::Pause() {
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_DecoderState = STATE_PAUSE;
}

void DecoderBase::Stop() {
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_DecoderState = STATE_STOP;
    m_Cond.notify_all();
}

void DecoderBase::SeekToPosition(float position) {
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_SeekPosition = position;
    m_DecoderState = STATE_DECODING;
    m_Cond.notify_all();
}

float DecoderBase::GetCurrentPosition() {
    return m_CurTimeStamp;
}

int DecoderBase::Init(const char *url, AVMediaType mediaType) {
    strcpy(m_Url, url);
    m_MediaType = mediaType;
    return 0;
}

void DecoderBase::UnInit() {
    if (m_Thread) {
        Stop();
        m_Thread->join();
        delete m_Thread;
        m_Thread = nullptr;
    }
}

int DecoderBase::InitFFDecoder() {
    int result = -1;
    do {
        //1.创建封装格式上下文
        m_AVFormatContext = avformat_alloc_context();

        //2.打开文件
        if(avformat_open_input(&m_AVFormatContext, m_Url, NULL, NULL) != 0)
        {
            LOGCATE("DecoderBase::InitFFDecoder avformat_open_input fail.");
            break;
        }

        //3.获取音视频流信息
        if(avformat_find_stream_info(m_AVFormatContext, NULL) < 0) {
            LOGCATE("DecoderBase::InitFFDecoder avformat_find_stream_info fail.");
            break;
        }

        //4.获取音视频流索引
        for(int i=0; i < m_AVFormatContext->nb_streams; i++) {
            if(m_AVFormatContext->streams[i]->codecpar->codec_type == m_MediaType) {
                m_StreamIndex = i;
                break;
            }
        }

        if(m_StreamIndex == -1) {
            LOGCATE("DecoderBase::InitFFDecoder Fail to find stream index.");
            break;
        }
        //5.获取解码器参数
        AVCodecParameters *codecParameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;

        //6.获取解码器
        m_AVCodec = avcodec_find_decoder(codecParameters->codec_id);
        if(m_AVCodec == nullptr) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_find_decoder fail.");
            break;
        }

        //7.创建解码器上下文
        m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
        if(avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_parameters_to_context fail.");
            break;
        }

        AVDictionary *pAVDictionary = nullptr;
        av_dict_set(&pAVDictionary, "buffer_size", "1024000", 0);
        av_dict_set(&pAVDictionary, "stimeout", "20000000", 0);
        av_dict_set(&pAVDictionary, "max_delay", "30000000", 0);
        av_dict_set(&pAVDictionary, "rtsp_transport", "tcp", 0);

        //8.打开解码器
        result = avcodec_open2(m_AVCodecContext, m_AVCodec, &pAVDictionary);
        if(result < 0) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_open2 fail. result=%d", result);
            break;
        }
        result = 0;

        m_Duration = m_AVFormatContext->duration / AV_TIME_BASE * 1000;//us to ms
        //创建 AVPacket 存放编码数据
        m_Packet = av_packet_alloc();
        //创建 AVFrame 存放解码后的数据
        m_Frame = av_frame_alloc();
    } while (false);

    if(result != 0 && m_MsgContext && m_MsgCallback)
        m_MsgCallback(m_MsgContext, -1, 0);

    return result;
}

void DecoderBase::UnInitDecoder() {
    if (m_Frame != nullptr) {
        av_frame_free(&m_Frame);
        m_Frame = nullptr;
    }

    if (m_Packet != nullptr) {
        av_packet_free(&m_Packet);
        m_Packet = nullptr;
    }

    if (m_AVCodecContext != nullptr) {
        avcodec_close(m_AVCodecContext);
        avcodec_free_context(&m_AVCodecContext);
        m_AVCodecContext = nullptr;
        m_AVCodec = nullptr;
    }

    if (m_AVFormatContext != nullptr) {
        avformat_close_input(&m_AVFormatContext);
        avformat_free_context(m_AVFormatContext);
        m_AVFormatContext = nullptr;
    }
}

void DecoderBase::StartDecodingThread() {
    m_Thread = new std::thread(DoAVDecoding, this);
}

void DecoderBase::DecodingLoop() {
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        lock.unlock();
    }

    while (true) {

        while (m_DecoderState == STATE_PAUSE) {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_Cond.wait_for(lock, std::chrono::milliseconds(10));
            m_StartTimeStamp = GetSysCurrentTime() - m_CurTimeStamp;
        }

        if (m_DecoderState == STATE_STOP) {
            break;
        }

        if (m_StartTimeStamp == -1) {
            m_StartTimeStamp = GetSysCurrentTime();
        }

        if (DecodeOnePacket() != 0) {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_DecoderState = STATE_PAUSE;
        }
    }
}

long DecoderBase::AVSync() {
    long curSysTime = GetSysCurrentTime();
    //基于系统时钟计算从开始播放流逝的时间
    long elapsedTime = curSysTime - m_StartTimeStamp;

    if(m_MsgContext && m_MsgCallback && m_MediaType == AVMEDIA_TYPE_AUDIO)
        m_MsgCallback(m_MsgContext, 2, m_CurTimeStamp * 1.0f / 1000);

    long delay = 0;

    //向系统时钟同步
    if(m_CurTimeStamp > elapsedTime) {
        //休眠时间
        auto sleepTime = static_cast<unsigned int>(m_CurTimeStamp - elapsedTime);//ms
        //限制休眠时间不能过长
        sleepTime = sleepTime > 100 ? 100 :  sleepTime;
        av_usleep(sleepTime * 1000);
    }
    delay = elapsedTime - m_CurTimeStamp;
    return 0;
}

int DecoderBase::DecodeOnePacket() {
    // 先Seek过去
    if (m_SeekPosition > 0) {
        int64_t seek_target = static_cast<int64_t>(m_SeekPosition * 1000000);
        int seek_ret = avformat_seek_file(m_AVFormatContext, -1, INT64_MIN, seek_target, INT64_MAX,
                                          0);
        if (seek_ret < 0) {
            m_SeekSuccess = false;
        } else {
            if (m_StreamIndex != -1) {
                avcodec_flush_buffers(m_AVCodecContext);
            }
            m_SeekSuccess = true;
        }
    }

    int result = av_read_frame(m_AVFormatContext, m_Packet);
    while (result == 0) {
        if (m_Packet->stream_index == m_StreamIndex) {
            if (avcodec_send_packet(m_AVCodecContext, m_Packet) == AVERROR_EOF) {
                result = -1;
                goto __EXIT;
            }

            int frameCount = 0;
            while (avcodec_receive_frame(m_AVCodecContext, m_Frame) == 0) {
                UpdateTimeStamp();
                AVSync();
                OnFrameAvailable(m_Frame);
                frameCount ++;
            }

            //判断一个 packet 是否解码完成
            if(frameCount > 0) {
                result = 0;
                goto __EXIT;
            }
        }
        av_packet_unref(m_Packet);
        result = av_read_frame(m_AVFormatContext, m_Packet);
    }

__EXIT:
    av_packet_unref(m_Packet);
    return result;
}

void DecoderBase::UpdateTimeStamp() {
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (m_Frame->pkt_dts != AV_NOPTS_VALUE) {
        m_CurTimeStamp = m_Frame->pkt_dts;
    } else if (m_Frame->pts != AV_NOPTS_VALUE) {
        m_CurTimeStamp = m_Frame->pts;
    } else {
        m_CurTimeStamp = 0;
    }

    m_CurTimeStamp = (int64_t)(m_CurTimeStamp * av_q2d(m_AVFormatContext->streams[m_StreamIndex]->time_base) * 1000);

    if (m_SeekPosition > 0 && m_SeekSuccess) {
        m_StartTimeStamp = GetSysCurrentTime() - m_CurTimeStamp;
        m_SeekPosition = 0;
        m_SeekSuccess = false;
    }
}

void DecoderBase::DoAVDecoding(DecoderBase *decoder) {

    // 开启线程不断循环调用，直到结束？什么时候结束
    do {
        if (decoder->InitFFDecoder() != 0) {
            break;
        }
        decoder->OnDecoderReady();
        decoder->DecodingLoop();
    } while (false);

    decoder->UnInitDecoder();
    decoder->OnDecoderDone();
}

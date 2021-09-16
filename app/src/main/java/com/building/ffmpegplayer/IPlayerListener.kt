package com.building.ffmpegplayer

import android.view.Surface

interface IPlayerListener {
    fun init(url: String, videoRenderType: Int, surface: Surface)
    fun play()
    fun pause()
    fun stop()
    fun destroy()
    fun seekToPosition(p: Float)
}
package com.building.ffmpegplayer;

import android.view.Surface;
import androidx.annotation.NonNull;

public class NativeBrigde implements IPlayerListener{

    static {
        String LibName = "nativeplayer";
        System.loadLibrary(LibName);

    }

    long handler = 0;

    @Override
    public void init(@NonNull String url, int videoRenderType, @NonNull Surface surface) {
        long handle = native_Init(url, videoRenderType, surface);
        if (handle != 0) {
            handler = handle;
        }
    }

    @Override
    public void play() {
        if (handler != 0) {
            native_Play(handler);
        }
    }

    @Override
    public void pause() {
        if (handler != 0) {
            native_Pause(handler);
        }
    }

    @Override
    public void stop() {
        if (handler != 0) {
            native_Stop(handler);
        }
    }

    @Override
    public void destroy() {
        if (handler != 0) {
            native_UnInit(handler);
        }
    }

    @Override
    public void seekToPosition(float p) {
        if (handler != 0) {
            native_SeekToPosition(handler, p);
        }
    }

    public void glInit() {
        native_GL_Init();
    }

    public void glUnInit() {
        native_GL_OnInit();
    }

    public void glSetImageData(int format, int width, int height, byte[] bytes) {
        native_GL_SetImageData(format, width, height, bytes);
    }

    public void glOnSurfaceCreated() {
        native_GL_OnSurfaceCreate();
    }

    public void glOnSurfaceChanged(int width, int height) {
        native_GL_OnSurfaceChanged(width, height);
    }

    public void glOnDrawFrames() {
        native_GL_OnDrawFrames();
    }

    private native long native_Init(String url, int renderType, Object surface);

    private native void native_Play(long playerHandle);

    private native void native_SeekToPosition(long playerHandle, float position);

    private native void native_Pause(long playerHandle);

    private native void native_Stop(long playerHandle);

    private native void native_UnInit(long playerHandle);

    private native void native_GL_Init();

    private native void native_GL_OnInit();

    private native void native_GL_SetImageData(int format, int width, int height, byte[] bytes);

    private native void native_GL_OnSurfaceCreate();

    private native void native_GL_OnSurfaceChanged(int width, int height);

    private native void native_GL_OnDrawFrames();

}

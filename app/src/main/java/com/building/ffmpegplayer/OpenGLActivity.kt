package com.building.ffmpegplayer

import android.opengl.GLSurfaceView
import android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class OpenGLActivity : AppCompatActivity() {
    private var mGLView: GLSurfaceView? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_open_gl_window)
        mGLView = findViewById(R.id.gl_window_view)
        mGLView?.apply {
            setEGLContextClientVersion(3)
            renderMode = RENDERMODE_CONTINUOUSLY
            val bridge = NativeBrigde()
            setRenderer(object : GLSurfaceView.Renderer {
                override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
                    bridge.glOnSurfaceCreated()
                }

                override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
                    bridge.glOnSurfaceChanged(width, height)
                }

                override fun onDrawFrame(gl: GL10?) {
                    bridge.glOnDrawFrames()
                }
            })
        }

    }
}
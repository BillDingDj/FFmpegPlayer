package com.building.ffmpegplayer

import android.os.Bundle
import android.os.Environment
import android.os.PersistableBundle
import android.view.SurfaceHolder
import android.view.SurfaceView
import androidx.appcompat.app.AppCompatActivity

class ANativePlayerActivity : AppCompatActivity() {

    companion object {
        private  var mVideoPath: String = Environment.getExternalStorageDirectory().getAbsolutePath()+"/building/one_piece.mp4"

        const val VIDEO_RENDER_OPENGL = 0
        const val VIDEO_RENDER_ANWINDOW = 1
        const val VIDEO_RENDER_3D_VR = 2
    }

    private var mSurfaceView: SurfaceView? = null

    private var mPlayer: IPlayerListener? = null

    override fun onCreate(savedInstanceState: Bundle?, persistentState: PersistableBundle?) {
        super.onCreate(savedInstanceState, persistentState)
        setContentView(R.layout.activity_a_native_player)
        mSurfaceView = findViewById(R.id.a_native_surface_view)
        initSurfaceView()
    }

    private fun initSurfaceView() {
        mSurfaceView?.apply {
            holder.addCallback(object : SurfaceHolder.Callback {
                override fun surfaceCreated(surfaceHolder: SurfaceHolder) {
                    mPlayer = FFmpegPlayer()
                    mPlayer?.init(mVideoPath, VIDEO_RENDER_ANWINDOW, surfaceHolder.getSurface())
                }

                override fun surfaceChanged(p0: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                    mPlayer?.play()
                }

                override fun surfaceDestroyed(p0: SurfaceHolder) {
                    mPlayer?.destroy()
                }

            })
        }
    }
}
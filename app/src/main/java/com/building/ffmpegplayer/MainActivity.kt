package com.building.ffmpegplayer

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    private var mANativeViewButton: TextView? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mANativeViewButton = findViewById(R.id.play_native_window)
        mANativeViewButton?.setOnClickListener {
            startActivity(Intent(this, ANativePlayerActivity::class.java))
        }
    }

    override fun onResume() {
        super.onResume()
        CommonUtils.copyAssetsDirToSDCard(this, "building", "/sdcard")
    }
}
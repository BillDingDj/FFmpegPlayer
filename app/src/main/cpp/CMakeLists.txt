cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set(jnilibs ${CMAKE_SOURCE_DIR}/../jniLibs)
set(libname nativeplayer)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/player
        ${CMAKE_SOURCE_DIR}/player/render
        ${CMAKE_SOURCE_DIR}/player/render/video
        ${CMAKE_SOURCE_DIR}/player/decoer
        ${CMAKE_SOURCE_DIR}/player/decoer/video
        ${CMAKE_SOURCE_DIR}/player
        ${CMAKE_SOURCE_DIR}/util
)

link_directories(
        ${jnilibs}/${ANDROID_ABI}
)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR} base)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/player player)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/player/render/video video_render)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/player/decoder base_decoder)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/player/decoder/video video_decoder)

add_library( # Sets the name of the library.
        ${libname}

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${base}
        ${player}
        ${video_render}
        ${base_decoder}
        ${video_decoder}
        )

set(third-party-libs
        avformat
        avcodec
        avfilter
        swresample
        swscale
        avutil
        fdk-aac
        x264
        )

set(native-libs
        android
        EGL
        GLESv3
        OpenSLES
        log
        m
        z
        )

target_link_libraries( # Specifies the target library.
        ${libname}

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${third-party-libs}
        ${native-libs}
        )
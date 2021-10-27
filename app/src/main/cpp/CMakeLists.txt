# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("a6666")
include_directories(ffmpeg.include)
set(CMAKE_CXX_FLAGS"${CMAKE_CXX_FLAGS} -std=gnu++11")
set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/../jLibs/armeabi-v7a)
set(ffmpeg_head_dir ${CMAKE_SOURCE_DIR}/ffmpeg)
include_directories(${ffmpeg_head_dir}/include)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
add_library( avutil-56
        SHARED
        IMPORTED )
set_target_properties( avutil-56
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavutil-56.so )

add_library( swresample-3
        SHARED
        IMPORTED )
set_target_properties( swresample-3
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswresample-3.so )

add_library( avcodec-58
        SHARED
        IMPORTED )
set_target_properties( avcodec-58
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavcodec-58.so )

add_library( avfilter-7
        SHARED
        IMPORTED)
set_target_properties( avfilter-7
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavfilter-7.so )

add_library( swscale-5
        SHARED
        IMPORTED)
set_target_properties( swscale-5
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswscale-5.so )

add_library( avformat-58
        SHARED
        IMPORTED)
set_target_properties( avformat-58
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavformat-58.so )

add_library( avdevice-58
        SHARED
        IMPORTED)
set_target_properties( avdevice-58
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavdevice-58.so )
add_library( postproc-55
        SHARED
        IMPORTED)
set_target_properties( postproc-55
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libpostproc-55.so )
add_library( # 参数 1 : 设置生成的动态库名称
        native-lib

        # 参数 2 : 设置生成的函数库类型 : ① 静态库 STATIC ② 动态库 SHARED
        SHARED

        # 参数 3 : 配置要编译的源文件
        native-lib.cpp
         DZAudio.cpp
        DZFFmpeg.cpp
        DZJNICall.cpp
        DZMedia.cpp
        DZPacketQueue.cpp
        DZPlayerStatus.cpp
        DZVideo.cpp
        )

# 查找代码中使用到的系统库
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# 配置目标so库编译信息





# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.

                      native-lib

                        OpenSLES
                       -WL,--start-group
                       avcodec-58
                       avdevice-58
                          avfilter-7
                       postproc-55
                      avformat-58
                      swresample-3
                      swscale-5
                      avutil-56
                      -WL,--end-group
                       android
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
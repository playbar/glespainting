cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                    -std=c++11 -fno-exceptions -Wall -fno-rtti -Wno-error=format-security \
                     ")

add_library(gles3jni SHARED
           gles3jni.cpp
           RendererES2.cpp
            )

target_link_libraries(gles3jni
                      android
                      log
                      GLESv3
                      EGL )




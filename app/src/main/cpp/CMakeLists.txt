cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -std=c++11 -fsigned-char -Wno-extern-c-compat -fexceptions")

add_definitions(-DUSE_FILE32API)


add_library(gles3jni SHARED
           gles3jni.cpp
           RendererES2.cpp
           drawtest.cpp
           math/CCAffineTransform.cpp
           math/CCGeometry.cpp
           math/CCVertex.cpp
           math/Mat4.cpp
           math/MathUtil.cpp
           math/Quaternion.cpp
           math/TransformUtils.cpp
           math/Vec2.cpp
           math/Vec3.cpp
           math/Vec4.cpp
           2d/CCNode.cpp
           2d/CCDrawLayer.cpp
           2d/CCScene.cpp
           2d/CCActionCatmullRom.cpp
           2d/CCSprite.cpp
           2d/CCMenu.cpp
           2d/CCMenuItem.cpp
           2d/CCLabelAtlas.cpp
           2d/CCAtlasNode.cpp
           base/ccTypes.cpp
           base/CCDirector.cpp
           base/CCAutoreleasePool.cpp
           base/ccFPSImages.c
           base/CCRef.cpp
           base/CCData.cpp
           base/CCScheduler.cpp
           base/CCConfiguration.cpp
           base/ccUtils.cpp
           base/CCConsole.cpp
           base/CCNinePatchImageParser.cpp
           renderer/CCTextureCache.cpp
           renderer/CCTexture2D.cpp
           renderer/ccGLStateCache.cpp
           renderer/CCGLProgram.cpp
           renderer/CCGLProgramCache.cpp
           platform/CCImage.cpp
           platform/android/CCDevice-android.cpp

           Caretaker.cpp
           LayerManager.cpp
           VisibleRect.cpp
           JniHelper.cpp
            )

 target_include_directories(gles3jni PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/external
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype2/include/android/freetype2
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/png/include/android
                           )

add_library(freetype_lib STATIC IMPORTED)
set_target_properties(freetype_lib PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype2/prebuilt/android/${ANDROID_ABI}/libfreetype.a)

add_library(png_lib STATIC IMPORTED)
set_target_properties(png_lib PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_CURRENT_SOURCE_DIR}/external/png/prebuilt/android/${ANDROID_ABI}/libpng.a)

 add_library(z_lib STATIC IMPORTED)
 set_target_properties(z_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/external/zlib/prebuilt/android/${ANDROID_ABI}/libz.a)


target_link_libraries(gles3jni
                      android
                      log
                      GLESv2
                      freetype_lib
                      png_lib
                      z_lib
                      EGL )




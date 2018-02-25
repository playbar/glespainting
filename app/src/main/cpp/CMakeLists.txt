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
           2d/CCCamera.cpp
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
           base/ccUTF8.cpp
           base/pvr.cpp
           base/etc1.cpp
           base/atitc.cpp
           base/s3tc.cpp
           base/TGAlib.cpp
           base/ZipUtils.cpp
           base/CCEventTouch.cpp
           base/CCEventDispatcher.cpp
           base/CCNinePatchImageParser.cpp
           base/CCEventListenerCustom.cpp
           base/CCEventListenerTouch.cpp
           base/CCEventListener.cpp
           base/CCEventListenerMouse.cpp
           base/CCEventListenerAcceleration.cpp
           base/CCEventListenerKeyboard.cpp
           base/CCEventListenerController.cpp
           renderer/CCTextureCache.cpp
           renderer/CCTexture2D.cpp
           renderer/ccGLStateCache.cpp
           renderer/CCGLProgram.cpp
           renderer/CCGLProgramCache.cpp
           renderer/ccShaders.cpp
           renderer/CCRenderer.cpp
           renderer/CCRenderState.cpp
           renderer/CCTrianglesCommand.cpp
           renderer/CCGLProgramState.cpp
           renderer/CCMeshCommand.cpp
           renderer/CCRenderCommand.cpp
           renderer/CCFrameBuffer.cpp
           renderer/CCBatchCommand.cpp
           renderer/CCCustomCommand.cpp
           renderer/CCGroupCommand.cpp
           renderer/CCPrimitiveCommand.cpp
           renderer/CCVertexAttribBinding.cpp
           renderer/CCTextureAtlas.cpp
           renderer/CCVertexIndexBuffer.cpp
           renderer/CCGLProgramStateCache.cpp
           renderer/CCPrimitive.cpp
           renderer/CCPass.cpp
           platform/CCImage.cpp
           platform/android/CCGLViewImpl-android.cpp
           platform/CCGLView.cpp
           platform/android/CCDevice-android.cpp
           external/ConvertUTF/ConvertUTF.c
           external/unzip/unzip.cpp
           external/unzip/ioapi.cpp
           external/unzip/ioapi_mem.cpp
           external/xxhash/xxhash.c
           Caretaker.cpp
           LayerManager.cpp
           VisibleRect.cpp
           JniHelper.cpp
            )

 target_include_directories(gles3jni PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/platform
                           ${CMAKE_CURRENT_SOURCE_DIR}/external
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/xxhash
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/unzip
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/ConvertUTF
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




cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -std=c++11 -fsigned-char -Wno-extern-c-compat -fexceptions")

add_definitions(-DUSE_FILE32API)


add_library(gles3jni SHARED
           gles3jni.cpp
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
           2d/CCRenderTexture.cpp
           2d/CCSpriteFrameCache.cpp
           2d/CCAnimationCache.cpp
           2d/CCFontFreeType.cpp
           2d/CCDrawingPrimitives.cpp
           2d/CCFontAtlasCache.cpp
           2d/CCFontFNT.cpp
           2d/CCTransition.cpp
           2d/CCActionManager.cpp
           2d/CCActionTiledGrid.cpp
           2d/CCActionGrid.cpp
           2d/CCLayer.cpp
           2d/CCComponentContainer.cpp
           2d/CCSpriteFrame.cpp
           2d/CCActionInterval.cpp
           2d/CCAction.cpp
           2d/CCAutoPolygon.cpp
           2d/CCActionInstant.cpp
           2d/CCAnimation.cpp
           2d/CCSpriteFrame.cpp
           2d/CCSpriteBatchNode.cpp
           2d/CCNodeGrid.cpp
           2d/CCGrid.cpp
           2d/CCGrabber.cpp
           2d/CCActionCamera.cpp
           2d/CCActionEase.cpp
           2d/CCFontAtlas.cpp
           2d/CCTweenFunction.cpp
           2d/CCFontCharMap.cpp
           2d/CCLabel.cpp
           2d/CCLabelTextFormatter.cpp
           2d/CCCameraBackgroundBrush.cpp
           3d/CCFrustum.cpp
           3d/CCPlane.cpp
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
           base/CCValue.cpp
           base/TGAlib.cpp
           base/CCNS.cpp
           base/ccCArray.cpp
           base/CCAsyncTaskPool.cpp
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
           base/CCEventListenerFocus.cpp
           base/CCEventCustom.cpp
           base/CCEvent.cpp
           base/base64.cpp
           base/CCTouch.cpp
           base/ObjectFactory.cpp
           base/CCUserDefault-android.cpp
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
           renderer/CCVertexIndexData.cpp
           renderer/CCPrimitive.cpp
           renderer/CCPass.cpp
           renderer/CCTextureCube.cpp
           renderer/CCQuadCommand.cpp
           platform/CCImage.cpp
           platform/CCGLView.cpp
           platform/CCFileUtils.cpp
           platform/CCSAXParser.cpp
           platform/android/CCGLViewImpl-android.cpp
           platform/android/CCDevice-android.cpp
           platform/android/CCApplication-android.cpp
           platform/android/CCFileUtils-android.cpp
           external/ConvertUTF/ConvertUTF.c
           external/unzip/unzip.cpp
           external/unzip/ioapi.cpp
           external/unzip/ioapi_mem.cpp
           external/xxhash/xxhash.c
           external/clipper/clipper.cpp
           external/md5/md5.c
           external/tinyxml2/tinyxml2.cpp
           external/edtaa3func/edtaa3func.cpp
           external/poly2tri/sweep/cdt.cc
           external/poly2tri/sweep/sweep_context.cc
           external/poly2tri/sweep/advancing_front.cc
           external/poly2tri/common/shapes.cc
           external/poly2tri/sweep/sweep.cc
           Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp
           Java_org_cocos2dx_lib_Cocos2dxHelper.cpp
           Caretaker.cpp
           LayerManager.cpp
           VisibleRect.cpp
           JniHelper.cpp
           cocos2d.cpp
            )

 target_include_directories(gles3jni PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/platform
                           ${CMAKE_CURRENT_SOURCE_DIR}/external
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/xxhash
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/unzip
                           ${CMAKE_CURRENT_SOURCE_DIR}/external/edtaa3func
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




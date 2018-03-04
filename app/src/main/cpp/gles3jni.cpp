/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <EGL/egl.h>
#include <platform/android/CCGLViewImpl-android.h>
#include <renderer/ccGLStateCache.h>
#include <renderer/CCGLProgramCache.h>
#include <2d/CCDrawingPrimitives.h>
#include <renderer/CCTextureCache.h>
#include <base/CCEventType.h>
#include <base/CCEventCustom.h>
#include <platform/CCApplication.h>
#include "base/CCEventDispatcher.h"

#include "gles3jni.h"
#include "mylog.h"
#include "base/CCDirector.h"
#include "LayerManager.h"


static cocos2d::Size designResolutionSize = cocos2d::Size(320, 480);
//static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);


JNIEXPORT void JNICALL
Java_com_haowan_openglnew_RenderLib_init(JNIEnv* env, jobject obj)
{
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview)
    {
        glview = cocos2d::GLViewImpl::create("Android app");
        glview->setFrameSize(1080, 1920);
        director->setOpenGLView(glview);


        if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
            glview = GLViewImpl::createWithRect("haowanlib", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
            glview = GLViewImpl::create("haowanlib");
#endif
            director->setOpenGLView(glview);
        }

        // turn on display FPS
        director->setDisplayStats(true);

        // set FPS. the default value is 1.0/60 if you don't call this
        director->setAnimationInterval(1.0f / 60);

        // Set the design resolution
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
        auto frameSize = glview->getFrameSize();
        // if the frame's height is larger than the height of medium size.
        if (frameSize.height > mediumResolutionSize.height)
        {
            director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
        }
            // if the frame's height is larger than the height of small size.
        else if (frameSize.height > smallResolutionSize.height)
        {
            director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
        }
            // if the frame's height is smaller than the height of medium size.
        else
        {
            director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
        }

//        // create a scene. it's an autorelease object
        auto scene = LayerManager::createScene();
//
//        // run
        director->runWithScene(scene);
        director->startAnimation();
    }
    else
    {
        cocos2d::GL::invalidateStateCache();
        cocos2d::GLProgramCache::getInstance()->reloadDefaultGLPrograms();
        cocos2d::VolatileTextureMgr::reloadAllTextures();

        cocos2d::EventCustom recreatedEvent(EVENT_RENDERER_RECREATED);
        director->getEventDispatcher()->dispatchEvent(&recreatedEvent);
        director->setGLDefaultValues();
    }
    return;
}


JNIEXPORT void JNICALL
Java_com_haowan_openglnew_RenderLib_resize(JNIEnv* env, jobject obj, jint width, jint height) {

    glViewport(0, 0, width, height);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0 );
}

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_RenderLib_step(JNIEnv* env, jobject obj)
{
    cocos2d::Director::getInstance()->mainLoop();
}


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    cocos2d::JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <EGL/egl.h>
#include "draw/painter.h"
#include "draw/layer.h"
#include "gles3jni.h"
#include "mylog.h"
#include "LayerManager.h"

extern LayerManager* g_renderer;

JNIEXPORT int JNICALL
Java_com_gles_painting_RenderLib_creatLayer(JNIEnv* env, jobject obj)
{
    return g_renderer->createLayer();
}


JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_init(JNIEnv* env, jobject obj, jint canvasWidth, jint canvasHeight,
                                         jint viewportWidth, jint viewportHeight)
{

    mh_init(canvasWidth, canvasHeight, viewportWidth, viewportHeight);
    return;
}


JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_resize(JNIEnv* env, jobject obj, jint width, jint height) {

//    glViewport(0, 0, width, height);
    mh_resize(width,height);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0 );
}

JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_step(JNIEnv* env, jobject obj)
{
//    cocos2d::Director::getInstance()->mainLoop();
}

JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_exit(JNIEnv* env, jobject obj)
{
    mh_exit();
}

JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_setTexture(JNIEnv * env, jobject obj,
                                               jint id, jint width, jint height,
                                               jintArray pixels)
{
    int size = env->GetArrayLength(pixels);
    jint *pdat = (jint*)malloc(sizeof(jint)*size);
    env->GetIntArrayRegion(pixels, 0, size, pdat);

    mh_setTexture(id, width, height, (unsigned char*)pdat);

    free(pdat);
}

JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_changeCanvas(
        JNIEnv* env, jobject obj, jint width, jint height, jint color)
{
    mh_changeCanvas(width,height,color);
}

JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_pan(JNIEnv* env, jobject obj,
                                     jfloat x0, jfloat y0, jfloat x1, jfloat y1)
{
    mh_pan(x0,y0,x1,y1);
}


JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_zoom(JNIEnv* env, jobject obj,
                                      jfloat scale, jfloat cx, jfloat cy)
{
    mh_zoom(scale,cx,cy);
}


JNIEXPORT jfloatArray JNICALL
Java_com_gles_painting_RenderLib_getCanvasCoord(JNIEnv* env, jobject obj,
                                                jfloat x, jfloat y)
{
    jfloat outCArray[] = {0.0f, 0.0f};
    Vec3 pt = mh_getCanvasCoord(x,y);
    outCArray[0] = pt.x; outCArray[1] = pt.y;
    jfloatArray outJNIArray = env->NewFloatArray(2);  // allocate
    if (NULL == outJNIArray) return NULL;
    env->SetFloatArrayRegion(outJNIArray, 0 , 2, outCArray);  // copy
    return outJNIArray;
}

JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_drawBlankCanvas(JNIEnv* env, jobject obj,
                                                 jfloat r, jfloat g, jfloat b)
{
    mh_drawBlankCanvas(r,g,b);
}


JNIEXPORT void JNICALL
Java_com_gles_painting_RenderLib_drawNormalLine(JNIEnv* env, jobject obj,
                                                jint action, jfloat x, jfloat y, jfloat width, jint color, jint textureID)
{
    LOGE("Fun:%s", __FUNCTION__);
    mh_drawNormalLine(action, x, y, width, color, textureID);
}


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{

    return JNI_VERSION_1_4;
}
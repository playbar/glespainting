
#include <jni.h>
#include <stdlib.h>
#include "painter.h"
#include "mylog.h"

extern "C" {
  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_exit(JNIEnv* env, jobject obj);

  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_init(
        JNIEnv* env, jobject obj, 
        jint canvasWidth,   jint canvasHeight, 
        jint viewportWidth, jint viewportHeight);

  JNIEXPORT void JNICALL
  Java_com_haowan_openglnew_JNILib_setTexture(JNIEnv * env, jobject obj, 
      jint id, jint width, jint height, jintArray pixels);

  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_drawBlankCanvas(JNIEnv* env, jobject obj,
        jfloat r, jfloat g, jfloat b);

  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_pan(JNIEnv* env, jobject obj,
        jfloat x0, jfloat y0, jfloat x1, jfloat y1);

  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_zoom(JNIEnv* env, jobject obj, 
        jfloat scale, jfloat cx, jfloat cy);

  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_resize(
        JNIEnv* env, jobject obj, 
        jint width, jint height);

  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_changeCanvas(
        JNIEnv* env, jobject obj, 
        jint width, jint height, jint color);


  JNIEXPORT void JNICALL
    Java_com_haowan_openglnew_JNILib_drawNormalLine(JNIEnv* env, jobject obj, 
        jint action, jfloat x, jfloat y, jfloat width, jint color, jint textureID);

  JNIEXPORT jfloatArray JNICALL
    Java_com_haowan_openglnew_JNILib_getCanvasCoord(JNIEnv* env, jobject obj, 
        jfloat x, jfloat y);
};

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_init(
    JNIEnv* env, jobject obj, 
    jint canvasWidth, jint canvasHeight, 
    jint viewportWidth, jint viewportHeight)
{
  mh_init(canvasWidth, canvasHeight, 
      viewportWidth, viewportHeight);
}
JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_exit(JNIEnv* env, jobject obj) {
  mh_exit();
}

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_setTexture(JNIEnv * env, jobject obj, 
    jint id, jint width, jint height, jintArray pixels)
{
  int size = env->GetArrayLength(pixels);
  jint *pdat = (jint*)malloc(sizeof(jint)*size);
  env->GetIntArrayRegion(pixels, 0, size, pdat);

  mh_setTexture(id, width, height, (unsigned char*)pdat);

  free(pdat);
}


JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_drawBlankCanvas(JNIEnv* env, jobject obj,
    jfloat r, jfloat g, jfloat b){
  mh_drawBlankCanvas(r,g,b);
}

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_resize(
    JNIEnv* env, jobject obj, 
    jint width, jint height) 
{
  mh_resize(width,height);
}

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_changeCanvas(
    JNIEnv* env, jobject obj, 
    jint width, jint height, jint color) 
{
  mh_changeCanvas(width,height,color);
}

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_pan(JNIEnv* env, jobject obj, 
    jfloat x0, jfloat y0, jfloat x1, jfloat y1)
{
  mh_pan(x0,y0,x1,y1);
}

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_zoom(JNIEnv* env, jobject obj, 
    jfloat scale, jfloat cx, jfloat cy)
{
  mh_zoom(scale,cx,cy);
}

JNIEXPORT void JNICALL
Java_com_haowan_openglnew_JNILib_drawNormalLine(JNIEnv* env, jobject obj,
    jint action, jfloat x, jfloat y, jfloat width, jint color, jint textureID)
{
  LOGE("Fun:%s", __FUNCTION__);
  mh_drawNormalLine(action, x, y, width, color, textureID);
}

JNIEXPORT jfloatArray JNICALL
Java_com_haowan_openglnew_JNILib_getCanvasCoord(JNIEnv* env, jobject obj, 
    jfloat x, jfloat y){
    jfloat outCArray[] = {0.0f, 0.0f};
    Vec3 pt = mh_getCanvasCoord(x,y);
    outCArray[0] = pt.x; outCArray[1] = pt.y;
    jfloatArray outJNIArray = env->NewFloatArray(2);  // allocate
    if (NULL == outJNIArray) return NULL;
    env->SetFloatArrayRegion(outJNIArray, 0 , 2, outCArray);  // copy
    return outJNIArray;
}

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

#ifndef GLES3JNI_H
#define GLES3JNI_H 1

#include <android/log.h>
#include <math.h>
#include <unistd.h>
#include <jni.h>
#include <GLES2/gl2.h>


#define DEBUG 1

#define LOG_TAG "GLES3JNI"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__ );
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

// ----------------------------------------------------------------------------
// Types, functions, and data used by both ES2 and ES3 renderers.
// Defined in gles3jni.cpp.

#define MAX_INSTANCES_PER_SIDE 4
#define MAX_INSTANCES   (MAX_INSTANCES_PER_SIDE * MAX_INSTANCES_PER_SIDE)
#define TWO_PI          (2.0 * M_PI)
#define MAX_ROT_SPEED   (0.3 * TWO_PI)

// This demo uses three coordinate spaces:
// - The model (a quad) is in a [-1 .. 1]^2 space
// - Scene space is either
//    landscape: [-1 .. 1] x [-1/(2*w/h) .. 1/(2*w/h)]
//    portrait:  [-1/(2*h/w) .. 1/(2*h/w)] x [-1 .. 1]
// - Clip space in OpenGL is [-1 .. 1]^2
//
// Conceptually, the quads are rotated in model space, then scaled (uniformly)
// and translated to place them in scene space. Scene space is then
// non-uniformly scaled to clip space. In practice the transforms are combined
// so vertices go directly from model to clip space.

struct Vertex {
    GLfloat pos[2];
    GLubyte rgba[4];
};
extern const Vertex QUAD[4];
extern const Vertex QUAD1[4];

// returns true if a GL error occurred
extern bool checkGlError(const char* funcName);
extern GLuint createShader(GLenum shaderType, const char* src);
extern GLuint createProgram(const char* vtxSrc, const char* fragSrc);
extern int GPU_Sobel();


// ----------------------------------------------------------------------------
// Interface to the ES2 and ES3 renderers, used by JNI code.

class Renderer {
public:
    virtual ~Renderer();
    void resize(int w, int h);
    void render();

protected:
    Renderer();

    // return a pointer to a buffer of MAX_INSTANCES * sizeof(vec2).
    // the buffer is filled with per-instance offsets, then unmapped.
    virtual float* mapOffsetBuf() = 0;
    virtual void unmapOffsetBuf() = 0;
    // return a pointer to a buffer of MAX_INSTANCES * sizeof(vec4).
    // the buffer is filled with per-instance scale and rotation transforms.
    virtual float* mapTransformBuf() = 0;
    virtual void unmapTransformBuf() = 0;

    virtual void draw(unsigned int numInstances) = 0;

private:
    void calcSceneParams(unsigned int w, unsigned int h, float* offsets);
    void step();

    unsigned int mNumInstances;
    float mScale[2];
    float mAngularVelocity[MAX_INSTANCES];
    uint64_t mLastFrameNs;
    float mAngles[MAX_INSTANCES];
};



#ifdef __cplusplus__
extern "C"{
#endif

extern Renderer* createES2Renderer();

JNIEXPORT void JNICALL Java_com_haowan_openglnew_RenderLib_init(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_com_haowan_openglnew_RenderLib_resize(JNIEnv* env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_com_haowan_openglnew_RenderLib_step(JNIEnv* env, jobject obj);

#ifdef __cplusplus__
}
#endif


#endif // GLES3JNI_H

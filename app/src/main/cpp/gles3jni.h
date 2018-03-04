#ifndef __GLES3JNI_H__
#define __GLES3JNI_H__

#include <android/log.h>
#include <math.h>
#include <unistd.h>
#include <jni.h>
#include <GLES2/gl2.h>


#ifdef __cplusplus
extern "C"{
#endif

JNIEXPORT void JNICALL Java_com_haowan_openglnew_RenderLib_init(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_com_haowan_openglnew_RenderLib_resize(JNIEnv* env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_com_haowan_openglnew_RenderLib_step(JNIEnv* env, jobject obj);

#ifdef __cplusplus
}
#endif


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
extern bool checkGlErrorTest(const char* funcName);
extern GLuint createShaderTest(GLenum shaderType, const char* src);
extern GLuint createProgramTest(const char* vtxSrc, const char* fragSrc);


#endif // __GLES3JNI_H__

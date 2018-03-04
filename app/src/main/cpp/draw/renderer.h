
#ifndef _RENDERER_H
#define _RENDERER_H 1

//#define IOS
#ifdef IOS
#include <OpenGLES/ES2/gl.h>
#define DEFO 1
#else
#include <GLES2/gl2.h>
#define DEFO 0
#endif

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();

    void changeCanvas();

    void resize(int w, int h);
    void draw();
    void undo();
    void redo();

    void panzoom();
    void clear(float r, float g, float b);

    void setTransformMatrix(double* mat);

    int setTexture(unsigned char* pdat, int w, int h);

    void present();

    void draw0(int us=1);
    void draw1(int us=1);
    void draw2();

    void snap1();
    void smap1(int);

public:

    GLuint mProgram;
    GLuint mVB, mVP;
    GLuint _fb0,_fb1,_fb;
    GLuint _tex0, _tex1, _tex[20];
    GLuint _st0, _dt0, _st1;
    int _fbo_width;
    int _fbo_height;

    GLuint _a0, _b0;

    int _curTexIdx;

    float transmat[16];
    float vx,vy,vw,vh;

    int _snaped;
};

#endif

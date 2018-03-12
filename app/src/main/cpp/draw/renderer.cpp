
#include "state.h"
#include "curve.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include "glsl.h"
#include "mylog.h"

extern Curve cc;
extern int g_opmode;
extern int g_pencil;
extern State st;



// returns true if a GL error occurred
bool checkGlError(const char* funcName);
GLuint createShader(GLenum shaderType, const char* src);
GLuint createProgram(const char* vtxSrc, const char* fragSrc);

static Vertex vts[4];

float *g_buf = 0; int g_bp = 0;

Renderer::Renderer() //:mEglContext(eglGetCurrentContext()),
  :mProgram(0), mVB(0), mVP(0)
{
  mProgramBlur = 0;
  _curTexIdx = -1;
  _snaped = -1;
}
Renderer::~Renderer() {
    glDeleteBuffers(1, &mVB);
    glDeleteBuffers(1, &mVP);
    glDeleteProgram(mProgram);
  }

bool Renderer::init() {
  mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
  if (!mProgram)
    return false;

  mProgramBlur = createProgram(vertexShaderprocess, fragmentShaderprocess);
  if( mProgramBlur == 0 )
    return false;

  float w = st.canvas_width;
  float h = st.canvas_height;
  _fbo_width = w;
  _fbo_height = h;

  glGenBuffers(1, &mVB);
  {
    float r = w * 0.5; float s = h * 0.5;
    vts[0].set(-r,-s, 0, 0); vts[1].set(-r, s, 0, 1);
    vts[2].set(r, -s, 1, 0); vts[3].set(r, s,  1, 1);
    glGenBuffers(1, &_a0);
    glBindBuffer(GL_ARRAY_BUFFER, _a0);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &vts[0].x, GL_STATIC_DRAW);
  }
  glGenBuffers(1, &_b0);

  //fbo0
  glGenFramebuffers(1, &_fb0);
  glBindFramebuffer(GL_FRAMEBUFFER, _fb0);
  {
    glGenTextures(1, &_tex0);
    glBindTexture(GL_TEXTURE_2D, _tex0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _fbo_width, _fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _tex0, 0);
      
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
      case GL_FRAMEBUFFER_COMPLETE:
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        //choose different formats
        printf("GL_FRAMEBUFFER_UNSUPPORTED Error.\n");
        break;
      default:
        printf("Framebuffer Error.\n");
        return false;
    }
  }
    
  //fbo1
  glGenFramebuffers(1, &_fb1);
  glBindFramebuffer(GL_FRAMEBUFFER, _fb1);
  {
    glGenTextures(1, &_tex1);
    glBindTexture(GL_TEXTURE_2D, _tex1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _fbo_width, _fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex1, 0);

    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
      case GL_FRAMEBUFFER_COMPLETE:
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        //choose different formats
        printf("GL_FRAMEBUFFER_UNSUPPORTED Error.\n");
        break;
      default:
        printf("Framebuffer Error.\n");
        return false;
    }
  }

  return true;
}

void Renderer::changeCanvas() {
  float w = st.canvas_width;
  float h = st.canvas_height;
  _fbo_width = w;
  _fbo_height = h;

  {
    float r = w * 0.5;
    float s = h * 0.5;
    vts[0].set(-r,-s, 0, 0);
    vts[1].set(-r, s, 0, 1);
    vts[2].set(r, -s, 1, 0);
    vts[3].set(r, s,  1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, _a0);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &vts[0].x, GL_STATIC_DRAW);
  }

  //fbo0
  glBindFramebuffer(GL_FRAMEBUFFER, _fb0);
  {
    glDeleteTextures(1, &_tex0);
    glGenTextures(1, &_tex0);
    glBindTexture(GL_TEXTURE_2D, _tex0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _fbo_width, _fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _tex0, 0);

    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
      case GL_FRAMEBUFFER_COMPLETE:
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        //choose different formats
        printf("GL_FRAMEBUFFER_UNSUPPORTED Error.\n");
        break;
      default:
        printf("Framebuffer Error.\n");
        return;
    }
  }
  
  //fbo1
  glBindFramebuffer(GL_FRAMEBUFFER, _fb1);
  {
    glDeleteTextures(1, &_tex1);
    glGenTextures(1, &_tex1);
    glBindTexture(GL_TEXTURE_2D, _tex1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _fbo_width, _fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex1, 0);

    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
      case GL_FRAMEBUFFER_COMPLETE:
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        //choose different formats
        printf("GL_FRAMEBUFFER_UNSUPPORTED Error.\n");
        break;
      default:
        printf("Framebuffer Error.\n");
        return;
    }
  }

  //clear canvas
  float b = ((st.bgcolor >> 8 & 0xFF)) / 255.0;
  float g = ((st.bgcolor >> 16) & 0xFF) / 255.0;
  float r = ((st.bgcolor >> 24) & 0xFF) / 255.0;
  clear(r,g,b);
}

int Renderer::setTexture(unsigned char* pdat, int w, int h){
  GLuint tid;
  glGenTextures(1,&tid);
  glBindTexture(GL_TEXTURE_2D, tid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pdat[0]);
  return tid;
}

void Renderer::setTransformMatrix(double* mat){
  for(int i=0; i<16; i++)
    transmat[i]=mat[i];
}


void Renderer::draw0(int us) {

  glBindFramebuffer(GL_FRAMEBUFFER, _fb0);
  glViewport(0, 0, _fbo_width, _fbo_height);

  Curve *pc = &cc;

  // 0 -- touch start,  a new curve
  // 1 -- touch moving, drawing
  // 2 -- touch end,    current curve ended
  if(pc->_status<0 || pc->_status>2)
    return;

  if(pc->_status == 0){  //when start, only one point
    return;
  }

  float *pos = &pc->pt2[0].x;
  int size = (int)pc->pt2.size();
  
  glUseProgram(mProgram);
  {
    GLint loc = glGetUniformLocation(mProgram, "texture");
    glUniform1i(loc, 0);

    float tm[16]; Mat4 qt;
    Mat4::createScale(2.0/_fbo_width,2.0/_fbo_height,1,&qt);
    for (int i = 0; i < 16; i++) tm[i] = qt.m[i];
    loc = glGetUniformLocation(mProgram, "mvp");
    glUniformMatrix4fv(loc, 1, GL_FALSE, tm);

    loc = glGetUniformLocation(mProgram, "color");
    glUniform4f(loc, pc->r, pc->g, pc->b, pc->a);

    loc = glGetUniformLocation(mProgram, "flag");
    glUniform1i(loc, 10);

    loc = glGetUniformLocation(mProgram, "pcl");
    glUniform1i(loc, pc->_pencil);  
  }

  { //prepare vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), pos, GL_STREAM_DRAW);
    GLint ptloc = glGetAttribLocation(mProgram, "pos");
    GLint tcloc = glGetAttribLocation(mProgram, "tcoord");
    glVertexAttribPointer(ptloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (const GLvoid *) offsetof(Vertex, x));
    glVertexAttribPointer(tcloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (const GLvoid *) offsetof(Vertex, u));
    glEnableVertexAttribArray(ptloc);
    glEnableVertexAttribArray(tcloc);
  }

  glDisable(GL_DEPTH_TEST); 

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pc->tid);

  glDrawArrays(GL_TRIANGLES, 0, size);

  glDisable(GL_BLEND);

  if (pc->_status == 2){
    pc->_status = 3;
    pc->pt0.clear();
    pc->pt2.clear();   //free memory
    if(g_opmode==6)
      return;
  }
}

//leaf pencil
void Renderer::draw1(int us)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fb1);
    glViewport(0, 0, _fbo_width, _fbo_height);
  glClearColor(1.0, 0.6, 0.6, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
    glUseProgram(mProgramBlur);

  glBindBuffer(GL_ARRAY_BUFFER, _a0);
  GLint ptloc = glGetAttribLocation(mProgramBlur, "pos");
  GLint tcloc = glGetAttribLocation(mProgramBlur, "tcoord");
  glVertexAttribPointer(ptloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const GLvoid *) offsetof(Vertex, x));
  glVertexAttribPointer(tcloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const GLvoid *) offsetof(Vertex, u));
  glEnableVertexAttribArray(ptloc);
  glEnableVertexAttribArray(tcloc);

  GLint loc = glGetUniformLocation(mProgramBlur, "mvp");
  glUniformMatrix4fv(loc, 1, GL_FALSE, transmat);

  glEnable(GL_TEXTURE_2D);

  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _tex0);
    loc = glGetUniformLocation(mProgramBlur, "texture");
    glUniform1i(loc, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  glDisableVertexAttribArray(ptloc);
  glDisableVertexAttribArray(tcloc);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D,0);

}

void Renderer::draw2( ) {
  glBindFramebuffer(GL_FRAMEBUFFER, DEFO);
  glViewport(st.view_posx, st.view_posy, st.view_width, st.view_height);

  glClearColor(1.0, 0.6, 0.6, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);

  glBindBuffer(GL_ARRAY_BUFFER, _a0);
  GLint ptloc = glGetAttribLocation(mProgram, "pos");
  GLint tcloc = glGetAttribLocation(mProgram, "tcoord");
  glVertexAttribPointer(ptloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (const GLvoid *) offsetof(Vertex, x));
  glVertexAttribPointer(tcloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (const GLvoid *) offsetof(Vertex, u));
  glEnableVertexAttribArray(ptloc);
  glEnableVertexAttribArray(tcloc);

  glUseProgram(mProgram); 
  GLint loc = glGetUniformLocation(mProgram, "mvp");
  glUniformMatrix4fv(loc, 1, GL_FALSE, transmat);

  glEnable(GL_TEXTURE_2D);

  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _tex1);
    GLint loc = glGetUniformLocation(mProgram, "flag");
    glUniform1i(loc, 2);
    loc = glGetUniformLocation(mProgram, "texture");
    glUniform1i(loc, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  glDisableVertexAttribArray(ptloc);
  glDisableVertexAttribArray(tcloc);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D,0);
}

void Renderer::snap1(){ }
void Renderer::smap1(int base) { }

void Renderer::draw() {
  if(g_opmode==0){  //drawing
      draw0();
      draw1();
      draw2();
  }
}

void Renderer::undo() { }
void Renderer::redo() { }

void Renderer::clear(float r, float g, float b){
  glBindFramebuffer(GL_FRAMEBUFFER, _fb0);
  glViewport(0, 0, _fbo_width, _fbo_height);
  glClearColor(r,g,b,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, DEFO);
  glViewport(st.view_posx, st.view_posy, st.view_width, st.view_height);

  glClearColor(0.6, 0.6, 0.6, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);

  glBindBuffer(GL_ARRAY_BUFFER, _a0);
  GLint ptloc = glGetAttribLocation(mProgram, "pos");
  GLint tcloc = glGetAttribLocation(mProgram, "tcoord");
  glVertexAttribPointer(ptloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (const GLvoid *) offsetof(Vertex, x));
  glVertexAttribPointer(tcloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (const GLvoid *) offsetof(Vertex, u));
  glEnableVertexAttribArray(ptloc);
  glEnableVertexAttribArray(tcloc);

  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _tex0);

  glUseProgram(mProgram); 
  GLint loc = glGetUniformLocation(mProgram, "mvp");
  glUniformMatrix4fv(loc, 1, GL_FALSE, transmat);
  loc = glGetUniformLocation(mProgram, "texture");
  glUniform1i(loc, 0);
  loc = glGetUniformLocation(mProgram, "flag");
  glUniform1i(loc, 2);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableVertexAttribArray(ptloc);
  glDisableVertexAttribArray(tcloc);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D,0);
}

void Renderer::panzoom() {
  glBindFramebuffer(GL_FRAMEBUFFER, DEFO);
  glViewport(st.view_posx, st.view_posy, st.view_width, st.view_height);

  glClearColor(0.6, 0.6, 0.6, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);

  glBindBuffer(GL_ARRAY_BUFFER, _a0);
  GLint ptloc = glGetAttribLocation(mProgram, "pos");
  GLint tcloc = glGetAttribLocation(mProgram, "tcoord");
  glVertexAttribPointer(ptloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (const GLvoid *) offsetof(Vertex, x));
  glVertexAttribPointer(tcloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (const GLvoid *) offsetof(Vertex, u));
  glEnableVertexAttribArray(ptloc);
  glEnableVertexAttribArray(tcloc);

  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _tex0);

  glUseProgram(mProgram); 
  GLint loc = glGetUniformLocation(mProgram, "mvp");
  glUniformMatrix4fv(loc, 1, GL_FALSE, transmat);
  loc = glGetUniformLocation(mProgram, "texture");
  glUniform1i(loc, 0);
  loc = glGetUniformLocation(mProgram, "flag");
  glUniform1i(loc, 2);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableVertexAttribArray(ptloc);
  glDisableVertexAttribArray(tcloc);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D,0);
}

bool checkGlError(const char* funcName) {
  GLint err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("GL error after %s(): 0x%08x\n", funcName, err);
    return true;
  }
  return false;
}

GLuint createShader(GLenum shaderType, const char* src) {
  GLuint shader = glCreateShader(shaderType);
  if (!shader) {
    checkGlError("glCreateShader");
    return 0;
  }
  glShaderSource(shader, 1, &src, NULL);

  GLint compiled = GL_FALSE;
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint infoLogLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
    if (infoLogLen > 0) {
      GLchar* infoLog = (GLchar*)malloc(infoLogLen);
      if (infoLog) {
        glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
        printf("Could not compile %s shader:\n%s\n",
            shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
            infoLog);
          LOGE("%s, shader:%s", shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
        free(infoLog);
      }
    }
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
  GLuint vtxShader = 0;
  GLuint fragShader = 0;
  GLuint program = 0;
  GLint linked = GL_FALSE;

  vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
  if (!vtxShader)
    goto exit;

  fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
  if (!fragShader)
    goto exit;

  program = glCreateProgram();
  if (!program) {
    checkGlError("glCreateProgram");
    goto exit;
  }
  glAttachShader(program, vtxShader);
  glAttachShader(program, fragShader);

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    printf("Could not link program");
    GLint infoLogLen = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
    if (infoLogLen) {
      GLchar* infoLog = (GLchar*)malloc(infoLogLen);
      if (infoLog) {
        glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
        printf("Could not link program:\n%s\n", infoLog);
        free(infoLog);
      }
    }
    glDeleteProgram(program);
    program = 0;
  }

exit:
  glDeleteShader(vtxShader);
  glDeleteShader(fragShader);
  return program;
}



#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

#include "state.h"
#include "curve.h"
#include "LayerManager.h"

enum OPMODE{
    OP_DRAW,    //0
    OP_PAN,
    OP_ZOOM,
    OP_CLEAR,
    OP_UNDO,
    OP_REDO,
    OP_PLAY,    //6
    OP_DRAW01,  //0
    OP_EMPTY,  //0
};

int g_opmode = OP_DRAW;
int g_pencil = 1; // 0--normal, 1--blur, 2--spray, 3--leaf
int g_width = 1;

State st;
LayerManager* g_renderer = NULL;
Curve cc;

Vec3 mh_getCanvasCoord(int x, int y){
  return st.scr2wld(x,y);
}
void mh_pan(float x0, float y0, float x1, float y1) {
  g_opmode = OP_PAN;
  st.pan(x0,y0,x1,y1);
  if(g_renderer){
    g_renderer->setTransformMatrix(st.mvp.m);
    g_renderer->panzoom();
  }
}
void mh_zoom(float k, float cx, float cy) {
  g_opmode = OP_ZOOM;
  st.zoom(k, cx, cy);
  if(g_renderer){
    g_renderer->setTransformMatrix(st.mvp.m);
    g_renderer->panzoom();
  }
}

void mh_drawBlankCanvas(float r, float g, float b){
  g_renderer->clear(r,g,b);
}

// x,y: canvas coordinate 
// color: rgba
void mh_drawNormalLine(int action, float x, float y, float size, int color, int textureID)
{
  g_opmode = OP_DRAW;
  if (action == 0)
  {
    cc.reset();
    // 0--normal, 1--blur, 2--spray, 3--leaf
    cc._pencil = 1;
    //play:3, redo:2, undo:1, draw:0, none:-1
    cc._flag = 0;
    cc.setWidth(size);
    cc.tid = st.texid[textureID];
    cc.setColor(color);

    cc.addPoint(x,y,0);
    g_renderer->draw();
  }
  else if (action == 1)
  {
    cc.addPoint(x,y,1);
    g_renderer->draw();
  }
  else if (action == 2)
  {
    cc.addPoint(x,y,2);
    g_renderer->draw();
  }
  else
    ;
}

void mh_init(float cw, float ch, float vw, float vh){
  st.init(cw,ch,vw,vh);
  g_renderer = new LayerManager;
  g_renderer->init();
  g_renderer->setTransformMatrix(st.mvp.m);
}

void mh_exit(){
  if(g_renderer) {
    delete g_renderer;
    g_renderer = 0;
  }
}

void mh_setTexture(int id, int w, int h, unsigned char* color){
  int tid = g_renderer->setTexture(color,w,h);
  st.texid[id] = tid;
}

void mh_resize(int width, int height) {
  st.resize(width,height);
  g_renderer->setTransformMatrix(st.mvp.m);
}
void mh_changeCanvas(float width, float height, int color)
{
  st.setCanvas(width,height,color);
  g_renderer->setTransformMatrix(st.mvp.m);
  g_renderer->changeCanvas();
}

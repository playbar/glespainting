
#include <math.h>
#include "vec2.h"
#include "mat4.h"
#include "state.h"

extern State st;
extern int g_opmode;
extern int g_pencil;
extern int g_width;

State::State(){}
State::~State(){}

void State::init(int cw,int ch, int vw, int vh) {
  setView(0,0,vw,vh);
  setCanvas(cw,ch,0xffffffff);
}

void State::setView(float xpos, float ypos, 
    float width, float height) 
{
  view_posx = xpos; view_posy = ypos;
  view_width = width; view_height = height;
  Mat4::createViewport(view_posx,view_posy,
      view_width,view_height, &_wndMat);
}

void State::setCanvas(float width, float height, int color)
{
  bgcolor = color;
  canvas_width = width; canvas_height = height;

  _modelMat.setIdentity();
  Mat4::createLookAt(Vec3(0,0,5),Vec3(0,0,0),
      Vec3(0,1,0),&_viewMat);
  Mat4::createOrthographic(
      canvas_width, canvas_width*view_height/view_width,
      0,10,&_projMat);

  _mvpMat = _projMat*_viewMat*_modelMat;
  mvp = mvp0 = _mvpMat;
}

void State::resize(float width, float height) {
  view_width = width; view_height = height;
  Mat4::createViewport(view_posx,view_posy,
      view_width,view_height, &_wndMat);

  Mat4::createOrthographic(
      canvas_width, canvas_width*view_height/view_width,
      0,10,&_projMat);

  _mvpMat = _projMat*_viewMat*_modelMat;
  mvp = mvp0 = _mvpMat;
}

void State::pan(float x0, float y0, float x1, float y1){
  Vec3 a = scr2wld(x0,y0);
  Vec3 b = scr2wld(x1,y1);
  Vec3 d = b-a;
  d *= _modelMat[5];

  Mat4 mt;
  Mat4::createTranslation(d.x,d.y,0,&mt);

  _modelMat = mt*_modelMat;
  _mvpMat = _projMat*_viewMat*_modelMat;
  mvp = _mvpMat;
}

void State::zoom(float k, float cx, float cy) {
  Vec3 ct = scr2wld(cx,cy);
  float t = ct.x;
  float a=_modelMat[0]; float b=_modelMat[12];
  float dx = (1-k)*a*t + b;

  a=_modelMat[5]; b=_modelMat[13];
  t = ct.y;
  float dy = (1-k)*a*t + b;

  Mat4 m1,m2;
  Mat4::createScale(k*a,k*a,1,&m1);
  Mat4::createTranslation(dx,dy,0,&m2);

  _modelMat = m2*m1;
  _mvpMat = _projMat*_viewMat*_modelMat;
   mvp = _mvpMat;
}

Vec3 State::scr2wld(float x, float y){
  y = view_height - y; 
  Mat4 inv = _wndMat*_mvpMat;
  inv = inv.getInversed();

  Vec4 p0 = inv*Vec4(x,y,0,1);
  Vec4 p1 = inv*Vec4(x,y,1,1);
  p0 = p0/p0.w;  p1 = p1/p1.w;
  double k = 1 - p0.z/p1.z;
  double xx = p0.x+k*(p1.x-p0.x);
  double yy = p0.y+k*(p1.y-p0.y);
  return Vec3(xx,yy,0);
}

Vec2 State::wld2scr(float x, float y, float z){
  Vec4 dpt = _wndMat*_mvpMat*Vec4(x,y,z,1);
  return Vec2(dpt.x,view_height-dpt.y);
}


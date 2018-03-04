
#pragma once

#include <map>
#include "vec2.h"
#include "mat4.h"

class State{
  public:
    State();
    ~State();

    void init(int cw,int ch, int vw, int vh);

    void resize(float width, float height);

    void setView(float xpos,float ypos,float width, float height);
    void setCanvas(float width, float height, int color);

    void pan(float x0, float y0, float x1, float y1);
    void zoom(float k, float cx, float cy);
    
    Vec3 scr2wld(float x, float y);
    Vec2 wld2scr(float x, float y, float z);

  public:
    float view_posx, view_posy, view_width, view_height;
    float canvas_width, canvas_height;
    int bgcolor;

    Mat4 ortho, view, mvp, mvp0;
    Mat4 vpmat;  //viewport matrix

    std::map<int, int> texid;
    int snapCount, drawCount;

    Mat4 _modelMat, _viewMat, _projMat, _wndMat;
    Mat4 _mvpMat;
};

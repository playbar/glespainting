
#pragma once

#include <math.h>
#include <vector>
using namespace std;

struct Point2F{
  Point2F():x(0),y(0){}
  Point2F(float ax, float ay):x(ax),y(ay){}
  void set(float ax, float ay){ x=ax; y=ay; }
  float distance(const Point2F& p){
    float dx = x-p.x;
    float dy = y-p.y;
    return sqrt(dx*dx+dy*dy);
  }
  float x,y;
};

struct Vertex{
  float x,y;
  float u,v;
  void set(float ax, float ay, float au, float av){
    x=ax; y=ay; u=au; v=av;
  }
};

#define SCALE 0.6f

class Curve{
  public:
    Curve();
    void reset();
    void addPoint(float x, float y, int state);
    void setWidth(float w){ _w = w; }
    void setColor(int rgba);

    void draw();

  private:
    void update();

    void getBesierPointsArray(Point2F p1, Point2F p2, Point2F p3,
        Point2F control, Point2F control2, float size, bool isInit);

  public:
    vector<Point2F> pt0;
    vector<Vertex> pt2;

    float _x0, _y0, _x1, _y1;  //bounding

    Point2F beforeLastPoint, c2;
    int pointDistance;  // 点距
    bool isInit;

    float r,g,b,a;
    int _pencil;
    float _w;
    int tid; //texture id
    int _status;  //0--start, 1--drawing, 2--end
    int _flag;  //play:3,redo:2,undo:1,draw:0,none:-1
};

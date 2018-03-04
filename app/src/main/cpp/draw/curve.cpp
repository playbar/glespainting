
#include "state.h"
#include "curve.h"
#include "vec3.h"
#include <stdlib.h>
#include <float.h>

Curve::Curve(){ reset(); }

void Curve::reset() {
  pt0.clear();  pt2.clear();
  _status = -1;  _flag = -1; _pencil = -1;

  c2.set(-1,-1);
  beforeLastPoint.set(0,0);
  pointDistance = 6;
  isInit = true;

  _x0 = _y0 = FLT_MAX;
  _x1 = _y1 = FLT_MIN;
}

void Curve::setColor(int color){
  a = (color & 0xFF) / 255.0;
  b = ((color >> 8 & 0xFF)) / 255.0;
  g = ((color >> 16) & 0xFF) / 255.0;
  r = ((color >> 24) & 0xFF) / 255.0;
}

void Curve::addPoint(float x, float y, int status){
  _status = status;
  pt0.push_back(Point2F(x,y));

  if(x>_x1) _x1=x;  if(x<_x0) _x0=x;
  if(y>_y1) _y1=y;  if(y<_y0) _y0=y;

  update();
}

void Curve::update(){
  int size = (int)pt0.size();
  if(size<3)
    return;
  else if(size==3)
    isInit = true;
  else
    isInit = false;

  Point2F p3 = pt0[size-1];
  Point2F p2 = pt0[size-2];
  Point2F p1 = pt0[size-3];
  Point2F m1((p1.x+p2.x)/2,(p1.y+p2.y)/2);
  Point2F m2((p3.x+p2.x)/2,(p3.y+p2.y)/2);
  Point2F mm((m1.x+m2.x)/2,(m1.y+m2.y)/2);
  
  //中点的中点与中间点的x,y偏移值
  float offsetx = p2.x - mm.x;
  float offsety = p2.y - mm.y;
  Point2F control1(m1.x+offsetx, m1.y+offsety);//第一控制点
  Point2F control2(m2.x+offsetx, m2.y+offsety);//第二控制点
  control1.x = p2.x+((control1.x-p2.x)*SCALE);
  control1.y = p2.y+((control1.y-p2.y)*SCALE);
  control2.x = p2.x+((control2.x-p2.x)*SCALE);
  control2.y = p2.y+((control2.y-p2.y)*SCALE);
	 
  if(beforeLastPoint.x != 0 || beforeLastPoint.y != 0){
    p1.set(beforeLastPoint.x, beforeLastPoint.y);
  }
  //调用获取贝塞尔曲线点方法
  if(c2.x == -1 && c2.y == -1)
    getBesierPointsArray(p1, p2, p3, control1, control1, _w, isInit);
  else
    getBesierPointsArray(p1, p2, p3, c2, control1, _w, isInit);
  
  c2.x = control2.x;
  c2.y = control2.y;
}

/**
 * 三次贝塞尔曲线x值  
 * 公式：c = c1*u*u*u + 3*c2*u*u*(1-u) 
 *           + 3*c3*u*(1-u)*(1-u) + c4*(1-u)*(1-u)*(1-u)
 */
float bezier3funcX(float u3, float u2_u, 
    float _u2u, float _u3, Point2F* controlP)
{  
  float part0 =     controlP[0].x *  u3;       //uu * uu * uu;  
  float part1 = 3 * controlP[1].x *  u2_u;  
  float part2 = 3 * controlP[2].x * _u2u;  
  float part3 =     controlP[3].x * _u3;     
  return part0 + part1 + part2 + part3;   
} 

/**
 * 三次贝塞尔曲线y值  
 * 公式：c = c1*u*u*u + 3*c2*u*u*(1-u) 
 *           +3*c3*u*(1-u)*(1-u) + c4*(1-u)*(1-u)*(1-u)
 */
float bezier3funcY(float u3,float u2_u,
    float _u2u, float _u3, Point2F* controlP)
{  
  float part0 =     controlP[0].y *  u3;  
  float part1 = 3 * controlP[1].y *  u2_u;  
  float part2 = 3 * controlP[2].y * _u2u;  
  float part3 =     controlP[3].y * _u3;     
  return part0 + part1 + part2 + part3;   
}

// 贝塞尔解析
void Curve::getBesierPointsArray(Point2F p1, Point2F p2, Point2F p3,
    Point2F control, Point2F control2, float size, bool isInit)
{
  float size1 = size;
  /* 柳叶笔时画笔尺寸固定为10
   *if(paintMode == PaintMode.PAINT_MODE_LEAF){
   *  size1 = UIHelper.LEAF_LINE_SIZE;
   *}
   *if(size1 < min_bezier_size){
   *  size1 = min_bezier_size;
   *}
   */
  if (size1 < 1) size1 = 1;

  if (p1.x == control.x && p1.y == control.y) {
    control.x = (p1.x + p2.x) / 2;
    control.y = (p1.y + p2.y) / 2;
    control2 = control;
  }

  Point2F controlPoint[4];
  controlPoint[0] = p1;       // 起点
  controlPoint[1] = control;  // 控制点
  controlPoint[2] = control2; // 控制点
  controlPoint[3] = p2;       // 终点
  float u = 1;
  if (isInit) {
    //pt0.clear();
    //pt2.clear();
  }
  // u的步长决定了曲线点的精度
  double distance = p1.distance(p2);// 两点距离
  double pointNum = distance / size1 * 20;// 点个数  //12
  if (pointNum <= 0) {
    pointNum = 1;
  }
  double uLength = 1.0 / pointNum;
  //setPD(size1,paintMode);
  if (size1 < 32) {
    pointDistance = 50;   //8
  } else {
    pointDistance = 12;
  }

  if(_flag==0){ //draw, not play/undo/redo
    if(_pencil!=3) //not leaf
      pt2.clear();
  }

  while (u >= 0) {
    float _1u = 1 - u;
    float u2 = u * u;
    float _u2 = _1u * _1u;
    float u3 = u2 * u;
    float u2_u = u2 * _1u;
    float _u2u = _u2 * u;
    float _u3 = _u2 * _1u;
    float px = bezier3funcX(u3, u2_u, _u2u, _u3, controlPoint);
    float py = bezier3funcY(u3, u2_u, _u2u, _u3, controlPoint);
    Point2F tempP(px, py);
    if (beforeLastPoint.distance(tempP) >= size1 / pointDistance) 
    {
      float r = _w*0.5f;
      Point2F pt = tempP;
      Vertex vt0,vt1,vt2,vt3;

      vt0.x = vt3.x = pt.x - r;
      vt1.x = vt2.x = pt.x + r;
      vt0.y = vt1.y = pt.y - r;
      vt2.y = vt3.y = pt.y + r;

      vt0.u = vt3.u = 0.0;
      vt1.u = vt2.u = 1.0;
      vt0.v = vt1.v = 0.0;
      vt2.v = vt3.v = 1.0;

      if(_pencil!=3) {  //not leaf pencil
        pt2.push_back(vt0); pt2.push_back(vt1); pt2.push_back(vt2);
        pt2.push_back(vt0); pt2.push_back(vt2); pt2.push_back(vt3);
      }
      else {
        vt0.x = pt.x; vt0.y = pt.y;
        pt2.push_back(vt0); 
      }
      beforeLastPoint = tempP;
    }
    //u的步长决定曲线的疏密
    u -= uLength;
  }

  /**若只获得一个贝塞尔点，则添加最后的点
   *if(paintMode == PaintMode.PAINT_MODE_OIL && pt1.size() == 1){
   *  pt1.remove(0);
   *  pt1.add(p3);
   *}
   */
}

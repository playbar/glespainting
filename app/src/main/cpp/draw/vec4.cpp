
#include <math.h>
#include "vec4.h"

Vec4::Vec4():x(0.0f),y(0.0f),z(0.0f),w(0.0f){ }
Vec4::Vec4(double xx, double yy, double zz, double ww):x(xx),y(yy),z(zz),w(ww){ }
Vec4::Vec4(Vec3 v3, double ww){ x=v3.x; y=v3.y; z=v3.z; w=ww; }
Vec4::Vec4(const Vec4& copy){ x=copy.x; y=copy.y; z=copy.z; w=copy.w; }
const Vec4& Vec4::operator=(const Vec4& a) { x=a.x; y=a.y; z=a.z; z=a.z; return *this; }

void Vec4::set(double ax, double ay, double az, double aw){
  x = ax; y = ay;
  z = az; w = aw;
}

Vec4 Vec4::operator+(const Vec4& a) const { return Vec4(x+a.x,y+a.y,z+a.z,w+a.w); }
Vec4 Vec4::operator-(const Vec4& a) const { return Vec4(x-a.x,y-a.y,z-a.z,w-a.w); }
Vec4 Vec4::operator*(double k) const {return Vec4(x*k,y*k,z*k,w*k);}
Vec4 Vec4::operator/(double k) const {return Vec4(x/k,y/k,z/k,w/k);}
Vec4 Vec4::operator*=(double k){ x*=k; y*=k; z*=k; w*=k; return *this; }
Vec4 Vec4::operator/=(double k){ k=1/k; x*=k; y*=k; z*=k; w*=k; return *this; }
Vec4 Vec4::operator-(void) const {return Vec4(-x,-y,-z,-w);}

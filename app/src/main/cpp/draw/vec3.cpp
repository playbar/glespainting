
#include <math.h>
#include "vec3.h"

Vec3::Vec3():x(0.0f),y(0.0f),z(0.0f){ }
Vec3::Vec3(double xx, double yy, double zz):x(xx),y(yy),z(zz){ }
Vec3::Vec3(const Vec3& copy){ x=copy.x; y=copy.y; z=copy.z; }
const Vec3& Vec3::operator=(const Vec3& a) { x=a.x; y=a.y; z=a.z; return *this; }

void Vec3::set(double ax, double ay, double az){
  x = ax;
  y = ay;
  z = az;
}

double Vec3::length() const { return sqrt(x*x + y*y + z*z); }
double Vec3::length2() const { return x*x + y*y + z*z; }
void Vec3::normalize(){
  double n = x * x + y * y + z * z;
  n = sqrt(n);
  if(n < 0.000000001) //Too close to zero.
    return;

  n = 1.0f / n;
  x *= n;
  y *= n;
  z *= n;
}

double Vec3::dot(const Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
Vec3 Vec3::cross(const Vec3& a) const {
  double xx = y*a.z - z*a.y;
  double yy = z*a.x - x*a.z;
  double zz = x*a.y - y*a.x;
  return Vec3(xx,yy,zz);
}

double Vec3::operator*(const Vec3& a) const{  //dot product
  return x*a.x + y*a.y + z*a.z; 
}
Vec3 Vec3::operator^(const Vec3& a) const{    //cross product
  double xx = y*a.z - z*a.y;
  double yy = z*a.x - x*a.z;
  double zz = x*a.y - y*a.x;
  return Vec3(xx,yy,zz);
}

Vec3 Vec3::operator+(const Vec3& a) const { return Vec3(x+a.x,y+a.y,z+a.z); }
Vec3 Vec3::operator-(const Vec3& a) const { return Vec3(x-a.x,y-a.y,z-a.z); }
Vec3 Vec3::operator*(double k) const {return Vec3(x*k,y*k,z*k);}
Vec3 Vec3::operator/(double k) const {return Vec3(x/k,y/k,z/k);}
Vec3 Vec3::operator*=(double k){ x*=k; y*=k; z*=k; return *this; }
Vec3 Vec3::operator/=(double k){ k=1/k; x*=k; y*=k; z*=k; return *this; }
Vec3 Vec3::operator-(void) const {return Vec3(-x,-y,-z);}

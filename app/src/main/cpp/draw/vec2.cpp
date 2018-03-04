
#include "vec2.h"

Vec2::Vec2():x(0.0f),y(0.0f){ }
Vec2::Vec2(double xx, double yy):x(xx),y(yy){ }
Vec2::Vec2(const Vec2& copy){ x=copy.x; y=copy.y; }
const Vec2& Vec2::operator=(const Vec2& a) { x=a.x; y=a.y; return *this; }

void Vec2::set(double ax, double ay){ x = ax; y = ay; }

Vec2 Vec2::operator+(const Vec2& a) const { return Vec2(x+a.x,y+a.y); }
Vec2 Vec2::operator-(const Vec2& a) const { return Vec2(x-a.x,y-a.y); }
Vec2 Vec2::operator*(double k) const {return Vec2(x*k,y*k);}
Vec2 Vec2::operator/(double k) const {return Vec2(x/k,y/k);}
Vec2 Vec2::operator*=(double k){ x*=k; y*=k; return *this; }
Vec2 Vec2::operator/=(double k){ k=1/k; x*=k; y*=k; return *this; }
Vec2 Vec2::operator-(void) const {return Vec2(-x,-y);}

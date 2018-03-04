#ifndef _VEC4_H_
#define _VEC4_H_ 1

#include "vec3.h"

class Vec4
{
  public:

    double x,y,z,w;

    Vec4();
    Vec4(double xx, double yy, double zz, double ww);
    Vec4(Vec3 v3, double ww);
    Vec4(const Vec4& copy);
    const Vec4& operator=(const Vec4& a);

    void set(double ax, double ay, double az, double aw);

    Vec4 operator+(const Vec4& a) const;
    Vec4 operator-(const Vec4& a) const;
    Vec4 operator*(double k) const;
    Vec4 operator/(double k) const;
    Vec4 operator*=(double k);
    Vec4 operator/=(double k);
    Vec4 operator-(void) const;
};

#endif

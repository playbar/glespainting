#ifndef _VEC3_H_
#define _VEC3_H_ 1

class Vec3
{
  public:

    double x,y,z;

    Vec3();
    Vec3(double xx, double yy, double zz);
    Vec3(const Vec3& copy);
    const Vec3& operator=(const Vec3& a);

    void set(double ax, double ay, double az);

    double length() const;
    double length2() const;
    void normalize();

    double dot(const Vec3& v) const;
    Vec3 cross(const Vec3& v) const;
    double operator*(const Vec3& a) const;  //dot product
    Vec3 operator^(const Vec3& a) const;    //cross product

    Vec3 operator+(const Vec3& a) const;
    Vec3 operator-(const Vec3& a) const;
    Vec3 operator*(double k) const;
    Vec3 operator/(double k) const;
    Vec3 operator*=(double k);
    Vec3 operator/=(double k);
    Vec3 operator-(void) const;
};

#endif

#ifndef _Vec2_H_
#define _Vec2_H_ 1


class Vec2
{
  public:

    double x,y;

    Vec2();
    Vec2(double xx, double yy);
    Vec2(const Vec2& copy);
    const Vec2& operator=(const Vec2& a);

    void set(double ax, double ay);

    Vec2 operator+(const Vec2& a) const;
    Vec2 operator-(const Vec2& a) const;
    Vec2 operator*(double k) const;
    Vec2 operator/(double k) const;
    Vec2 operator*=(double k);
    Vec2 operator/=(double k);
    Vec2 operator-(void) const;
};

#endif

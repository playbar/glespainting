/**
 Copyright 2013 BlackBerry Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 Original file from GamePlay3D: http://gameplay3d.org

 This file was modified to fit the cocos2d-x project
 */

#include "math/CocVec2.h"

NS_CC_MATH_BEGIN

inline CocVec2::CocVec2()
: x(0.0f), y(0.0f)
{
}

inline CocVec2::CocVec2(float xx, float yy)
: x(xx), y(yy)
{
}

inline CocVec2::CocVec2(const float* array)
{
    set(array);
}

inline CocVec2::CocVec2(const CocVec2& p1, const CocVec2& p2)
{
    set(p1, p2);
}

inline CocVec2::CocVec2(const CocVec2& copy)
{
    set(copy);
}

inline CocVec2::~CocVec2()
{
}

inline bool CocVec2::isZero() const
{
    return x == 0.0f && y == 0.0f;
}

bool CocVec2::isOne() const
{
    return x == 1.0f && y == 1.0f;
}

inline void CocVec2::add(const CocVec2& v)
{
    x += v.x;
    y += v.y;
}

inline float CocVec2::distanceSquared(const CocVec2& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    return (dx * dx + dy * dy);
}

inline float CocVec2::dot(const CocVec2& v) const
{
    return (x * v.x + y * v.y);
}

inline float CocVec2::lengthSquared() const
{
    return (x * x + y * y);
}

inline void CocVec2::negate()
{
    x = -x;
    y = -y;
}

inline void CocVec2::scale(float scalar)
{
    x *= scalar;
    y *= scalar;
}

inline void CocVec2::scale(const CocVec2& scale)
{
    x *= scale.x;
    y *= scale.y;
}

inline void CocVec2::set(float xx, float yy)
{
    this->x = xx;
    this->y = yy;
}

inline void CocVec2::set(const CocVec2& v)
{
    this->x = v.x;
    this->y = v.y;
}

inline void CocVec2::set(const CocVec2& p1, const CocVec2& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
}

void CocVec2::setZero()
{
    x = y = 0.0f;
}

inline void CocVec2::subtract(const CocVec2& v)
{
    x -= v.x;
    y -= v.y;
}

inline void CocVec2::smooth(const CocVec2& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}

inline CocVec2 CocVec2::operator+(const CocVec2& v) const
{
    CocVec2 result(*this);
    result.add(v);
    return result;
}

inline CocVec2& CocVec2::operator+=(const CocVec2& v)
{
    add(v);
    return *this;
}

inline CocVec2 CocVec2::operator-(const CocVec2& v) const
{
    CocVec2 result(*this);
    result.subtract(v);
    return result;
}

inline CocVec2& CocVec2::operator-=(const CocVec2& v)
{
    subtract(v);
    return *this;
}

inline CocVec2 CocVec2::operator-() const
{
    CocVec2 result(*this);
    result.negate();
    return result;
}

inline CocVec2 CocVec2::operator*(float s) const
{
    CocVec2 result(*this);
    result.scale(s);
    return result;
}

inline CocVec2& CocVec2::operator*=(float s)
{
    scale(s);
    return *this;
}

inline CocVec2 CocVec2::operator/(const float s) const
{
    return CocVec2(this->x / s, this->y / s);
}

inline bool CocVec2::operator<(const CocVec2& v) const
{
    if (x == v.x)
    {
        return y < v.y;
    }
    return x < v.x;
}

inline bool CocVec2::operator>(const CocVec2& v) const
{
    if (x == v.x)
    {
        return y > v.y;
    }
    return x > v.x;
}

inline bool CocVec2::operator==(const CocVec2& v) const
{
    return x==v.x && y==v.y;
}

inline bool CocVec2::operator!=(const CocVec2& v) const
{
    return x!=v.x || y!=v.y;
}

inline CocVec2 operator*(float x, const CocVec2& v)
{
    CocVec2 result(v);
    result.scale(x);
    return result;
}

void CocVec2::setPoint(float xx, float yy)
{
    this->x = xx;
    this->y = yy;
}

NS_CC_MATH_END

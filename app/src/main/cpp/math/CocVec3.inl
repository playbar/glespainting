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

#include "math/CocVec3.h"
#include "math/CocMat4.h"

NS_CC_MATH_BEGIN


inline bool CocVec3::isZero() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f;
}

inline bool CocVec3::isOne() const
{
    return x == 1.0f && y == 1.0f && z == 1.0f;
}

inline void CocVec3::add(const CocVec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

inline void CocVec3::add(float xx, float yy, float zz)
{
    x += xx;
    y += yy;
    z += zz;
}

inline float CocVec3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

inline float CocVec3::lengthSquared() const
{
    return (x * x + y * y + z * z);
}

inline void CocVec3::negate()
{
    x = -x;
    y = -y;
    z = -z;
}

inline void CocVec3::scale(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

inline CocVec3 CocVec3::lerp(const CocVec3 &target, float alpha) const
{
    return *this * (1.f - alpha) + target * alpha;
}

inline void CocVec3::set(float xx, float yy, float zz)
{
    this->x = xx;
    this->y = yy;
    this->z = zz;
}

inline void CocVec3::set(const float* array)
{
    GP_ASSERT(array);

    x = array[0];
    y = array[1];
    z = array[2];
}

inline void CocVec3::set(const CocVec3& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

inline void CocVec3::set(const CocVec3& p1, const CocVec3& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
    z = p2.z - p1.z;
}

inline void CocVec3::setZero()
{
    x = y = z = 0.0f;
}

inline void CocVec3::subtract(const CocVec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

inline CocVec3 CocVec3::operator+(const CocVec3& v) const
{
    CocVec3 result(*this);
    result.add(v);
    return result;
}

inline CocVec3& CocVec3::operator+=(const CocVec3& v)
{
    add(v);
    return *this;
}

inline CocVec3 CocVec3::operator-(const CocVec3& v) const
{
    CocVec3 result(*this);
    result.subtract(v);
    return result;
}

inline CocVec3& CocVec3::operator-=(const CocVec3& v)
{
    subtract(v);
    return *this;
}

inline CocVec3 CocVec3::operator-() const
{
    CocVec3 result(*this);
    result.negate();
    return result;
}

inline CocVec3 CocVec3::operator*(float s) const
{
    CocVec3 result(*this);
    result.scale(s);
    return result;
}

inline CocVec3& CocVec3::operator*=(float s)
{
    scale(s);
    return *this;
}

inline CocVec3 CocVec3::operator/(const float s) const
{
    return CocVec3(this->x / s, this->y / s, this->z / s);
}

inline bool CocVec3::operator==(const CocVec3& v) const
{
    return x==v.x && y==v.y && z==v.z;
}

inline bool CocVec3::operator!=(const CocVec3& v) const
{
    return x!=v.x || y!=v.y || z!=v.z;
}

inline CocVec3 operator*(float x, const CocVec3& v)
{
    CocVec3 result(v);
    result.scale(x);
    return result;
}

NS_CC_MATH_END

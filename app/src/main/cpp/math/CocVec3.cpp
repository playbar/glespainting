/**
 Copyright 2013 BlackBerry Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

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
#include "math/MathUtil.h"
#include "base/ccMacros.h"

NS_CC_MATH_BEGIN

CocVec3::CocVec3()
    : x(0.0f), y(0.0f), z(0.0f)
{
}

CocVec3::CocVec3(float xx, float yy, float zz)
    : x(xx), y(yy), z(zz)
{
}

CocVec3::CocVec3(const float* array)
{
    set(array);
}

CocVec3::CocVec3(const CocVec3& p1, const CocVec3& p2)
{
    set(p1, p2);
}

CocVec3::CocVec3(const CocVec3& copy)
{
    set(copy);
}

CocVec3 CocVec3::fromColor(unsigned int color)
{
    float components[3];
    int componentIndex = 0;
    for (int i = 2; i >= 0; --i)
    {
        int component = (color >> i*8) & 0x0000ff;

        components[componentIndex++] = static_cast<float>(component) / 255.0f;
    }

    CocVec3 value(components);
    return value;
}

CocVec3::~CocVec3()
{
}

float CocVec3::angle(const CocVec3& v1, const CocVec3& v2)
{
    float dx = v1.y * v2.z - v1.z * v2.y;
    float dy = v1.z * v2.x - v1.x * v2.z;
    float dz = v1.x * v2.y - v1.y * v2.x;

    return std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, dot(v1, v2));
}

void CocVec3::add(const CocVec3& v1, const CocVec3& v2, CocVec3* dst)
{
    GP_ASSERT(dst);

    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
    dst->z = v1.z + v2.z;
}

void CocVec3::clamp(const CocVec3& min, const CocVec3& max)
{
    GP_ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));

    // Clamp the x value.
    if (x < min.x)
        x = min.x;
    if (x > max.x)
        x = max.x;

    // Clamp the y value.
    if (y < min.y)
        y = min.y;
    if (y > max.y)
        y = max.y;

    // Clamp the z value.
    if (z < min.z)
        z = min.z;
    if (z > max.z)
        z = max.z;
}

void CocVec3::clamp(const CocVec3& v, const CocVec3& min, const CocVec3& max, CocVec3* dst)
{
    GP_ASSERT(dst);
    GP_ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));

    // Clamp the x value.
    dst->x = v.x;
    if (dst->x < min.x)
        dst->x = min.x;
    if (dst->x > max.x)
        dst->x = max.x;

    // Clamp the y value.
    dst->y = v.y;
    if (dst->y < min.y)
        dst->y = min.y;
    if (dst->y > max.y)
        dst->y = max.y;

    // Clamp the z value.
    dst->z = v.z;
    if (dst->z < min.z)
        dst->z = min.z;
    if (dst->z > max.z)
        dst->z = max.z;
}

void CocVec3::cross(const CocVec3& v)
{
    cross(*this, v, this);
}

void CocVec3::cross(const CocVec3& v1, const CocVec3& v2, CocVec3* dst)
{
    GP_ASSERT(dst);

    // NOTE: This code assumes CocVec3 struct members are contiguous floats in memory.
    // We might want to revisit this (and other areas of code that make this assumption)
    // later to guarantee 100% safety/compatibility.
    MathUtil::crossVec3(&v1.x, &v2.x, &dst->x);
}

float CocVec3::distance(const CocVec3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float CocVec3::distanceSquared(const CocVec3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return (dx * dx + dy * dy + dz * dz);
}

float CocVec3::dot(const CocVec3& v) const
{
    return (x * v.x + y * v.y + z * v.z);
}

float CocVec3::dot(const CocVec3& v1, const CocVec3& v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

void CocVec3::normalize()
{
    float n = x * x + y * y + z * z;
    // Already normalized.
    if (n == 1.0f)
        return;
    
    n = std::sqrt(n);
    // Too close to zero.
    if (n < MATH_TOLERANCE)
        return;
    
    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
}

CocVec3 CocVec3::getNormalized() const
{
    CocVec3 v(*this);
    v.normalize();
    return v;
}

void CocVec3::subtract(const CocVec3& v1, const CocVec3& v2, CocVec3* dst)
{
    GP_ASSERT(dst);

    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
    dst->z = v1.z - v2.z;
}

void CocVec3::smooth(const CocVec3& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}

const CocVec3 CocVec3::ZERO(0.0f, 0.0f, 0.0f);
const CocVec3 CocVec3::ONE(1.0f, 1.0f, 1.0f);
const CocVec3 CocVec3::UNIT_X(1.0f, 0.0f, 0.0f);
const CocVec3 CocVec3::UNIT_Y(0.0f, 1.0f, 0.0f);
const CocVec3 CocVec3::UNIT_Z(0.0f, 0.0f, 1.0f);

NS_CC_MATH_END

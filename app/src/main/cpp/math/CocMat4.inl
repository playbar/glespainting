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

#include "math/CocMat4.h"

NS_CC_MATH_BEGIN

inline CocMat4 CocMat4::operator+(const CocMat4& mat) const
{
    CocMat4 result(*this);
    result.add(mat);
    return result;
}

inline CocMat4& CocMat4::operator+=(const CocMat4& mat)
{
    add(mat);
    return *this;
}

inline CocMat4 CocMat4::operator-(const CocMat4& mat) const
{
    CocMat4 result(*this);
    result.subtract(mat);
    return result;
}

inline CocMat4& CocMat4::operator-=(const CocMat4& mat)
{
    subtract(mat);
    return *this;
}

inline CocMat4 CocMat4::operator-() const
{
    CocMat4 mat(*this);
    mat.negate();
    return mat;
}

inline CocMat4 CocMat4::operator*(const CocMat4& mat) const
{
    CocMat4 result(*this);
    result.multiply(mat);
    return result;
}

inline CocMat4& CocMat4::operator*=(const CocMat4& mat)
{
    multiply(mat);
    return *this;
}

inline CocVec3& operator*=(CocVec3& v, const CocMat4& m)
{
    m.transformVector(&v);
    return v;
}

inline CocVec3 operator*(const CocMat4& m, const CocVec3& v)
{
    CocVec3 x;
    m.transformVector(v, &x);
    return x;
}

inline CocVec4& operator*=(CocVec4& v, const CocMat4& m)
{
    m.transformVector(&v);
    return v;
}

inline CocVec4 operator*(const CocMat4& m, const CocVec4& v)
{
    CocVec4 x;
    m.transformVector(v, &x);
    return x;
}

NS_CC_MATH_END

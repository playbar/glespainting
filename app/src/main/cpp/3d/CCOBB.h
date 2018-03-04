/****************************************************************************
 Copyright (c) 2014-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CC_OBB_H__
#define __CC_OBB_H__

#include "3d/CCAABB.h"

NS_CC_BEGIN

/**
 * @addtogroup _3d
 * @{
 */

/**
 * Oriented Bounding Box(OBB)
 * @brief the OBB is similar to the AABB but the bounding box has the same direction as Sprite3D. so it's collision detection more precise than AABB
 * @js NA
 */
class CC_DLL OBB
{
public:
    OBB();

    /*
     * Construct obb from oriented bounding box
     *
     * @lua NA
     */
    OBB(const AABB& aabb);
    
    /*
     * Construct obb from points
     *
     * @lua NA
     */
    OBB(const CocVec3* verts, int num);
    
    /*
     * Check point in
     */
    bool containPoint(const CocVec3& point) const;

    /*
     * Specify obb values
     */
    void set(const CocVec3& center, const CocVec3& _xAxis, const CocVec3& _yAxis, const CocVec3& _zAxis, const CocVec3& _extents);
    
    /*
     * Clear obb
     */ 
    void reset();

    /* face to the obb's -z direction
     * verts[0] : left top front
     * verts[1] : left bottom front
     * verts[2] : right bottom front
     * verts[3] : right top front
     *
     * face to the obb's z direction
     * verts[4] : right top back
     * verts[5] : right bottom back
     * verts[6] : left bottom back
     * verts[7] : left top back
     */
    void getCorners(CocVec3* verts) const;
    
    /*
     * Check intersect with other
     */
    bool intersects(const OBB& box) const;
    
	/**
     * Transforms the obb by the given transformation matrix.
     */
    void transform(const CocMat4& mat);
    
protected:
    /*
    * compute extX, extY, extZ
    */
    void computeExtAxis()
    {
        _extentX = _xAxis * _extents.x;
        _extentY = _yAxis * _extents.y;
        _extentZ = _zAxis * _extents.z;
    }
    
    /*
     * Project point to the target axis
     */
    float projectPoint(const CocVec3& point, const CocVec3& axis) const;
    
    /*
     * Calculate the min and max project value of through the box's corners
     */
    void getInterval(const OBB& box, const CocVec3& axis, float &min, float &max) const;
    
    /*
     * Get the edge of x y z axis direction
     */
    CocVec3 getEdgeDirection(int index) const;
    
    /*
     * Get the face of x y z axis direction
     */
    CocVec3 getFaceDirection(int index) const;

public:
    CocVec3 _center;   // obb center
    CocVec3 _xAxis;    // x axis of obb, unit vector
    CocVec3 _yAxis;    // y axis of obb, unit vector
    CocVec3 _zAxis;    // z axis of obb, unit vector
    CocVec3 _extentX;  // _xAxis * _extents.x
    CocVec3 _extentY;  // _yAxis * _extents.y
    CocVec3 _extentZ;  // _zAxis * _extents.z
    CocVec3 _extents;  // obb length along each axis
};

// end of 3d group
/// @}

NS_CC_END

#endif

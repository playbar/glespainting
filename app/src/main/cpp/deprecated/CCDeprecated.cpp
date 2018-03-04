/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
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
#include "deprecated/CCDeprecated.h"

#include "platform/CCPlatformMacros.h"
#include "math/CocVec2.h"
#include "math/CCGeometry.h"
#include "base/ccTypes.h"
#include "renderer/CCGLProgram.h"
#include "2d/CCDrawingPrimitives.h"
#include "base/CCDirector.h"


#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif

NS_CC_BEGIN

const CocVec2 CCPointZero;

/* The "zero" size -- equivalent to Size(0, 0). */
const Size CCSizeZero = Size::ZERO;

/* The "zero" rectangle -- equivalent to Rect(0, 0, 0, 0). */
const Rect CCRectZero = Rect::ZERO;


const Color3B ccWHITE = Color3B::WHITE;
const Color3B ccYELLOW = Color3B::YELLOW;
const Color3B ccGREEN = Color3B::GREEN;
const Color3B ccBLUE = Color3B::BLUE;
const Color3B ccRED = Color3B::RED;
const Color3B ccMAGENTA = Color3B::MAGENTA;
const Color3B ccBLACK = Color3B::BLACK;
const Color3B ccORANGE = Color3B::ORANGE;
const Color3B ccGRAY = Color3B::GRAY;

const BlendFunc kCCBlendFuncDisable = BlendFunc::DISABLE;

const char* kCCShader_PositionTextureColor = GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR;
const char* kCCShader_PositionTextureColorAlphaTest = GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST;
const char* kCCShader_PositionColor = GLProgram::SHADER_NAME_POSITION_COLOR;
const char* kCCShader_PositionTexture = GLProgram::SHADER_NAME_POSITION_TEXTURE;
const char* kCCShader_PositionTexture_uColor = GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR;
const char* kCCShader_PositionTextureA8Color = GLProgram::SHADER_NAME_POSITION_TEXTURE_A8_COLOR;
const char* kCCShader_Position_uColor = GLProgram::SHADER_NAME_POSITION_U_COLOR;
const char* kCCShader_PositionLengthTexureColor = GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR;

// uniform names
const char* kCCUniformPMatrix_s = GLProgram::UNIFORM_NAME_P_MATRIX;
const char* kCCUniformMVMatrix_s = GLProgram::UNIFORM_NAME_MV_MATRIX;
const char* kCCUniformMVPMatrix_s = GLProgram::UNIFORM_NAME_MVP_MATRIX;
const char* kCCUniformTime_s = GLProgram::UNIFORM_NAME_TIME;
const char* kCCUniformSinTime_s = GLProgram::UNIFORM_NAME_SIN_TIME;
const char* kCCUniformCosTime_s	= GLProgram::UNIFORM_NAME_COS_TIME;
const char* kCCUniformRandom01_s = GLProgram::UNIFORM_NAME_RANDOM01;
const char* kCCUniformSampler_s	= GLProgram::UNIFORM_NAME_SAMPLER0;
const char* kCCUniformAlphaTestValue = GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE;

// Attribute names
const char* kCCAttributeNameColor = GLProgram::ATTRIBUTE_NAME_COLOR;
const char* kCCAttributeNamePosition = GLProgram::ATTRIBUTE_NAME_POSITION;
const char* kCCAttributeNameTexCoord = GLProgram::ATTRIBUTE_NAME_TEX_COORD;

void ccDrawInit()
{
    DrawPrimitives::init();
}

void ccDrawFree()
{
//it will crash clang static analyzer so hide it if __clang_analyzer__ defined
#ifndef __clang_analyzer__
    DrawPrimitives::free();
#endif
}

void ccDrawPoint( const CocVec2& point )
{
    DrawPrimitives::drawPoint(point);
}

void ccDrawPoints( const CocVec2 *points, unsigned int numberOfPoints )
{
    DrawPrimitives::drawPoints(points, numberOfPoints);
}

void ccDrawLine( const CocVec2& origin, const CocVec2& destination )
{
    DrawPrimitives::drawLine(origin, destination);
}

void ccDrawRect( CocVec2 origin, CocVec2 destination )
{
    DrawPrimitives::drawRect(origin, destination);
}

void ccDrawSolidRect( CocVec2 origin, CocVec2 destination, Color4F color )
{
    DrawPrimitives::drawSolidRect(origin, destination, color);
}

void ccDrawPoly( const CocVec2 *vertices, unsigned int numOfVertices, bool closePolygon )
{
    DrawPrimitives::drawPoly(vertices, numOfVertices, closePolygon);
}

void ccDrawSolidPoly( const CocVec2 *poli, unsigned int numberOfPoints, Color4F color )
{
    DrawPrimitives::drawSolidPoly(poli, numberOfPoints, color);
}

void ccDrawCircle( const CocVec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY)
{
    DrawPrimitives::drawCircle(center, radius, angle, segments, drawLineToCenter, scaleX, scaleY);
}

void ccDrawCircle( const CocVec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter)
{
    DrawPrimitives::drawCircle(center, radius, angle, segments, drawLineToCenter);
}

void ccDrawSolidCircle( const CocVec2& center, float radius, float angle, unsigned int segments, float scaleX, float scaleY)
{
    DrawPrimitives::drawSolidCircle(center, radius, angle, segments, scaleX, scaleY);
}

void ccDrawSolidCircle( const CocVec2& center, float radius, float angle, unsigned int segments)
{
    DrawPrimitives::drawSolidCircle(center, radius, angle, segments);
}

void ccDrawQuadBezier(const CocVec2& origin, const CocVec2& control, const CocVec2& destination, unsigned int segments)
{
    DrawPrimitives::drawQuadBezier(origin, control, destination, segments);
}

void ccDrawCubicBezier(const CocVec2& origin, const CocVec2& control1, const CocVec2& control2, const CocVec2& destination, unsigned int segments)
{
    DrawPrimitives::drawCubicBezier(origin, control1, control2, destination, segments);
}

void ccDrawCatmullRom( PointArray *arrayOfControlPoints, unsigned int segments )
{
    DrawPrimitives::drawCatmullRom(arrayOfControlPoints, segments);
}

void ccDrawCardinalSpline( PointArray *config, float tension,  unsigned int segments )
{
    DrawPrimitives::drawCardinalSpline(config, tension, segments);
}

void ccDrawColor4B( GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
    DrawPrimitives::setDrawColor4B(r, g, b, a);
}

void ccDrawColor4F( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    DrawPrimitives::setDrawColor4F(r, g, b, a);
}

void ccPointSize( GLfloat pointSize )
{
    DrawPrimitives::setPointSize(pointSize);
}

MATRIX_STACK_TYPE currentActiveStackType = MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW;

void CC_DLL kmGLFreeAll(void)
{
    Director::getInstance()->resetMatrixStack();
}

void CC_DLL kmGLPushMatrix(void)
{
    Director::getInstance()->pushMatrix(currentActiveStackType);
}

void CC_DLL kmGLPopMatrix(void)
{
    Director::getInstance()->popMatrix(currentActiveStackType);
}

void CC_DLL kmGLMatrixMode(unsigned int mode)
{
    if(KM_GL_MODELVIEW == mode)
        currentActiveStackType = MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW;
    else if(KM_GL_PROJECTION == mode)
        currentActiveStackType = MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION;
    else if(KM_GL_TEXTURE == mode)
        currentActiveStackType = MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE;
    else
    {
        CC_ASSERT(false);
    }
}

void CC_DLL kmGLLoadIdentity(void)
{
    Director::getInstance()->loadIdentityMatrix(currentActiveStackType);
}

void CC_DLL kmGLLoadMatrix(const CocMat4* pIn)
{
    Director::getInstance()->loadMatrix(currentActiveStackType, *pIn);
}

void CC_DLL kmGLMultMatrix(const CocMat4* pIn)
{
    Director::getInstance()->multiplyMatrix(currentActiveStackType, *pIn);
}

void CC_DLL kmGLTranslatef(float x, float y, float z)
{
    CocMat4 mat;
    CocMat4::createTranslation(CocVec3(x, y, z), &mat);
    Director::getInstance()->multiplyMatrix(currentActiveStackType, mat);
}

void CC_DLL kmGLRotatef(float angle, float x, float y, float z)
{
    CocMat4 mat;
    CocMat4::createRotation(CocVec3(x, y, z), angle, &mat);
    Director::getInstance()->multiplyMatrix(currentActiveStackType, mat);
}

void CC_DLL kmGLScalef(float x, float y, float z)
{
    CocMat4 mat;
    CocMat4::createScale(x, y, z, &mat);
    Director::getInstance()->multiplyMatrix(currentActiveStackType, mat);
}

void CC_DLL kmGLGetMatrix(unsigned int mode, CocMat4* pOut)
{
    if(KM_GL_MODELVIEW == mode)
        *pOut = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    else if(KM_GL_PROJECTION == mode)
        *pOut = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    else if(KM_GL_TEXTURE == mode)
        *pOut = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE);
    else
    {
        CC_ASSERT(false);
    }
}

CocMat4* kmMat4Fill(CocMat4* pOut, const float* pMat)
{
    pOut->set(pMat);
    return pOut;
}

CocMat4* kmMat4Assign(CocMat4* pOut, const CocMat4* pIn)
{
    pOut->set(pIn->m);
    return pOut;
}

CocMat4* kmMat4Identity(CocMat4* pOut)
{
    *pOut = CocMat4::IDENTITY;
    return pOut;
}

CocMat4* kmMat4Inverse(CocMat4* pOut, const CocMat4* pM)
{
    *pOut = pM->getInversed();
    return pOut;
}

CocMat4* kmMat4Transpose(CocMat4* pOut, const CocMat4* pIn)
{
    *pOut = pIn->getTransposed();
    return pOut;
}

CocMat4* kmMat4Multiply(CocMat4* pOut, const CocMat4* pM1, const CocMat4* pM2)
{
    *pOut = (*pM1) * (*pM2);
    return pOut;
}

CocMat4* kmMat4Translation(CocMat4* pOut, const float x, const float y, const float z)
{
    CocMat4::createTranslation(x, y, z, pOut);
    return pOut;
}

CocMat4* kmMat4RotationX(CocMat4* pOut, const float radians)
{
    CocMat4::createRotationX(radians, pOut);
    return pOut;
}

CocMat4* kmMat4RotationY(CocMat4* pOut, const float radians)
{
    CocMat4::createRotationY(radians, pOut);
    return pOut;
}

CocMat4* kmMat4RotationZ(CocMat4* pOut, const float radians)
{
    CocMat4::createRotationZ(radians, pOut);
    return pOut;
}

CocMat4* kmMat4RotationAxisAngle(CocMat4* pOut, const CocVec3* axis, float radians)
{
    CocMat4::createRotation(*axis, radians, pOut);
    return pOut;
}

CocMat4* kmMat4Scaling(CocMat4* pOut, const float x, const float y, const float z)
{
    CocMat4::createScale(x, y, z, pOut);
    return pOut;
}

CocMat4* kmMat4PerspectiveProjection(CocMat4* pOut, float fovY, float aspect, float zNear, float zFar)
{
    CocMat4::createPerspective(fovY, aspect, zNear, zFar, pOut);
    return pOut;
}

CocMat4* kmMat4OrthographicProjection(CocMat4* pOut, float left, float right, float bottom, float top, float nearVal, float farVal)
{
    CocMat4::createOrthographicOffCenter(left, right, bottom, top, nearVal, farVal, pOut);
    return pOut;
}

CocMat4* kmMat4LookAt(CocMat4* pOut, const CocVec3* pEye, const CocVec3* pCenter, const CocVec3* pUp)
{
    CocMat4::createLookAt(*pEye, *pCenter, *pUp, pOut);
    return pOut;
}

CocVec3* kmVec3Fill(CocVec3* pOut, float x, float y, float z)
{
    pOut->x = x;
    pOut->y = y;
    pOut->z = z;
    return pOut;
}

float kmVec3Length(const CocVec3* pIn)
{
    return pIn->length();
}

float kmVec3LengthSq(const CocVec3* pIn)
{
    return pIn->lengthSquared();
}

CC_DLL CocVec3* kmVec3Lerp(CocVec3* pOut, const CocVec3* pV1, const CocVec3* pV2, float t)
{
    pOut->x = pV1->x + t * ( pV2->x - pV1->x );
    pOut->y = pV1->y + t * ( pV2->y - pV1->y );
    pOut->z = pV1->z + t * ( pV2->z - pV1->z );
    return pOut;
}

CocVec3* kmVec3Normalize(CocVec3* pOut, const CocVec3* pIn)
{
    *pOut = pIn->getNormalized();
    return pOut;
}

CocVec3* kmVec3Cross(CocVec3* pOut, const CocVec3* pV1, const CocVec3* pV2)
{
    CocVec3::cross(*pV1, *pV2, pOut);
    return pOut;
}

float kmVec3Dot(const CocVec3* pV1, const CocVec3* pV2)
{
    return CocVec3::dot(*pV1, *pV2);
}

CocVec3* kmVec3Add(CocVec3* pOut, const CocVec3* pV1, const CocVec3* pV2)
{
    CocVec3::add(*pV1, *pV2, pOut);
    return pOut;
}

CocVec3* kmVec3Subtract(CocVec3* pOut, const CocVec3* pV1, const CocVec3* pV2)
{
    CocVec3::subtract(*pV1, *pV2, pOut);
    return pOut;
}

CocVec3* kmVec3Transform(CocVec3* pOut, const CocVec3* pV1, const CocMat4* pM)
{
    pM->transformPoint(*pV1, pOut);
    return pOut;
}

CocVec3* kmVec3TransformNormal(CocVec3* pOut, const CocVec3* pV, const CocMat4* pM)
{
    pM->transformVector(*pV, pOut);
    return pOut;
}

CocVec3* kmVec3TransformCoord(CocVec3* pOut, const CocVec3* pV, const CocMat4* pM)
{
    CocVec4 v(pV->x, pV->y, pV->z, 1);
    pM->transformVector(&v);
    v = v * (1/v.w);
    pOut->set(v.x, v.y, v.z);
    return pOut;
}

CocVec3* kmVec3Scale(CocVec3* pOut, const CocVec3* pIn, const float s)
{
    *pOut = *pIn * s;
    return pOut;
}

CocVec3* kmVec3Assign(CocVec3* pOut, const CocVec3* pIn)
{
    *pOut = *pIn;
    return pOut;
}

CocVec3* kmVec3Zero(CocVec3* pOut)
{
    pOut->set(0, 0, 0);
    return pOut;
}

CocVec2* kmVec2Fill(CocVec2* pOut, float x, float y)
{
    pOut->set(x, y);
    return pOut;
}

float kmVec2Length(const CocVec2* pIn)
{
    return pIn->length();
}

float kmVec2LengthSq(const CocVec2* pIn)
{
    return pIn->lengthSquared();
}

CocVec2* kmVec2Normalize(CocVec2* pOut, const CocVec2* pIn)
{
    *pOut = pIn->getNormalized();
    return pOut;
}

CocVec2* kmVec2Lerp(CocVec2* pOut, const CocVec2* pV1, const CocVec2* pV2, float t)
{
    pOut->x = pV1->x + t * ( pV2->x - pV1->x );
    pOut->y = pV1->y + t * ( pV2->y - pV1->y );
    return pOut;
}

CocVec2* kmVec2Add(CocVec2* pOut, const CocVec2* pV1, const CocVec2* pV2)
{
    CocVec2::add(*pV1, *pV2, pOut);
    return pOut;
}

float kmVec2Dot(const CocVec2* pV1, const CocVec2* pV2)
{
    return CocVec2::dot(*pV1, *pV2);
}

CocVec2* kmVec2Subtract(CocVec2* pOut, const CocVec2* pV1, const CocVec2* pV2)
{
    CocVec2::subtract(*pV1, *pV2, pOut);
    return pOut;
}

CocVec2* kmVec2Scale(CocVec2* pOut, const CocVec2* pIn, const float s)
{
    *pOut = *pIn * s;
    return pOut;
}

CocVec2* kmVec2Assign(CocVec2* pOut, const CocVec2* pIn)
{
    *pOut = *pIn;
    return pOut;
}

CocVec4* kmVec4Fill(CocVec4* pOut, float x, float y, float z, float w)
{
    pOut->set(x, y, z, w);
    return pOut;
}

CocVec4* kmVec4Add(CocVec4* pOut, const CocVec4* pV1, const CocVec4* pV2)
{
    CocVec4::add(*pV1, *pV2, pOut);
    return pOut;
}

float kmVec4Dot(const CocVec4* pV1, const CocVec4* pV2)
{
    return CocVec4::dot(*pV1, *pV2);
}

float kmVec4Length(const CocVec4* pIn)
{
    return pIn->length();
}

float kmVec4LengthSq(const CocVec4* pIn)
{
    return pIn->lengthSquared();
}

CocVec4* kmVec4Lerp(CocVec4* pOut, const CocVec4* pV1, const CocVec4* pV2, float t)
{
    pOut->x = pV1->x + t * ( pV2->x - pV1->x );
    pOut->y = pV1->y + t * ( pV2->y - pV1->y );
    pOut->z = pV1->z + t * ( pV2->z - pV1->z );
    pOut->w = pV1->w + t * ( pV2->w - pV1->w );
    return pOut;
}

CocVec4* kmVec4Normalize(CocVec4* pOut, const CocVec4* pIn)
{
    *pOut = pIn->getNormalized();
    return pOut;
}

CocVec4* kmVec4Scale(CocVec4* pOut, const CocVec4* pIn, const float s)
{
    *pOut = *pIn * s;
    return pOut;
}

CocVec4* kmVec4Subtract(CocVec4* pOut, const CocVec4* pV1, const CocVec4* pV2)
{
    CocVec4::subtract(*pV1, *pV2, pOut);
    return pOut;
}

CocVec4* kmVec4Assign(CocVec4* pOut, const CocVec4* pIn)
{
    *pOut = *pIn;
    return pOut;
}

CocVec4* kmVec4MultiplyMat4(CocVec4* pOut, const CocVec4* pV, const CocMat4* pM)
{
    pM->transformVector(*pV, pOut);
    return pOut;
}

CocVec4* kmVec4Transform(CocVec4* pOut, const CocVec4* pV, const CocMat4* pM)
{
    pM->transformVector(*pV, pOut);
    return pOut;
}

const CocVec3 KM_VEC3_NEG_Z(0, 0, -1);
const CocVec3 KM_VEC3_POS_Z(0, 0, 1);
const CocVec3 KM_VEC3_POS_Y(0, 1, 0);
const CocVec3 KM_VEC3_NEG_Y(0, -1, 0);
const CocVec3 KM_VEC3_NEG_X(-1, 0, 0);
const CocVec3 KM_VEC3_POS_X(1, 0, 0);
const CocVec3 KM_VEC3_ZERO(0, 0, 0);

const CocVec2 KM_VEC2_POS_Y(0, 1);
const CocVec2 KM_VEC2_NEG_Y(0, -1);
const CocVec2 KM_VEC2_NEG_X(-1, 0);
const CocVec2 KM_VEC2_POS_X(1, 0);
const CocVec2 KM_VEC2_ZERO(0, 0);

NS_CC_END

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (pop)
#endif

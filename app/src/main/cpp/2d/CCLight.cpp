/****************************************************************************
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

#include "2d/CCLight.h"
#include "2d/CCScene.h"

NS_CC_BEGIN

void BaseLight::setIntensity(float intensity)
{
    CC_ASSERT(intensity >= 0);
    _intensity = intensity;
}

void BaseLight::onEnter()
{
    auto scene = getScene();
    if (scene)
    {
        auto &lights = scene->_lights;
        auto iter = std::find(lights.begin(), lights.end(), this);
        if (iter == lights.end())
            lights.push_back(this);
    }
    Node::onEnter();
}
void BaseLight::onExit()
{
    auto scene = getScene();
    if (scene)
    {
        auto &lights = scene->_lights;
        auto iter = std::find(lights.begin(), lights.end(), this);
        if (iter != lights.end())
            lights.erase(iter);
    }
    Node::onExit();
}

void BaseLight::setRotationFromDirection( const CocVec3 &direction )
{
    float projLen = sqrt(direction.x * direction.x + direction.z * direction.z);
    float rotY = CC_RADIANS_TO_DEGREES(atan2f(-direction.x, -direction.z));
    float rotX = -CC_RADIANS_TO_DEGREES(atan2f(-direction.y, projLen));
    setRotation3D(CocVec3(rotX, rotY, 0.0f));
}

BaseLight::BaseLight()
: _intensity(1.0f)
, _lightFlag(LightFlag::LIGHT0)
, _enabled(true)
{
    
}
BaseLight::~BaseLight()
{
    
}


////////////////////////////////////////////////////////////////////
DirectionLight* DirectionLight::create(const CocVec3 &direction, const Color3B &color)
{
    auto light = new (std::nothrow) DirectionLight();
    light->setRotationFromDirection(direction);
    light->setColor(color);
    light->autorelease();
    return light;
}

void DirectionLight::setDirection(const CocVec3 &dir)
{
    setRotationFromDirection(dir);
}
CocVec3 DirectionLight::getDirection() const
{
    CocMat4 mat = getNodeToParentTransform();
    return CocVec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}
CocVec3 DirectionLight::getDirectionInWorld() const
{
    CocMat4 mat = getNodeToWorldTransform();
    return CocVec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}
DirectionLight::DirectionLight()
{
    
}
DirectionLight::~DirectionLight()
{
    
}

//////////////////////////////////////////////////////////////////
PointLight* PointLight::create(const CocVec3 &position, const Color3B &color, float range)
{
    auto light = new (std::nothrow) PointLight();
    light->setPosition3D(position);
    light->setColor(color);
    light->_range = range;
    light->autorelease();
    return light;
}

PointLight::PointLight()
{
    
}
PointLight::~PointLight()
{
    
}

//////////////////////////////////////////////////////////////
SpotLight* SpotLight::create(const CocVec3 &direction, const CocVec3 &position, const Color3B &color, float innerAngle, float outerAngle, float range)
{
    auto light = new (std::nothrow) SpotLight();
    light->setRotationFromDirection(direction);
    light->setPosition3D(position);
    light->setColor(color);
    light->setInnerAngle(innerAngle);
    light->setOuterAngle(outerAngle);
    light->_range = range;
    light->autorelease();
    return light;
}

void SpotLight::setDirection(const CocVec3 &dir)
{
    setRotationFromDirection(dir);
}

CocVec3 SpotLight::getDirection() const
{
    CocMat4 mat = getNodeToParentTransform();
    return CocVec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}

CocVec3 SpotLight::getDirectionInWorld() const
{
    CocMat4 mat = getNodeToWorldTransform();
    return CocVec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}

void SpotLight::setInnerAngle(float angle)
{
    _innerAngle = angle;
    _cosInnerAngle = cosf(angle);
}

void SpotLight::setOuterAngle(float angle)
{
    _outerAngle = angle;
    _cosOuterAngle = cosf(angle);
}

SpotLight::SpotLight()
{

}

SpotLight::~SpotLight()
{

}

/////////////////////////////////////////////////////////////

AmbientLight* AmbientLight::create( const Color3B &color )
{
    auto light = new (std::nothrow) AmbientLight();
    light->setColor(color);
    light->autorelease();
    return light;
}

AmbientLight::AmbientLight()
{

}

AmbientLight::~AmbientLight()
{

}

NS_CC_END
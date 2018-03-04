#include "2d/CCDrawLayer.h"
#include "base/CCEventType.h"
#include "base/CCConfiguration.h"
#include "renderer/CCRenderer.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgramCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventDispatcher.h"
#include "2d/CCActionCatmullRom.h"
#include "platform/CCGL.h"

NS_CC_BEGIN

// CocVec2 == CGPoint in 32-bits, but not in 64-bits (OS X)
// that's why the "v2f" functions are needed
static CocVec2 v2fzero(0.0f,0.0f);

static inline CocVec2 v2f(float x, float y)
{
    CocVec2 ret(x, y);
    return ret;
}

static inline CocVec2 v2fadd(const CocVec2 &v0, const CocVec2 &v1)
{
    return v2f(v0.x+v1.x, v0.y+v1.y);
}

static inline CocVec2 v2fsub(const CocVec2 &v0, const CocVec2 &v1)
{
    return v2f(v0.x-v1.x, v0.y-v1.y);
}

static inline CocVec2 v2fmult(const CocVec2 &v, float s)
{
    return v2f(v.x * s, v.y * s);
}

static inline CocVec2 v2fperp(const CocVec2 &p0)
{
    return v2f(-p0.y, p0.x);
}

static inline CocVec2 v2fneg(const CocVec2 &p0)
{
    return v2f(-p0.x, - p0.y);
}

static inline float v2fdot(const CocVec2 &p0, const CocVec2 &p1)
{
    return  p0.x * p1.x + p0.y * p1.y;
}

static inline CocVec2 v2fnormalize(const CocVec2 &p)
{
    CocVec2 r(p.x, p.y);
    r.normalize();
    return v2f(r.x, r.y);
}

static inline CocVec2 __v2f(const CocVec2 &v)
{
//#ifdef __LP64__
    return v2f(v.x, v.y);
// #else
//     return * ((CocVec2*) &v);
// #endif
}

static inline Tex2F __t(const CocVec2 &v)
{
    return *(Tex2F*)&v;
}

// implementation of DrawLayer

DrawLayer::DrawLayer(GLfloat lineWidth)
: _vao(0)
, _vbo(0)
, _vaoGLPoint(0)
, _vboGLPoint(0)
, _vaoGLLine(0)
, _vboGLLine(0)
, _bufferCapacity(0)
, _bufferCount(0)
, _buffer(nullptr)
, _bufferCapacityGLPoint(0)
, _bufferCountGLPoint(0)
, _bufferGLPoint(nullptr)
, _bufferCapacityGLLine(0)
, _bufferCountGLLine(0)
, _bufferGLLine(nullptr)
, _dirty(false)
, _dirtyGLPoint(false)
, _dirtyGLLine(false)
, _lineWidth(lineWidth)
, _defaultLineWidth(lineWidth)
{
    static int layercount = 0;
    char name[16] = {0};
    sprintf(name, "layer_%d", ++layercount);
    _layerName = name;
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
}

DrawLayer::~DrawLayer()
{
    free(_buffer);
    _buffer = nullptr;
    free(_bufferGLPoint);
    _bufferGLPoint = nullptr;
    free(_bufferGLLine);
    _bufferGLLine = nullptr;
    
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_vboGLLine);
    glDeleteBuffers(1, &_vboGLPoint);
    _vbo = 0;
    _vboGLPoint = 0;
    _vboGLLine = 0;
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
        glDeleteVertexArrays(1, &_vao);
        glDeleteVertexArrays(1, &_vaoGLLine);
        glDeleteVertexArrays(1, &_vaoGLPoint);
        _vao = _vaoGLLine = _vaoGLPoint = 0;
    }
}

DrawLayer* DrawLayer::create(GLfloat defaultLineWidth)
{
    DrawLayer* ret = new (std::nothrow) DrawLayer(defaultLineWidth);
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

void DrawLayer::ensureCapacity(int count)
{
    CCASSERT(count>=0, "capacity must be >= 0");
    
    if(_bufferCount + count > _bufferCapacity)
    {
        _bufferCapacity += MAX(_bufferCapacity, count);
        _buffer = (V2F_C4B_T2F*)realloc(_buffer, _bufferCapacity*sizeof(V2F_C4B_T2F));
    }
}

void DrawLayer::ensureCapacityGLPoint(int count)
{
    CCASSERT(count>=0, "capacity must be >= 0");
    
    if(_bufferCountGLPoint + count > _bufferCapacityGLPoint)
    {
        _bufferCapacityGLPoint += MAX(_bufferCapacityGLPoint, count);
        _bufferGLPoint = (V2F_C4B_T2F*)realloc(_bufferGLPoint, _bufferCapacityGLPoint*sizeof(V2F_C4B_T2F));
    }
}

void DrawLayer::ensureCapacityGLLine(int count)
{
    CCASSERT(count>=0, "capacity must be >= 0");
    
    if(_bufferCountGLLine + count > _bufferCapacityGLLine)
    {
        _bufferCapacityGLLine += MAX(_bufferCapacityGLLine, count);
        _bufferGLLine = (V2F_C4B_T2F*)realloc(_bufferGLLine, _bufferCapacityGLLine*sizeof(V2F_C4B_T2F));
    }
}

bool DrawLayer::init()
{
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));
    
    ensureCapacity(512);
    ensureCapacityGLPoint(64);
    ensureCapacityGLLine(256);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        glGenVertexArrays(1, &_vao);
        GL::bindVAO(_vao);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcoord
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
        
        glGenVertexArrays(1, &_vaoGLLine);
        GL::bindVAO(_vaoGLLine);
        glGenBuffers(1, &_vboGLLine);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcoord
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
        
        glGenVertexArrays(1, &_vaoGLPoint);
        GL::bindVAO(_vaoGLPoint);
        glGenBuffers(1, &_vboGLPoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // Texture coord as pointsize
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
        
        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    }
    else
    {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
        
        glGenBuffers(1, &_vboGLLine);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        
        glGenBuffers(1, &_vboGLPoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    CHECK_GL_ERROR_DEBUG();
    
    _dirty = true;
    _dirtyGLLine = true;
    _dirtyGLPoint = true;
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // Need to listen the event only when not use batchnode, because it will use VBO
    auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
   /** listen the event that renderer was recreated on Android/WP8 */
        this->init();
    });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    
    return true;
}

void DrawLayer::draw(CocRenderer *renderer, const CocMat4 &transform, uint32_t flags)
{
    if(_bufferCount)
    {
        _customCommand.init(_globalZOrder, transform, flags);
        _customCommand.func = CC_CALLBACK_0(DrawLayer::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
    
    if(_bufferCountGLPoint)
    {
        _customCommandGLPoint.init(_globalZOrder, transform, flags);
        _customCommandGLPoint.func = CC_CALLBACK_0(DrawLayer::onDrawGLPoint, this, transform, flags);
        renderer->addCommand(&_customCommandGLPoint);
    }
    
    if(_bufferCountGLLine)
    {
        _customCommandGLLine.init(_globalZOrder, transform, flags);
        _customCommandGLLine.func = CC_CALLBACK_0(DrawLayer::onDrawGLLine, this, transform, flags);
        renderer->addCommand(&_customCommandGLLine);
    }
}

void DrawLayer::onDraw(const CocMat4 &transform, uint32_t /*flags*/)
{
    getGLProgramState()->apply(transform);
    auto glProgram = this->getGLProgram();
    glProgram->setUniformLocationWith1f(glProgram->getUniformLocation("u_alpha"), _displayedOpacity / 255.0);
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    if (_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);
        
        _dirty = false;
    }
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vao);
    }
    else
    {
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcoord
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }

    glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
    CHECK_GL_ERROR_DEBUG();
}

void DrawLayer::onDrawGLLine(const CocMat4 &transform, uint32_t /*flags*/)
{
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR);
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    glProgram->setUniformLocationWith1f(glProgram->getUniformLocation("u_alpha"), _displayedOpacity / 255.0);

    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    if (_dirtyGLLine)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        _dirtyGLLine = false;
    }
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vaoGLLine);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcoord
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }

    glLineWidth(_lineWidth);
    glDrawArrays(GL_LINES, 0, _bufferCountGLLine);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLLine);

    CHECK_GL_ERROR_DEBUG();
}

void DrawLayer::onDrawGLPoint(const CocMat4 &transform, uint32_t /*flags*/)
{
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE);
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    glProgram->setUniformLocationWith1f(glProgram->getUniformLocation("u_alpha"), _displayedOpacity / 255.0);

    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    if (_dirtyGLPoint)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        
        _dirtyGLPoint = false;
    }
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vaoGLPoint);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }
    
    glDrawArrays(GL_POINTS, 0, _bufferCountGLPoint);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    mh_drawNormalLine(1, 100, 100, 10, 0xff00ff00, 0);
//    mh_drawNormalLine(1, 200, 100, 10, 0xff00ff00, 0);
//    mh_drawNormalLine(1, 300, 100, 10, 0xff00ff00, 0);

    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLPoint);
    CHECK_GL_ERROR_DEBUG();
}

void DrawLayer::drawPoint(const CocVec2& position, const float pointSize, const Color4F &color)
{
    if( _block )
        return;

    ensureCapacityGLPoint(1);
    
    V2F_C4B_T2F *point = (V2F_C4B_T2F*)(_bufferGLPoint + _bufferCountGLPoint);
    V2F_C4B_T2F a = {position, Color4B(color), Tex2F(pointSize,0)};
    *point = a;
    
    _bufferCountGLPoint += 1;
    _dirtyGLPoint = true;
}

void DrawLayer::drawPoints(const CocVec2 *position, unsigned int numberOfPoints, const Color4F &color)
{
    drawPoints(position, numberOfPoints, 1.0, color);
}

void DrawLayer::drawPoints(const CocVec2 *position, unsigned int numberOfPoints, const float pointSize, const Color4F &color)
{
    if( _block )
        return;
    ensureCapacityGLPoint(numberOfPoints);
    
    V2F_C4B_T2F *point = (V2F_C4B_T2F*)(_bufferGLPoint + _bufferCountGLPoint);
    
    for(unsigned int i=0; i < numberOfPoints; i++,point++)
    {
        V2F_C4B_T2F a = {position[i], Color4B(color), Tex2F(pointSize,0)};
        *point = a;
    }
    
    _bufferCountGLPoint += numberOfPoints;
    _dirtyGLPoint = true;
}

void DrawLayer::drawLine(const CocVec2 &origin, const CocVec2 &destination, const Color4F &color)
{
    if( _block )
        return;
    ensureCapacityGLLine(2);
    
    V2F_C4B_T2F *point = (V2F_C4B_T2F*)(_bufferGLLine + _bufferCountGLLine);
    
    V2F_C4B_T2F a = {origin, Color4B(color), Tex2F(0.0, 0.0)};
    V2F_C4B_T2F b = {destination, Color4B(color), Tex2F(0.0, 0.0)};
    
    *point = a;
    *(point+1) = b;
    
    _bufferCountGLLine += 2;
    _dirtyGLLine = true;
}

void DrawLayer::drawRect(const CocVec2 &origin, const CocVec2 &destination, const Color4F &color)
{
    if( _block )
        return;
    drawLine(CocVec2(origin.x, origin.y), CocVec2(destination.x, origin.y), color);
    drawLine(CocVec2(destination.x, origin.y), CocVec2(destination.x, destination.y), color);
    drawLine(CocVec2(destination.x, destination.y), CocVec2(origin.x, destination.y), color);
    drawLine(CocVec2(origin.x, destination.y), CocVec2(origin.x, origin.y), color);
}

void DrawLayer::drawPoly(const CocVec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color)
{
    if( _block )
        return;
    unsigned int vertex_count;
    if(closePolygon)
    {
        vertex_count = 2 * numberOfPoints;
        ensureCapacityGLLine(vertex_count);
    }
    else
    {
        vertex_count = 2 * (numberOfPoints - 1);
        ensureCapacityGLLine(vertex_count);
    }
    
    V2F_C4B_T2F *point = (V2F_C4B_T2F*)(_bufferGLLine + _bufferCountGLLine);
 
    unsigned int i = 0;
    for(; i<numberOfPoints-1; i++)
    {
        V2F_C4B_T2F a = {poli[i], Color4B(color), Tex2F(0.0, 0.0)};
        V2F_C4B_T2F b = {poli[i+1], Color4B(color), Tex2F(0.0, 0.0)};
        
        *point = a;
        *(point+1) = b;
        point += 2;
    }
    if(closePolygon)
    {
        V2F_C4B_T2F a = {poli[i], Color4B(color), Tex2F(0.0, 0.0)};
        V2F_C4B_T2F b = {poli[0], Color4B(color), Tex2F(0.0, 0.0)};
        *point = a;
        *(point+1) = b;
    }
    
    _bufferCountGLLine += vertex_count;
}

void DrawLayer::drawCircle(const CocVec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const Color4F &color)
{
    if( _block )
        return;
    const float coef = 2.0f * (float)M_PI/segments;
    
    CocVec2 *vertices = new (std::nothrow) CocVec2[segments+2];
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) * scaleX + center.x;
        GLfloat k = radius * sinf(rads + angle) * scaleY + center.y;
        
        vertices[i].x = j;
        vertices[i].y = k;
    }
    if(drawLineToCenter)
    {
        vertices[segments+1].x = center.x;
        vertices[segments+1].y = center.y;
        drawPoly(vertices, segments+2, true, color);
    }
    else
        drawPoly(vertices, segments+1, true, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawLayer::drawCircle(const CocVec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const Color4F &color)
{
    if( _block )
        return;
    drawCircle(center, radius, angle, segments, drawLineToCenter, 1.0f, 1.0f, color);
}

void DrawLayer::drawQuadBezier(const CocVec2 &origin, const CocVec2 &control, const CocVec2 &destination, unsigned int segments, const Color4F &color)
{
    if( _block )
        return;
    CocVec2* vertices = new (std::nothrow) CocVec2[segments + 1];
    if( ! vertices )
        return;
    
    float t = 0.0f;
    for(unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 2) * origin.x + 2.0f * (1 - t) * t * control.x + t * t * destination.x;
        vertices[i].y = powf(1 - t, 2) * origin.y + 2.0f * (1 - t) * t * control.y + t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;
    
    drawPoly(vertices, segments+1, false, color);

    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawLayer::drawCubicBezier(const CocVec2 &origin, const CocVec2 &control1, const CocVec2 &control2, const CocVec2 &destination, unsigned int segments, const Color4F &color)
{
    if( _block )
        return;
    CocVec2* vertices = new (std::nothrow) CocVec2[segments + 1];
    if( ! vertices )
        return;
    
    float t = 0;
    for (unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 3) * origin.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * destination.x;
        vertices[i].y = powf(1 - t, 3) * origin.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;
    
    drawPoly(vertices, segments+1, false, color);

    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawLayer::drawCardinalSpline(PointArray *config, float tension,  unsigned int segments, const Color4F &color)
{
    if( _block )
        return;
    CocVec2* vertices = new (std::nothrow) CocVec2[segments + 1];
    if( ! vertices )
        return;
    
    ssize_t p;
    float lt;
    float deltaT = 1.0f / config->count();
    
    for( unsigned int i=0; i < segments+1;i++) {
        
        float dt = (float)i / segments;
        
        // border
        if( dt == 1 ) {
            p = config->count() - 1;
            lt = 1;
        } else {
            p = dt / deltaT;
            lt = (dt - deltaT * (float)p) / deltaT;
        }
        
        // Interpolate
        CocVec2 pp0 = config->getControlPointAtIndex(p-1);
        CocVec2 pp1 = config->getControlPointAtIndex(p+0);
        CocVec2 pp2 = config->getControlPointAtIndex(p+1);
        CocVec2 pp3 = config->getControlPointAtIndex(p+2);
        
        CocVec2 newPos = ccCardinalSplineAt( pp0, pp1, pp2, pp3, tension, lt);
        vertices[i].x = newPos.x;
        vertices[i].y = newPos.y;
    }
    
    drawPoly(vertices, segments+1, false, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawLayer::drawCatmullRom(PointArray *points, unsigned int segments, const Color4F &color)
{
    if( _block )
        return;
    drawCardinalSpline( points, 0.5f, segments, color);
}

void DrawLayer::drawDot(const CocVec2 &pos, float radius, const Color4F &color)
{
    if( _block )
        return;
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
    
    V2F_C4B_T2F a = {CocVec2(pos.x - radius, pos.y - radius), Color4B(color), Tex2F(-1.0, -1.0) };
    V2F_C4B_T2F b = {CocVec2(pos.x - radius, pos.y + radius), Color4B(color), Tex2F(-1.0,  1.0) };
    V2F_C4B_T2F c = {CocVec2(pos.x + radius, pos.y + radius), Color4B(color), Tex2F( 1.0,  1.0) };
    V2F_C4B_T2F d = {CocVec2(pos.x + radius, pos.y - radius), Color4B(color), Tex2F( 1.0, -1.0) };
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    V2F_C4B_T2F_Triangle triangle1 = {a, c, d};
    triangles[0] = triangle0;
    triangles[1] = triangle1;
    
    _bufferCount += vertex_count;
    
    _dirty = true;
}

void DrawLayer::drawRect(const CocVec2 &p1, const CocVec2 &p2, const CocVec2 &p3, const CocVec2& p4, const Color4F &color)
{
    if( _block )
        return;
    drawLine(CocVec2(p1.x, p1.y), CocVec2(p2.x, p2.y), color);
    drawLine(CocVec2(p2.x, p2.y), CocVec2(p3.x, p3.y), color);
    drawLine(CocVec2(p3.x, p3.y), CocVec2(p4.x, p4.y), color);
    drawLine(CocVec2(p4.x, p4.y), CocVec2(p1.x, p1.y), color);
}

void DrawLayer::drawSegment(const CocVec2 &from, const CocVec2 &to, float radius, const Color4F &color)
{
    if( _block )
        return;
    unsigned int vertex_count = 6*3;
    ensureCapacity(vertex_count);
    
    CocVec2 a = __v2f(from);
    CocVec2 b = __v2f(to);
    
    
    CocVec2 n = v2fnormalize(v2fperp(v2fsub(b, a)));
    CocVec2 t = v2fperp(n);
    
    CocVec2 nw = v2fmult(n, radius);
    CocVec2 tw = v2fmult(t, radius);
    CocVec2 v0 = v2fsub(b, v2fadd(nw, tw));
    CocVec2 v1 = v2fadd(b, v2fsub(nw, tw));
    CocVec2 v2 = v2fsub(b, nw);
    CocVec2 v3 = v2fadd(b, nw);
    CocVec2 v4 = v2fsub(a, nw);
    CocVec2 v5 = v2fadd(a, nw);
    CocVec2 v6 = v2fsub(a, v2fsub(nw, tw));
    CocVec2 v7 = v2fadd(a, v2fadd(nw, tw));
    
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    
    V2F_C4B_T2F_Triangle triangles0 = {
        {v0, Color4B(color), __t(v2fneg(v2fadd(n, t)))},
        {v1, Color4B(color), __t(v2fsub(n, t))},
        {v2, Color4B(color), __t(v2fneg(n))},
    };
    triangles[0] = triangles0;
    
    V2F_C4B_T2F_Triangle triangles1 = {
        {v3, Color4B(color), __t(n)},
        {v1, Color4B(color), __t(v2fsub(n, t))},
        {v2, Color4B(color), __t(v2fneg(n))},
    };
    triangles[1] = triangles1;
    
    V2F_C4B_T2F_Triangle triangles2 = {
        {v3, Color4B(color), __t(n)},
        {v4, Color4B(color), __t(v2fneg(n))},
        {v2, Color4B(color), __t(v2fneg(n))},
    };
    triangles[2] = triangles2;

    V2F_C4B_T2F_Triangle triangles3 = {
        {v3, Color4B(color), __t(n)},
        {v4, Color4B(color), __t(v2fneg(n))},
        {v5, Color4B(color), __t(n) },
    };
    triangles[3] = triangles3;

    V2F_C4B_T2F_Triangle triangles4 = {
        {v6, Color4B(color), __t(v2fsub(t, n))},
        {v4, Color4B(color), __t(v2fneg(n)) },
        {v5, Color4B(color), __t(n)},
    };
    triangles[4] = triangles4;

    V2F_C4B_T2F_Triangle triangles5 = {
        {v6, Color4B(color), __t(v2fsub(t, n))},
        {v7, Color4B(color), __t(v2fadd(n, t))},
        {v5, Color4B(color), __t(n)},
    };
    triangles[5] = triangles5;
    
    _bufferCount += vertex_count;
    
    _dirty = true;
}

void DrawLayer::drawPolygon(const CocVec2 *verts, int count, const Color4F &fillColor, float borderWidth, const Color4F &borderColor)
{
    if( _block )
        return;
    CCASSERT(count >= 0, "invalid count value");
    
    bool outline = (borderColor.a > 0.0f && borderWidth > 0.0f);
    
    auto  triangle_count = outline ? (3*count - 2) : (count - 2);
    auto vertex_count = 3*triangle_count;
    ensureCapacity(vertex_count);
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle *cursor = triangles;
    
    for (int i = 0; i < count-2; i++)
    {
        V2F_C4B_T2F_Triangle tmp = {
            {verts[0], Color4B(fillColor), __t(v2fzero)},
            {verts[i+1], Color4B(fillColor), __t(v2fzero)},
            {verts[i+2], Color4B(fillColor), __t(v2fzero)},
        };
        
        *cursor++ = tmp;
    }
    
    if(outline)
    {
        struct ExtrudeVerts {CocVec2 offset, n;};
        struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts)*count);
        memset(extrude, 0, sizeof(struct ExtrudeVerts)*count);
        
        for (int i = 0; i < count; i++)
        {
            CocVec2 v0 = __v2f(verts[(i-1+count)%count]);
            CocVec2 v1 = __v2f(verts[i]);
            CocVec2 v2 = __v2f(verts[(i+1)%count]);
            
            CocVec2 n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
            CocVec2 n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));
            
            CocVec2 offset = v2fmult(v2fadd(n1, n2), 1.0f / (v2fdot(n1, n2) + 1.0f));
            struct ExtrudeVerts tmp = {offset, n2};
            extrude[i] = tmp;
        }
        
        for(int i = 0; i < count; i++)
        {
            int j = (i+1)%count;
            CocVec2 v0 = __v2f(verts[i]);
            CocVec2 v1 = __v2f(verts[j]);
            
            CocVec2 n0 = extrude[i].n;
            
            CocVec2 offset0 = extrude[i].offset;
            CocVec2 offset1 = extrude[j].offset;
            
            CocVec2 inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
            CocVec2 inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
            CocVec2 outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
            CocVec2 outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));
            
            V2F_C4B_T2F_Triangle tmp1 = {
                {inner0, Color4B(borderColor), __t(v2fneg(n0))},
                {inner1, Color4B(borderColor), __t(v2fneg(n0))},
                {outer1, Color4B(borderColor), __t(n0)}
            };
            *cursor++ = tmp1;
            
            V2F_C4B_T2F_Triangle tmp2 = {
                {inner0, Color4B(borderColor), __t(v2fneg(n0))},
                {outer0, Color4B(borderColor), __t(n0)},
                {outer1, Color4B(borderColor), __t(n0)}
            };
            *cursor++ = tmp2;
        }
        
        free(extrude);
    }
    
    _bufferCount += vertex_count;
    
    _dirty = true;
}

void DrawLayer::drawSolidRect(const CocVec2 &origin, const CocVec2 &destination, const Color4F &color)
{
    if( _block )
        return;
    CocVec2 vertices[] = {
        origin,
        CocVec2(destination.x, origin.y),
        destination,
        CocVec2(origin.x, destination.y)
    };
    
    drawSolidPoly(vertices, 4, color );
}

void DrawLayer::drawSolidPoly(const CocVec2 *poli, unsigned int numberOfPoints, const Color4F &color)
{
    if( _block )
        return;
    drawPolygon(poli, numberOfPoints, color, 0.0, Color4F(0.0, 0.0, 0.0, 0.0));
}

void DrawLayer::drawSolidCircle(const CocVec2& center, float radius, float angle, unsigned int segments, float scaleX, float scaleY, const Color4F &color)
{
    if( _block )
        return;
    const float coef = 2.0f * (float)M_PI/segments;
    
    CocVec2 *vertices = new (std::nothrow) CocVec2[segments];
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i < segments; i++)
    {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) * scaleX + center.x;
        GLfloat k = radius * sinf(rads + angle) * scaleY + center.y;
        
        vertices[i].x = j;
        vertices[i].y = k;
    }
    
    drawSolidPoly(vertices, segments, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawLayer::drawSolidCircle( const CocVec2& center, float radius, float angle, unsigned int segments, const Color4F& color)
{
    if( _block )
        return;
    drawSolidCircle(center, radius, angle, segments, 1.0f, 1.0f, color);
}

void DrawLayer::drawTriangle(const CocVec2 &p1, const CocVec2 &p2, const CocVec2 &p3, const Color4F &color)
{
    if( _block )
        return;
    unsigned int vertex_count = 3;
    ensureCapacity(vertex_count);

    Color4B col = Color4B(color);
    V2F_C4B_T2F a = {CocVec2(p1.x, p1.y), col, Tex2F(0.0, 0.0) };
    V2F_C4B_T2F b = {CocVec2(p2.x, p2.y), col, Tex2F(0.0,  0.0) };
    V2F_C4B_T2F c = {CocVec2(p3.x, p3.y), col, Tex2F(0.0,  0.0) };

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle = {a, b, c};
    triangles[0] = triangle;

    _bufferCount += vertex_count;
    _dirty = true;
}

void DrawLayer::drawQuadraticBezier(const CocVec2& from, const CocVec2& control, const CocVec2& to, unsigned int segments, const Color4F &color)
{
    if( _block )
        return;
    drawQuadBezier(from, control, to, segments, color);
}

void DrawLayer::clear()
{
    if( _block )
        return;
    _bufferCount = 0;
    _dirty = true;
    _bufferCountGLLine = 0;
    _dirtyGLLine = true;
    _bufferCountGLPoint = 0;
    _dirtyGLPoint = true;
    _lineWidth = _defaultLineWidth;
}

const BlendFunc& DrawLayer::getBlendFunc() const
{
    return _blendFunc;
}

void DrawLayer::setBlendFunc(const BlendFunc &blendFunc)
{
    if( _block )
        return;
    _blendFunc = blendFunc;
}

    DrawLayer *DrawLayer::clone()
    {
        DrawLayer *pResule = DrawLayer::create();
        //Node
        pResule->_rotationX = this->_rotationX;
        pResule->_rotationY = this->_rotationY;

        pResule->_rotationZ_X = this->_rotationZ_X;
        pResule->_rotationZ_Y = this->_rotationZ_Y;

        pResule->_rotationQuat = this->_rotationQuat;

        pResule->_scaleX = this->_scaleX;
        pResule->_scaleY = this->_scaleY;
        pResule->_scaleZ = this->_scaleZ;

        pResule->_position = this->_position;
        pResule->_positionZ = this->_positionZ;
        pResule->_normalizedPosition = this->_normalizedPosition;
        pResule->_usingNormalizedPosition = this->_usingNormalizedPosition;
        pResule->_normalizedPositionDirty = this->_normalizedPositionDirty;

        pResule->_skewX = this->_skewX;
        pResule->_skewY = this->_skewY;

        pResule->_anchorPointInPoints = this->_anchorPointInPoints;
        pResule->_anchorPoint = this->_anchorPoint;

        pResule->_contentSize = this->_contentSize;
        pResule->_contentSizeDirty = this->_contentSizeDirty;

        pResule->_modelViewTransform = this->_modelViewTransform;
        pResule->_transform = this->_transform;
        pResule->_transformDirty = this->_transformDirty;
        pResule->_inverse = this->_inverse;
        pResule->_inverseDirty = this->_inverseDirty;
        if( this->_additionalTransform) {
            pResule->_additionalTransform = new CocMat4[2];
            memcpy(pResule->_additionalTransform, this->_additionalTransform, sizeof(CocMat4) * 2);
        }
        pResule->_additionalTransformDirty = this->_additionalTransformDirty;
        pResule->_transformUpdated = this->_transformUpdated;
        pResule->_orderOfArrival = this->_orderOfArrival;
        pResule->_localZOrder = this->_localZOrder;
        pResule->_globalZOrder = this->_globalZOrder;
        pResule->s_globalOrderOfArrival = this->s_globalOrderOfArrival;
        pResule->_children = this->_children;
        pResule->_parent = this->_parent;
        pResule->_director = this->_director;
        pResule->_tag = this->_tag;
        pResule->_name = this->_name;
        pResule->_hashOfName = this->_hashOfName;
        pResule->_userData = this->_userData;
        pResule->_userObject = this->_userObject;
        pResule->_glProgramState = this->_glProgramState;
        pResule->_scheduler = this->_scheduler;
        pResule->_actionManager = this->_actionManager;
        pResule->_eventDispatcher = this->_eventDispatcher;
        pResule->_running = this->_running;
        pResule->_visible = this->_visible;
        pResule->_block = this->_block;
        pResule->_ignoreAnchorPointForPosition = this->_ignoreAnchorPointForPosition;
        pResule->_reorderChildDirty = this->_reorderChildDirty;
        pResule->_isTransitionFinished = this->_isTransitionFinished;
        pResule->_componentContainer = this->_componentContainer;
        pResule->_displayedOpacity = this->_displayedOpacity;
        pResule->_realOpacity = this->_realOpacity;
        pResule->_displayedColor = this->_displayedColor;
        pResule->_realColor = this->_realColor;
        pResule->_cascadeColorEnabled = this->_cascadeColorEnabled;
        pResule->_cascadeOpacityEnabled = this->_cascadeOpacityEnabled;
        pResule->_cameraMask = this->_cameraMask;
        pResule->_onEnterCallback = this->_onEnterCallback;
        pResule->_onExitCallback = this->_onExitCallback;
        pResule->_onEnterTransitionDidFinishCallback = this->_onEnterTransitionDidFinishCallback;
        pResule->_onExitTransitionDidStartCallback = this->_onExitTransitionDidStartCallback;
        pResule->__attachedNodeCount = this->__attachedNodeCount;



        // DrayLayer
        pResule->ensureCapacity(_bufferCapacity);
        pResule->_bufferCount = this->_bufferCount;

        if( pResule->_bufferCount > 0) {
            memcpy(pResule->_buffer, this->_buffer, sizeof(V2F_C4B_T2F) * _bufferCount);
            pResule->_dirty = true;
        } else{
            pResule->_dirty = this->_dirty;
        }

        pResule->ensureCapacity(_bufferCapacityGLLine);
        pResule->_bufferCountGLPoint = this->_bufferCountGLPoint;
        if(pResule->_bufferCountGLPoint > 0){
            memcpy(pResule->_bufferGLPoint, this->_bufferGLPoint, sizeof(V2F_C4B_T2F) * _bufferCountGLPoint);
            pResule->_dirtyGLPoint = true;
        }else {
            pResule->_dirtyGLPoint = this->_dirtyGLPoint;
        }

        pResule->ensureCapacity(_bufferCapacityGLLine);
        pResule->_bufferCountGLLine = this->_bufferCountGLLine;
        if( pResule->_bufferCountGLLine > 0 ){
            memcpy(pResule->_bufferGLLine, this->_bufferGLLine, sizeof(V2F_C4B_T2F) * _bufferCountGLLine);
            pResule->_dirtyGLLine = true;
        }else {
            pResule->_dirtyGLLine = this->_dirtyGLLine;
        }

        pResule->_blendFunc = this->_blendFunc;
        pResule->_customCommand = this->_customCommand;
        pResule->_customCommandGLPoint = this->_customCommandGLPoint;
        pResule->_customCommandGLLine = this->_customCommandGLLine;

        pResule->_lineWidth = this->_lineWidth;
        pResule->_defaultLineWidth = _defaultLineWidth;


//       memcpy(pResule, this, sizeof(DrawLayer));
        return pResule;
    }

void DrawLayer::setLineWidth(GLfloat lineWidth)
{
    if( _block )
        return;
    _lineWidth = lineWidth;
}

    void DrawLayer::drawTest()
    {
//        mh_drawNormalLine(0, -262.22223, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, -214.07408, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, -110.37037, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, 7.4074073, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, 144.44444, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(2, 247.40741, 100, 50, 0xff00ff00, 2);
    }

GLfloat DrawLayer::getLineWidth()
{
    return this->_lineWidth;
}


NS_CC_END

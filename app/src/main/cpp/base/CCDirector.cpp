// cocos2d includes
#include "base/CCDirector.h"

// standard includes
#include <string>
#include <draw/LayerManager.h>

#include "2d/CCDrawingPrimitives.h"
#include "2d/CCSpriteFrameCache.h"
#include "platform/CCFileUtils.h"

#include "2d/CCActionManager.h"
#include "2d/CCFontFNT.h"
#include "2d/CCFontAtlasCache.h"
#include "2d/CCAnimationCache.h"
#include "2d/CCTransition.h"
#include "2d/CCFontFreeType.h"
#include "2d/CCLabelAtlas.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCGLProgramStateCache.h"
#include "renderer/CCTextureCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCRenderState.h"
#include "renderer/CCFrameBuffer.h"
#include "2d/CCCamera.h"
#include "base/CCUserDefault.h"
#include "base/ccFPSImages.h"
#include "base/CCScheduler.h"
#include "base/ccMacros.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventCustom.h"
#include "base/CCConsole.h"
#include "base/CCAutoreleasePool.h"
#include "base/CCConfiguration.h"
#include "base/CCAsyncTaskPool.h"
#include "base/ObjectFactory.h"

/**
 Position of the FPS
 
 Default: 0,0 (bottom-left corner)
 */
#ifndef CC_DIRECTOR_STATS_POSITION
#define CC_DIRECTOR_STATS_POSITION Director::getInstance()->getVisibleOrigin()
#endif // CC_DIRECTOR_STATS_POSITION

using namespace std;

NS_CC_BEGIN
// FIXME: it should be a Director ivar. Move it there once support for multiple directors is added

// singleton stuff
static Director *s_SharedDirector = nullptr;

#define kDefaultFPS        60  // 60 frames per second
extern const char* cocos2dVersion(void);

const char *Director::EVENT_BEFORE_SET_NEXT_SCENE = "director_before_set_next_scene";
const char *Director::EVENT_AFTER_SET_NEXT_SCENE = "director_after_set_next_scene";
const char *Director::EVENT_PROJECTION_CHANGED = "director_projection_changed";
const char *Director::EVENT_AFTER_DRAW = "director_after_draw";
const char *Director::EVENT_AFTER_VISIT = "director_after_visit";
const char *Director::EVENT_BEFORE_UPDATE = "director_before_update";
const char *Director::EVENT_AFTER_UPDATE = "director_after_update";
const char *Director::EVENT_RESET = "director_reset";
const char *Director::EVENT_BEFORE_DRAW = "director_before_draw";

Director* Director::getInstance()
{
    if (!s_SharedDirector)
    {
        s_SharedDirector = new (std::nothrow) Director;
        CCASSERT(s_SharedDirector, "FATAL: Not enough memory");
        s_SharedDirector->init();
    }

    return s_SharedDirector;
}

Director::Director()
: _isStatusLabelUpdated(true)
, _invalid(true)
, _deltaTimePassedByCaller(false)
{
}

bool Director::init(void)
{
    setDefaultValues();

    _layerManager = nullptr;

    _notificationNode = nullptr;

    // FPS
    _accumDt = 0.0f;
    _frameRate = 0.0f;
    _FPSLabel = _drawnBatchesLabel = _drawnVerticesLabel = nullptr;
    _totalFrames = 0;
    _lastUpdate = std::chrono::steady_clock::now();
    
    _secondsPerFrame = 1.0f;
    _frames = 0;

    // paused ?
    _paused = false;

    // purge ?
    _purgeDirectorInNextLoop = false;
    
    // restart ?
    _restartDirectorInNextLoop = false;
    
    // invalid ?
    _invalid = false;

    _winSizeInPoints = Size::ZERO;

    _openGLView = nullptr;
    _defaultFBO = nullptr;
    
    _contentScaleFactor = 1.0f;

    // scheduler
    _scheduler = new (std::nothrow) Scheduler();
    // action manager
    _actionManager = new (std::nothrow) ActionManager();
    _scheduler->scheduleUpdate(_actionManager, Scheduler::PRIORITY_SYSTEM, false);

    _eventDispatcher = new (std::nothrow) EventDispatcher();
    
    _beforeSetNextScene = new (std::nothrow) EventCustom(EVENT_BEFORE_SET_NEXT_SCENE);
    _beforeSetNextScene->setUserData(this);
    _afterSetNextScene = new (std::nothrow) EventCustom(EVENT_AFTER_SET_NEXT_SCENE);
    _afterSetNextScene->setUserData(this);
    _eventAfterDraw = new (std::nothrow) EventCustom(EVENT_AFTER_DRAW);
    _eventAfterDraw->setUserData(this);
    _eventBeforeDraw = new (std::nothrow) EventCustom(EVENT_BEFORE_DRAW);
    _eventBeforeDraw->setUserData(this);
    _eventAfterVisit = new (std::nothrow) EventCustom(EVENT_AFTER_VISIT);
    _eventAfterVisit->setUserData(this);
    _eventBeforeUpdate = new (std::nothrow) EventCustom(EVENT_BEFORE_UPDATE);
    _eventBeforeUpdate->setUserData(this);
    _eventAfterUpdate = new (std::nothrow) EventCustom(EVENT_AFTER_UPDATE);
    _eventAfterUpdate->setUserData(this);
    _eventProjectionChanged = new (std::nothrow) EventCustom(EVENT_PROJECTION_CHANGED);
    _eventProjectionChanged->setUserData(this);
    _eventResetDirector = new (std::nothrow) EventCustom(EVENT_RESET);
    //init TextureCache
    initTextureCache();
    initMatrixStack();

    _renderer = new (std::nothrow) CocRenderer;
    RenderState::initialize();

    return true;
}

Director::~Director(void)
{
    CCLOGINFO("deallocing Director: %p", this);

    CC_SAFE_RELEASE(_FPSLabel);
    CC_SAFE_RELEASE(_drawnVerticesLabel);
    CC_SAFE_RELEASE(_drawnBatchesLabel);

    CC_SAFE_RELEASE(_layerManager);
    CC_SAFE_RELEASE(_notificationNode);
    CC_SAFE_RELEASE(_scheduler);
    CC_SAFE_RELEASE(_actionManager);
    CC_SAFE_DELETE(_defaultFBO);

    CC_SAFE_RELEASE(_beforeSetNextScene);
    CC_SAFE_RELEASE(_afterSetNextScene);
    CC_SAFE_RELEASE(_eventBeforeUpdate);
    CC_SAFE_RELEASE(_eventAfterUpdate);
    CC_SAFE_RELEASE(_eventAfterDraw);
    CC_SAFE_RELEASE(_eventBeforeDraw);
    CC_SAFE_RELEASE(_eventAfterVisit);
    CC_SAFE_RELEASE(_eventProjectionChanged);
    CC_SAFE_RELEASE(_eventResetDirector);

    delete _renderer;

    CC_SAFE_RELEASE(_eventDispatcher);
    
    Configuration::destroyInstance();
    ObjectFactory::destroyInstance();

    s_SharedDirector = nullptr;
}

void Director::setDefaultValues(void)
{
    Configuration *conf = Configuration::getInstance();

    // default FPS
    double fps = conf->getValue("cocos2d.x.fps", Value(kDefaultFPS)).asDouble();
    _oldAnimationInterval = _animationInterval = 1.0 / fps;

    // Display FPS
    _displayStats = conf->getValue("cocos2d.x.display_fps", Value(false)).asBool();

    // GL projection
    std::string projection = conf->getValue("cocos2d.x.gl.projection", Value("3d")).asString();
    if (projection == "3d")
        _projection = Projection::_3D;
    else if (projection == "2d")
        _projection = Projection::_2D;
    else if (projection == "custom")
        _projection = Projection::CUSTOM;
    else
        CCASSERT(false, "Invalid projection value");

    // Default pixel format for PNG images with alpha
    std::string pixel_format = conf->getValue("cocos2d.x.texture.pixel_format_for_png", Value("rgba8888")).asString();
    if (pixel_format == "rgba8888")
        Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
    else if(pixel_format == "rgba4444")
        Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
    else if(pixel_format == "rgba5551")
        Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB5A1);

    // PVR v2 has alpha premultiplied ?
    bool pvr_alpha_premultiplied = conf->getValue("cocos2d.x.texture.pvrv2_has_alpha_premultiplied", Value(false)).asBool();
    Image::setPVRImagesHavePremultipliedAlpha(pvr_alpha_premultiplied);
}

void Director::setGLDefaultValues()
{
    // This method SHOULD be called only after openGLView_ was initialized
    CCASSERT(_openGLView, "opengl view should not be null");

    setAlphaBlending(true);
    setDepthTest(false);
    setProjection(_projection);
}

// Draw the Scene
void Director::drawScene()
{
    // calculate "global" dt
    calculateDeltaTime();
    
    if (_openGLView)
    {
        _openGLView->pollEvents();
    }

    //tick before glClear: issue #533
    if (! _paused)
    {
        _eventDispatcher->dispatchEvent(_eventBeforeUpdate);
        _scheduler->update(_deltaTime);
        _eventDispatcher->dispatchEvent(_eventAfterUpdate);
    }


//    _renderer->clear();
    experimental::FrameBuffer::clearAllFBOs();
    
    _eventDispatcher->dispatchEvent(_eventBeforeDraw);

    pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    if (_layerManager)
    {
        //clear draw stats
        _renderer->clearDrawStats();
        
        //render the scene
        if(_openGLView)
            _openGLView->renderScene(_layerManager, _renderer);
        
        _eventDispatcher->dispatchEvent(_eventAfterVisit);
    }

    // draw the notifications node
    if (_notificationNode)
    {
        _notificationNode->visit(_renderer, CocMat4::IDENTITY, 0);
    }

    updateFrameRate();
    
    if (_displayStats)
    {
        showStats();
    }
    
    _renderer->render();

    _eventDispatcher->dispatchEvent(_eventAfterDraw);

    popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    _totalFrames++;

    // swap buffers
    if (_openGLView)
    {
        _openGLView->swapBuffers();
    }

    if (_displayStats)
    {
        calculateMPF();
    }
}

void Director::calculateDeltaTime()
{
    // new delta time. Re-fixed issue #1277
    if (_nextDeltaTimeZero)
    {
        _deltaTime = 0;
        _nextDeltaTimeZero = false;
    }
    else
    {
        // delta time may passed by invoke mainLoop(dt)
        if (!_deltaTimePassedByCaller)
        {
            auto now = std::chrono::steady_clock::now();
            _deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastUpdate).count() / 1000000.0f;
            _lastUpdate = now;
        }
        _deltaTime = MAX(0, _deltaTime);
    }
    return;
}

float Director::getDeltaTime() const
{
    return _deltaTime;
}
void Director::setOpenGLView(GLView *openGLView)
{
    CCASSERT(openGLView, "opengl view should not be null");

    if (_openGLView != openGLView)
    {
        // Configuration. Gather GPU info
        Configuration *conf = Configuration::getInstance();
        conf->gatherGPUInfo();
        CCLOG("%s\n",conf->getInfo().c_str());

        if(_openGLView)
            _openGLView->release();
        _openGLView = openGLView;
        _openGLView->retain();

        // set size
        _winSizeInPoints = _openGLView->getDesignResolutionSize();

        _isStatusLabelUpdated = true;

        if (_openGLView)
        {
            setGLDefaultValues();
        }

        _renderer->initGLView();

        CHECK_GL_ERROR_DEBUG();

        if (_eventDispatcher)
        {
            _eventDispatcher->setEnabled(true);
        }
        
//        _defaultFBO = experimental::FrameBuffer::getOrCreateDefaultFBO(_openGLView);
//        _defaultFBO->retain();
    }
}

TextureCache* Director::getTextureCache() const
{
    return _textureCache;
}

void Director::initTextureCache()
{
    _textureCache = new (std::nothrow) TextureCache();
}

void Director::destroyTextureCache()
{
    if (_textureCache)
    {
        _textureCache->waitForQuit();
        CC_SAFE_RELEASE_NULL(_textureCache);
    }
}

void Director::setViewport()
{
    if (_openGLView)
    {
        _openGLView->setViewPortInPoints(0, 0, _winSizeInPoints.width, _winSizeInPoints.height);
    }
}

void Director::setNextDeltaTimeZero(bool nextDeltaTimeZero)
{
    _nextDeltaTimeZero = nextDeltaTimeZero;
}

void Director::initMatrixStack()
{
    while (!_modelViewMatrixStack.empty())
    {
        _modelViewMatrixStack.pop();
    }

    _projectionMatrixStackList.clear();

    while (!_textureMatrixStack.empty())
    {
        _textureMatrixStack.pop();
    }

    _modelViewMatrixStack.push(CocMat4::IDENTITY);
    std::stack<CocMat4> projectionMatrixStack;
    projectionMatrixStack.push(CocMat4::IDENTITY);
    _projectionMatrixStackList.push_back(projectionMatrixStack);
    _textureMatrixStack.push(CocMat4::IDENTITY);
}

void Director::resetMatrixStack()
{
    initMatrixStack();
}

void Director::initProjectionMatrixStack(size_t stackCount)
{
    _projectionMatrixStackList.clear();
    std::stack<CocMat4> projectionMatrixStack;
    projectionMatrixStack.push(CocMat4::IDENTITY);
    for (size_t i = 0; i < stackCount; ++i)
        _projectionMatrixStackList.push_back(projectionMatrixStack);
}

size_t Director::getProjectionMatrixStackSize()
{
    return _projectionMatrixStackList.size();
}

void Director::popMatrix(MATRIX_STACK_TYPE type)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.pop();
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStackList[0].pop();
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.pop();
    }
    else
    {
        CCASSERT(false, "unknown matrix stack type");
    }
}

void Director::popProjectionMatrix(size_t index)
{
    _projectionMatrixStackList[index].pop();
}

void Director::loadIdentityMatrix(MATRIX_STACK_TYPE type)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.top() = CocMat4::IDENTITY;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStackList[0].top() = CocMat4::IDENTITY;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.top() = CocMat4::IDENTITY;
    }
    else
    {
        CCASSERT(false, "unknown matrix stack type");
    }
}

void Director::loadProjectionIdentityMatrix(size_t index)
{
    _projectionMatrixStackList[index].top() = CocMat4::IDENTITY;
}

void Director::loadMatrix(MATRIX_STACK_TYPE type, const CocMat4& mat)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.top() = mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStackList[0].top() = mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.top() = mat;
    }
    else
    {
        CCASSERT(false, "unknown matrix stack type");
    }
}

void Director::loadProjectionMatrix(const CocMat4& mat, size_t index)
{
    _projectionMatrixStackList[index].top() = mat;
}

void Director::multiplyMatrix(MATRIX_STACK_TYPE type, const CocMat4& mat)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.top() *= mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStackList[0].top() *= mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.top() *= mat;
    }
    else
    {
        CCASSERT(false, "unknown matrix stack type");
    }
}

void Director::multiplyProjectionMatrix(const CocMat4& mat, size_t index)
{
    _projectionMatrixStackList[index].top() *= mat;
}

void Director::pushMatrix(MATRIX_STACK_TYPE type)
{
    if(type == MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW)
    {
        _modelViewMatrixStack.push(_modelViewMatrixStack.top());
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION)
    {
        _projectionMatrixStackList[0].push(_projectionMatrixStackList[0].top());
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE)
    {
        _textureMatrixStack.push(_textureMatrixStack.top());
    }
    else
    {
        CCASSERT(false, "unknown matrix stack type");
    }
}

void Director::pushProjectionMatrix(size_t index)
{
    _projectionMatrixStackList[index].push(_projectionMatrixStackList[index].top());
}

const CocMat4& Director::getMatrix(MATRIX_STACK_TYPE type) const
{
    if(type == MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW)
    {
        return _modelViewMatrixStack.top();
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION)
    {
        return _projectionMatrixStackList[0].top();
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE)
    {
        return _textureMatrixStack.top();
    }

    CCASSERT(false, "unknown matrix stack type, will return modelview matrix instead");
    return  _modelViewMatrixStack.top();
}

const CocMat4& Director::getProjectionMatrix(size_t index) const
{
    return _projectionMatrixStackList[index].top();
}

void Director::setProjection(Projection projection)
{
    Size size = _winSizeInPoints;

    if (size.width == 0 || size.height == 0)
    {
        CCLOGERROR("cocos2d: warning, Director::setProjection() failed because size is 0");
        return;
    }

    setViewport();

    switch (projection)
    {
        case Projection::_2D:
        {
            CocMat4 orthoMatrix;
            CocMat4::createOrthographicOffCenter(0, size.width, 0, size.height, -1024, 1024, &orthoMatrix);
            loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, orthoMatrix);
            loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
            break;
        }
            
        case Projection::_3D:
        {
            float zeye = this->getZEye();

            CocMat4 matrixPerspective, matrixLookup;

            // issue #1334
            CocMat4::createPerspective(60, (GLfloat)size.width/size.height, 10, zeye+size.height/2, &matrixPerspective);

            CocVec3 eye(size.width/2, size.height/2, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
            CocMat4::createLookAt(eye, center, up, &matrixLookup);
            CocMat4 proj3d = matrixPerspective * matrixLookup;

            loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, proj3d);
            loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
            break;
        }

        case Projection::CUSTOM:
            // Projection Delegate is no longer needed
            // since the event "PROJECTION CHANGED" is emitted
            break;

        default:
            CCLOG("cocos2d: Director: unrecognized projection");
            break;
    }

    _projection = projection;
    GL::setProjectionMatrixDirty();

    _eventDispatcher->dispatchEvent(_eventProjectionChanged);
}

void Director::purgeCachedData(void)
{
    FontFNT::purgeCachedData();
    FontAtlasCache::purgeCachedData();

    if (s_SharedDirector->getOpenGLView())
    {
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        _textureCache->removeUnusedTextures();

        // Note: some tests such as ActionsTest are leaking refcounted textures
        // There should be no test textures left in the cache
        log("%s\n", _textureCache->getCachedTextureInfo().c_str());
    }
    FileUtils::getInstance()->purgeCachedEntries();
}

float Director::getZEye(void) const
{
    return (_winSizeInPoints.height / 1.154700538379252f);//(2 * tanf(M_PI/6))
}

void Director::setAlphaBlending(bool on)
{
    if (on)
    {
        GL::blendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    }
    else
    {
        GL::blendFunc(GL_ONE, GL_ZERO);
    }

    CHECK_GL_ERROR_DEBUG();
}

void Director::setDepthTest(bool on)
{
    _renderer->setDepthTest(on);
}

void Director::setClearColor(const Color4F& clearColor)
{
    _renderer->setClearColor(clearColor);
    auto defaultFBO = experimental::FrameBuffer::getOrCreateDefaultFBO(_openGLView);
    
    if(defaultFBO) defaultFBO->setClearColor(clearColor);
}

static void GLToClipTransform(CocMat4 *transformOut)
{
    if(nullptr == transformOut) return;
    
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");

    auto projection = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    auto modelview = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    *transformOut = projection * modelview;
}

CocVec2 Director::convertToGL(const CocVec2& uiPoint)
{
    CocMat4 transform;
    GLToClipTransform(&transform);

    CocMat4 transformInv = transform.getInversed();

    // Calculate z=0 using -> transform*[0, 0, 0, 1]/w
    float zClip = transform.m[14]/transform.m[15];

    Size glSize = _openGLView->getDesignResolutionSize();
    CocVec4 clipCoord(2.0f*uiPoint.x/glSize.width - 1.0f, 1.0f - 2.0f*uiPoint.y/glSize.height, zClip, 1);

    CocVec4 glCoord;
    //transformInv.transformPoint(clipCoord, &glCoord);
    transformInv.transformVector(clipCoord, &glCoord);
    float factor = 1.0f / glCoord.w;
    return CocVec2(glCoord.x * factor, glCoord.y * factor);
}

CocVec2 Director::convertToUI(const CocVec2& glPoint)
{
    CocMat4 transform;
    GLToClipTransform(&transform);

    CocVec4 clipCoord;
    // Need to calculate the zero depth from the transform.
    CocVec4 glCoord(glPoint.x, glPoint.y, 0.0, 1);
    transform.transformVector(glCoord, &clipCoord);

	/*
	BUG-FIX #5506

	a = (Vx, Vy, Vz, 1)
	b = (a×M)T
	Out = 1 ⁄ bw(bx, by, bz)
	*/
	
	clipCoord.x = clipCoord.x / clipCoord.w;
	clipCoord.y = clipCoord.y / clipCoord.w;
	clipCoord.z = clipCoord.z / clipCoord.w;

    Size glSize = _openGLView->getDesignResolutionSize();
    float factor = 1.0f / glCoord.w;
    return CocVec2(glSize.width * (clipCoord.x * 0.5f + 0.5f) * factor, glSize.height * (-clipCoord.y * 0.5f + 0.5f) * factor);
}

const Size& Director::getWinSize(void) const
{
    return _winSizeInPoints;
}

Size Director::getWinSizeInPixels() const
{
    return Size(_winSizeInPoints.width * _contentScaleFactor, _winSizeInPoints.height * _contentScaleFactor);
}

Size Director::getVisibleSize() const
{
    if (_openGLView)
    {
        return _openGLView->getVisibleSize();
    }
    else
    {
        return Size::ZERO;
    }
}

CocVec2 Director::getVisibleOrigin() const
{
    if (_openGLView)
    {
        return _openGLView->getVisibleOrigin();
    }
    else
    {
        return CocVec2::ZERO;
    }
}

Rect Director::getSafeAreaRect() const
{
    if (_openGLView)
    {
        return _openGLView->getSafeAreaRect();
    }
    else
    {
        return Rect::ZERO;
    }
}

// scene management

void Director::runWithScene(Scene *scene)
{
    CCASSERT(scene != nullptr, "This command can only be used to start the Director. There is already a scene present.");
    CCASSERT(_layerManager == nullptr, "_layerManager should be null");

    _eventDispatcher->dispatchEvent(_beforeSetNextScene);
    _layerManager = scene;
    _layerManager->retain();
    _sendCleanupToScene = false;
    _layerManager->onEnter();
    _layerManager->onEnterTransitionDidFinish();

//    _eventDispatcher->dispatchEvent(_afterSetNextScene);

//    pushScene(scene);
}



void Director::end()
{
    _purgeDirectorInNextLoop = true;
}

void Director::restart()
{
    _restartDirectorInNextLoop = true;
}

void Director::reset()
{

    if (_layerManager)
    {
        _layerManager->onExit();
        _layerManager->cleanup();
        _layerManager->release();
    }
    
    _layerManager = nullptr;
//    _nextScene = nullptr;

    if (_eventDispatcher)
        _eventDispatcher->dispatchEvent(_eventResetDirector);
    
    // cleanup scheduler
    getScheduler()->unscheduleAll();
    
    // Remove all events
    if (_eventDispatcher)
    {
        _eventDispatcher->removeAllEventListeners();
    }
    
    if(_notificationNode)
    {
        _notificationNode->onExit();
        _notificationNode->cleanup();
        _notificationNode->release();
    }
    
    _notificationNode = nullptr;

    stopAnimation();
    
    CC_SAFE_RELEASE_NULL(_notificationNode);
    CC_SAFE_RELEASE_NULL(_FPSLabel);
    CC_SAFE_RELEASE_NULL(_drawnBatchesLabel);
    CC_SAFE_RELEASE_NULL(_drawnVerticesLabel);
    
    // purge bitmap cache
    FontFNT::purgeCachedData();
    FontAtlasCache::purgeCachedData();
    
    FontFreeType::shutdownFreeType();
    
    // purge all managed caches

    DrawPrimitives::free();
    AnimationCache::destroyInstance();
    SpriteFrameCache::destroyInstance();
    GLProgramCache::destroyInstance();
    GLProgramStateCache::destroyInstance();
    FileUtils::destroyInstance();
    AsyncTaskPool::destroyInstance();
    
    // cocos2d-x specific data structures
    UserDefault::destroyInstance();
    
    GL::invalidateStateCache();

    RenderState::finalize();
    
    destroyTextureCache();
}

void Director::purgeDirector()
{
    reset();

    CHECK_GL_ERROR_DEBUG();
    
    // OpenGL view
    if (_openGLView)
    {
        _openGLView->end();
        _openGLView = nullptr;
    }

    // delete Director
    release();
}

void Director::restartDirector()
{
    reset();
    
    // RenderState need to be reinitialized
    RenderState::initialize();

    // Texture cache need to be reinitialized
    initTextureCache();
    
    // Reschedule for action manager
    getScheduler()->scheduleUpdate(getActionManager(), Scheduler::PRIORITY_SYSTEM, false);
    
    // release the objects
    PoolManager::getInstance()->getCurrentPool()->clear();

    // Restart animation
    startAnimation();

}

void Director::pause()
{
    if (_paused)
    {
        return;
    }

    _oldAnimationInterval = _animationInterval;

    // when paused, don't consume CPU
    setAnimationInterval(1 / 4.0, SetIntervalReason::BY_DIRECTOR_PAUSE);
    _paused = true;
}

void Director::resume()
{
    if (! _paused)
    {
        return;
    }

    setAnimationInterval(_oldAnimationInterval, SetIntervalReason::BY_ENGINE);

    _paused = false;
    _deltaTime = 0;
    // fix issue #3509, skip one fps to avoid incorrect time calculation.
    setNextDeltaTimeZero(true);
}

void Director::updateFrameRate()
{
//    static const float FPS_FILTER = 0.1f;
//    static float prevDeltaTime = 0.016f; // 60FPS
//    
//    float dt = _deltaTime * FPS_FILTER + (1.0f-FPS_FILTER) * prevDeltaTime;
//    prevDeltaTime = dt;
//    _frameRate = 1.0f/dt;

    // Frame rate should be the real value of current frame.
    _frameRate = 1.0f / _deltaTime;
}


// display the FPS using a LabelAtlas
// updates the FPS every frame
void Director::showStats()
{
    if (_isStatusLabelUpdated)
    {
        createStatsLabel();
        _isStatusLabelUpdated = false;
    }

    static unsigned long prevCalls = 0;
    static unsigned long prevVerts = 0;

    ++_frames;
    _accumDt += _deltaTime;
    
    if (_displayStats && _FPSLabel && _drawnBatchesLabel && _drawnVerticesLabel)
    {
        char buffer[30] = {0};

        // Probably we don't need this anymore since
        // the framerate is using a low-pass filter
        // to make the FPS stable
        if (_accumDt > CC_DIRECTOR_STATS_INTERVAL)
        {
            sprintf(buffer, "%.1f / %.3f", _frames / _accumDt, _secondsPerFrame);
            _FPSLabel->setString(buffer);
            _accumDt = 0;
            _frames = 0;
        }

        auto currentCalls = (unsigned long)_renderer->getDrawnBatches();
        auto currentVerts = (unsigned long)_renderer->getDrawnVertices();
        if( currentCalls != prevCalls ) {
            sprintf(buffer, "GL calls:%6lu", currentCalls);
            _drawnBatchesLabel->setString(buffer);
            prevCalls = currentCalls;
        }

        if( currentVerts != prevVerts) {
            sprintf(buffer, "GL verts:%6lu", currentVerts);
            _drawnVerticesLabel->setString(buffer);
            prevVerts = currentVerts;
        }

        const CocMat4& identity = CocMat4::IDENTITY;
        _drawnVerticesLabel->visit(_renderer, identity, 0);
        _drawnBatchesLabel->visit(_renderer, identity, 0);
        _FPSLabel->visit(_renderer, identity, 0);
    }
}

void Director::calculateMPF()
{
    static float prevSecondsPerFrame = 0;
    static const float MPF_FILTER = 0.10f;

    _secondsPerFrame = _deltaTime * MPF_FILTER + (1-MPF_FILTER) * prevSecondsPerFrame;
    prevSecondsPerFrame = _secondsPerFrame;
}

// returns the FPS image data pointer and len
void Director::getFPSImageData(unsigned char** datapointer, ssize_t* length)
{
    // FIXME: fixed me if it should be used 
    *datapointer = cc_fps_images_png;
    *length = cc_fps_images_len();
}

void Director::createStatsLabel()
{
    Texture2D *texture = nullptr;
    std::string fpsString = "00.0";
    std::string drawBatchString = "000";
    std::string drawVerticesString = "00000";
    if (_FPSLabel)
    {
        fpsString = _FPSLabel->getString();
        drawBatchString = _drawnBatchesLabel->getString();
        drawVerticesString = _drawnVerticesLabel->getString();
        
        CC_SAFE_RELEASE_NULL(_FPSLabel);
        CC_SAFE_RELEASE_NULL(_drawnBatchesLabel);
        CC_SAFE_RELEASE_NULL(_drawnVerticesLabel);
        _textureCache->removeTextureForKey("/cc_fps_images");
        FileUtils::getInstance()->purgeCachedEntries();
    }

    Texture2D::PixelFormat currentFormat = Texture2D::getDefaultAlphaPixelFormat();
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
    unsigned char *data = nullptr;
    ssize_t dataLength = 0;
    getFPSImageData(&data, &dataLength);

    Image* image = new (std::nothrow) Image();
    bool isOK = image ? image->initWithImageData(data, dataLength) : false;
    if (! isOK) {
        if(image)
            delete image;
        CCLOGERROR("%s", "Fails: init fps_images");
        return;
    }

    texture = _textureCache->addImage(image, "/cc_fps_images");
    CC_SAFE_RELEASE(image);

    /*
     We want to use an image which is stored in the file named ccFPSImage.c 
     for any design resolutions and all resource resolutions. 
     
     To achieve this, we need to ignore 'contentScaleFactor' in 'AtlasNode' and 'LabelAtlas'.
     So I added a new method called 'setIgnoreContentScaleFactor' for 'AtlasNode',
     this is not exposed to game developers, it's only used for displaying FPS now.
     */
    float scaleFactor = 1 / CC_CONTENT_SCALE_FACTOR();

    _FPSLabel = LabelAtlas::create();
    _FPSLabel->retain();
    _FPSLabel->setIgnoreContentScaleFactor(true);
    _FPSLabel->initWithString(fpsString, texture, 12, 32 , '.');
    _FPSLabel->setScale(scaleFactor);

    _drawnBatchesLabel = LabelAtlas::create();
    _drawnBatchesLabel->retain();
    _drawnBatchesLabel->setIgnoreContentScaleFactor(true);
    _drawnBatchesLabel->initWithString(drawBatchString, texture, 12, 32, '.');
    _drawnBatchesLabel->setScale(scaleFactor);

    _drawnVerticesLabel = LabelAtlas::create();
    _drawnVerticesLabel->retain();
    _drawnVerticesLabel->setIgnoreContentScaleFactor(true);
    _drawnVerticesLabel->initWithString(drawVerticesString, texture, 12, 32, '.');
    _drawnVerticesLabel->setScale(scaleFactor);


    Texture2D::setDefaultAlphaPixelFormat(currentFormat);

    const int height_spacing = 22 / CC_CONTENT_SCALE_FACTOR();
    _drawnVerticesLabel->setPosition(CocVec2(0, height_spacing*2) + CC_DIRECTOR_STATS_POSITION);
    _drawnBatchesLabel->setPosition(CocVec2(0, height_spacing*1) + CC_DIRECTOR_STATS_POSITION);
    _FPSLabel->setPosition(CocVec2(0, height_spacing*0)+CC_DIRECTOR_STATS_POSITION);
}


void Director::setContentScaleFactor(float scaleFactor)
{
    if (scaleFactor != _contentScaleFactor)
    {
        _contentScaleFactor = scaleFactor;
        _isStatusLabelUpdated = true;
    }
}

void Director::setNotificationNode(Node *node)
{
	if (_notificationNode != nullptr){
		_notificationNode->onExitTransitionDidStart();
		_notificationNode->onExit();
		_notificationNode->cleanup();
	}
	CC_SAFE_RELEASE(_notificationNode);

	_notificationNode = node;
	if (node == nullptr)
		return;
	_notificationNode->onEnter();
	_notificationNode->onEnterTransitionDidFinish();
    CC_SAFE_RETAIN(_notificationNode);
}

void Director::setScheduler(Scheduler* scheduler)
{
    if (_scheduler != scheduler)
    {
        CC_SAFE_RETAIN(scheduler);
        CC_SAFE_RELEASE(_scheduler);
        _scheduler = scheduler;
    }
}

void Director::setActionManager(ActionManager* actionManager)
{
    if (_actionManager != actionManager)
    {
        CC_SAFE_RETAIN(actionManager);
        CC_SAFE_RELEASE(_actionManager);
        _actionManager = actionManager;
    }    
}

void Director::setEventDispatcher(EventDispatcher* dispatcher)
{
    if (_eventDispatcher != dispatcher)
    {
        CC_SAFE_RETAIN(dispatcher);
        CC_SAFE_RELEASE(_eventDispatcher);
        _eventDispatcher = dispatcher;
    }
}

void Director::startAnimation()
{
    startAnimation(SetIntervalReason::BY_ENGINE);
}

void Director::startAnimation(SetIntervalReason reason)
{
    _lastUpdate = std::chrono::steady_clock::now();

    _invalid = false;

    _cocos2d_thread_id = std::this_thread::get_id();


    // fix issue #3509, skip one fps to avoid incorrect time calculation.
    setNextDeltaTimeZero(true);
}

void Director::mainLoop()
{
    if (_purgeDirectorInNextLoop)
    {
        _purgeDirectorInNextLoop = false;
        purgeDirector();
    }
    else if (_restartDirectorInNextLoop)
    {
        _restartDirectorInNextLoop = false;
        restartDirector();
    }
    else if (! _invalid)
    {
//        mh_drawBlankCanvas(1.0, 0.5, 0.5);
//        mh_drawNormalLine(0, -262.22223, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, -214.07408, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, -110.37037, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, 7.4074073, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(1, 144.44444, 100, 50, 0xff00ff00, 2);
//        mh_drawNormalLine(2, 247.40741, 100, 50, 0xff00ff00, 2);
        drawScene();
//
     
        // release the objects
        PoolManager::getInstance()->getCurrentPool()->clear();
    }
}

void Director::mainLoop(float dt)
{
    _deltaTime = dt;
    _deltaTimePassedByCaller = true;
    mainLoop();
}

void Director::stopAnimation()
{
    _invalid = true;
}

void Director::setAnimationInterval(float interval)
{
    setAnimationInterval(interval, SetIntervalReason::BY_GAME);
}

void Director::setAnimationInterval(float interval, SetIntervalReason reason)
{
    _animationInterval = interval;
    if (! _invalid)
    {
        stopAnimation();
        startAnimation(reason);
    }
}

NS_CC_END


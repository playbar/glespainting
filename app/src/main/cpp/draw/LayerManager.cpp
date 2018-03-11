#include "LayerManager.h"
#include "VisibleRect.h"
#include "coclog.h"
#include "draw/curve.h"

LayerManager::LayerManager()
{
    mCurrentLayer = 0;
}

LayerID LayerManager::createLayer()
{
    LayerID layerID;
    glGenTextures(1, &layerID);
    glBindTexture(GL_TEXTURE_2D, layerID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _fbo_width, _fbo_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, 0);
    return layerID;
}

int LayerManager::deleteLayer(LayerID id)
{
    glDeleteTextures(1, &id);
    return -1;
}

int LayerManager::copyLayer(LayerID texid)
{
    LayerID layerID;
    glGenTextures(1, &layerID);
    glBindTexture(GL_TEXTURE_2D, layerID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _fbo_width, _fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, _fb0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, layerID, 0);


    glViewport(0, 0, _fbo_width, _fbo_height);
    glClearColor(1.0, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glBindBuffer(GL_ARRAY_BUFFER, _a0);
    GLint ptloc = glGetAttribLocation(mProgram, "pos");
    GLint tcloc = glGetAttribLocation(mProgram, "tcoord");
    glVertexAttribPointer(ptloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const GLvoid *) offsetof(Vertex, x));
    glVertexAttribPointer(tcloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const GLvoid *) offsetof(Vertex, u));
    glEnableVertexAttribArray(ptloc);
    glEnableVertexAttribArray(tcloc);

    glUseProgram(mProgram);
    GLint loc = glGetUniformLocation(mProgram, "mvp");
    glUniformMatrix4fv(loc, 1, GL_FALSE, transmat);

    glEnable(GL_TEXTURE_2D);

    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texid);
        GLint loc = glGetUniformLocation(mProgram, "flag");
        glUniform1i(loc, 2);
        loc = glGetUniformLocation(mProgram, "texture");
        glUniform1i(loc, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glDisableVertexAttribArray(ptloc);
    glDisableVertexAttribArray(tcloc);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D,0);

    return layerID;
}

int LayerManager::clearLayer(LayerID id)
{
    if(mCurrentLayer != 0 )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _fb0);
        glBindTexture(GL_TEXTURE_2D, mCurrentLayer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mCurrentLayer, 0);
        glViewport(0, 0, _fbo_width, _fbo_height);
        glClearColor(0, 0, 0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        return 1;
    }
    return -1;
}
int LayerManager::mergeLayer(LayerID *id1, LayerID *id2, int merge_mode)
{

    return -1;
}
int LayerManager::swapLayer(int layer1, int layer2)
{
    return -1;
}

LayerID LayerManager::getCurrentLayer(int id)
{
    return mCurrentLayer;
}
int LayerManager::setOpacity(float opacity)
{
    if( mCurrentLayer != NULL )
    {
//        mCurrentLayer->setOpacity( opacity);
        return 1;
    }
    return -1;
}
float LayerManager::getOpacity()
{
    if( mCurrentLayer != NULL ){
//        return mCurrentLayer->getOpacity();
    }
    return 0.0f;
}
int LayerManager::setVisiable(int id, bool visiable)
{
    if( mCurrentLayer != NULL )
    {
//        mCurrentLayer->setVisible( visiable);
        return 1;
    }
    return -1;
}
bool LayerManager::getVisiable(int id)
{
    if( mCurrentLayer != NULL )
    {
//        return mCurrentLayer->isVisible();
    }
    return false;
}
int LayerManager::setLock(int id, bool block)
{
    if( mCurrentLayer != NULL )
    {
//        mCurrentLayer->setLock(block);
        return 1;
    }
    return -1;
}
bool LayerManager::getLock(int id)
{
    if( mCurrentLayer != NULL )
//        return mCurrentLayer->isLock();
    return false;
}
int LayerManager::setTranslate(float x, float y, float z)
{
    if( mCurrentLayer != NULL )
    {
//        mCurrentLayer->setPosition( x, y );
        return 1;
    }
    return -1;
}
//vec3 getTranslate();//当前图层，获取当前图层的位置，返回图层所在的位置
int LayerManager::setScale(float fx, float fy, float scale)
{
    if(mCurrentLayer != NULL )
    {
//        mCurrentLayer->setScale(fx, fy);
        return 1;
    }
    return -1;
}
//vec3 getScale(); // 获取当前图层的缩放比例，
int LayerManager::setRotate(float cx, float cy, float angle)
{
    if( mCurrentLayer != NULL ) {
//        mCurrentLayer->setRotation(cx);
        return 1;
    }
    return -1;
}
//vec3 getRotate(); //获取当前旋转值
////////////////////////
int LayerManager::setAllTranslate(float x, float y, float z)
{

    return -1;
}

Vec3 LayerManager::getAllTranslate()
{
    return Vec3(0.0f, 0.0f, 0.0f);
}

int LayerManager::setAllScale(float cx, float cy, float scale)
{
    return -1;
}
Vec3 LayerManager::getAllScale()
{
    return Vec3(0.0f, 0.0f, 0.0f);
}

int LayerManager::setAllRotate(float cx, float cy, float angle)
{
    return -1;
}
Vec3 LayerManager::getAllRotate()
{
    return Vec3(0.0f, 0.0f, 0.0f);
}
/////////////
int LayerManager::setBlendMode(int mode)
{
    return -1;
}
int LayerManager::getBlendMode()
{
    return -1;
}
int LayerManager::setBackgroundColor(float r, float g, float b, float a)
{
    return -1;
}
Color4B LayerManager::getBackgroundColor()
{
    return Color4B();
}

int LayerManager::setBackgroundTexture(int texid)
{
    return -1;
}
int LayerManager::setBackgroundTexture(char *pdata)
{
    return -1;
}
int LayerManager::setBackgroundTexture(std::string texid)
{
    return -1;
}
int LayerManager::getBackgroundTexture()
{
    return -1;
}

int LayerManager::getLayerThumbnailData(int layerid)
{
    return -1;
}

int LayerManager::getThumbnailData()
{
    return -1;
}


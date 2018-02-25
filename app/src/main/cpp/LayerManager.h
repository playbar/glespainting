/*
 
 //创建图层
 #define CREATE_LAYER 800000
 //单图层行为 开始头定义
 #define SINGLE_LAYER_OP 880000
 //多图层行为 开始头定义
 #define MULT_LAYER_OP 890000
 //path  layerid 独立存储(兼容考虑)
 #define PATH_LAYERID_PREFIX 810000
 
 //图层的拷贝
 #define LAYER_ACTION_COPY   1
 //图层内容的清空
 #define LAYER_ACTION_CLEAR   2
 //图层  删除
 #define LAYER_ACTION_DELETE   3
 //图层  ALPHA 调节
 #define LAYER_ACTION_ALPHA   4
 //图层 的填充
 #define LAYER_ACTION_FILL_COLOR   5
 //图层移动
 #define LAYER_ACTION_TRAN   6
 //图层缩放
 #define LAYER_ACTION_SCALE   7
 //图层 的旋转
 #define LAYER_ACTION_RORATE   8
 //图层 的混合模式
 #define LAYER_ACTION_BLEND   9
 //图层 的混合模式
 #define LAYER_ACTION_VISIBLE  10
 //图层 REDO
 #define LAYER_ACTION_REDO  11
 //图层 UNDO
 #define LAYER_ACTION_UNDO  12
 //图层交换
 #define LAYER_ACTION_EXCHANGE  13
 //图层合并
 #define LAYER_ACTION_MERGE  14
 
 */

#ifndef __LAYER_MANAGER_H__
#define __LAYER_MANAGER_H__

#include "cocos2d.h"
#include "Caretaker.h"

using namespace cocos2d;

class LayerManager : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void drawTest();

    void createMenu();
    void menuCreateCallback(Ref *pSender);
    void menuDeleteCallback(Ref *pSender);
    void menuDrawTestCallback(Ref *pSender);
    void menuRotaCallback(Ref *pSender);
    void menuTranslateCallback(Ref *pSender);
    void menuCopyCallback(Ref *pSender);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
//    CREATE_FUNC(LayerManager);
    static LayerManager *create()
    {
        LayerManager *pRet = new(std::nothrow)LayerManager();
        if(pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
public:
    DrawLayer *createLayer(); //返回创建图层的id，发回UI层使用，成功返回从1开始的值，失败返回-1
    int deleteLayer(DrawLayer *id); //删除图层，成功返回1，失败返回-1；
    int copyLayer(DrawLayer *id);  //复制当前图层的内容到新图层，并且返回新图层的id，当前图层为新的图层。成功返回图层id， 失败返回-1。
    int clearLayer(int id);//清空当前图层内容，成功返回1， 失败返回-1；
    int mergeLayer(DrawLayer *id1, DrawLayer *id2, int merge_mode);// 合并图层, 相邻图层，根据alphe混合, 成功返回合并后的id， 失败返回-1
    int swapLayer(int layer1, int layer2);//交换图层位置信息，成功返回1， 失败返回-1；

    DrawLayer *getCurrentLayer(int id);//获取当前图层，参数id为创建图层id， 成功返回当前id为指定参数的图层，失败返回-1
    int setOpacity(float opacity);  //获取当前图层，设置透明度，参数从0-1.0，成功返回1，失败返回-1；
    float getOpacity();//获取当前图层，获取透明度，失败为-1，成功为0.0-1.0的值，
    int setVisiable(int id, bool visiable);//visiable为false为隐藏，true为显示， 设置是否可以显示，成功返回为1，失败为-1
    bool getVisiable(int id);//获取当前显示属性
    int setLock(int id, bool block); // block为false为未锁状态，block为true为锁定状态。
    bool getLock(int id);//获取当前图层的锁定状态。
    int setTranslate(float x, float y, float z);//当前图层平移，失败返回-1，成功返回1
    //vec3 getTranslate();//当前图层，获取当前图层的位置，返回图层所在的位置
    int setScale(float cx, float cy, float scale);//设置当前图层的缩放，失败返回-1，成功返回1,中心点，两手指中点
    //vec3 getScale(); // 获取当前图层的缩放比例，
    int setRotate(float cx, float cy, float angle);//设置图层当前旋转, 成功返回1， 失败返回-1，中心点，两手指中点
    //vec3 getRotate(); //获取当前旋转值
    ////////////////////////
    int setAllTranslate(float x, float y, float z);//整个场景平移，失败返回-1，成功返回1
    cocos2d::Vec3 getAllTranslate();//整个场景，获取当前图层的位置，返回图层所在的位置
    int setAllScale(float cx, float cy, float scale);//设置整个场景缩放，失败返回-1，成功返回1, 中心点，两手指中点
    cocos2d::Vec3 getAllScale(); // 获取当前图层的缩放比例，
    int setAllRotate(float cx, float cy, float angle);//设置图层当前旋转, 成功返回1， 失败返回-1, 中心点，两手指中点
    cocos2d::Vec3 getAllRotate(); //获取当前旋转值
    /////////////
    int setBlendMode(int mode);//设置当前图层的混合模式，成功返回1，失败返回-1
    int getBlendMode(); //设置当前图层的混合模式
    int setBackgroundColor(float r, float g, float b, float a);//设置图层的背景颜色，成功返回1，失败返回-1；
    Color4B getBackgroundColor(); //获取背景图层的颜色
    int setBackgroundTexture(int texid); //设置背景纹理，成功返回1，失败返回-1
    int setBackgroundTexture(char *pdata); //设置背景纹理，成功返回1，失败返回-1
    int setBackgroundTexture(std::string texid); //设置背景纹理，成功返回1，失败返回-1
    int getBackgroundTexture();//获取背景纹理，成功返回纹理id，失败返回-1；
    int getLayerThumbnailData(int layerid);// 获取图层的缩略图
    int getThumbnailData(); //获取整个截图

private:
    DrawLayer *mCurrentLayer;
    Caretaker mCaretake;

public: // for test
    Size visibleSize;
    Vec2 origin;
};

#endif // __HELLOWORLD_SCENE_H__

//#include <2d/painter.h>
#include "LayerManager.h"
#include "VisibleRect.h"
#include "coclog.h"

USING_NS_CC;

Scene* LayerManager::createScene()
{
    return LayerManager::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


void LayerManager::createMenu()
{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(LayerManager::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // Events
    MenuItemFont::setFontName("fonts/arial.ttf");
    MenuItemFont::setFontSize(10);
    // Bugs Item
    auto createMenu = MenuItemFont::create("Create", CC_CALLBACK_1(LayerManager::menuCreateCallback, this));
    createMenu->setPosition( origin.x + 20., origin.y + 20);

    auto deleteMenu = MenuItemFont::create("Delete", CC_CALLBACK_1(LayerManager::menuDeleteCallback, this));
    deleteMenu->setPosition(origin.x + 20, origin.y + 10);

    auto drawTestMenu = MenuItemFont::create("Test", CC_CALLBACK_1(LayerManager::menuDrawTestCallback, this));
    drawTestMenu->setPosition( origin.x + 20., origin.y + 100);

    auto rotaMenu = MenuItemFont::create("Rota", CC_CALLBACK_1(LayerManager::menuRotaCallback, this));
    rotaMenu->setPosition( origin.x + 20., origin.y + 80);


    auto translateMenu = MenuItemFont::create("translate", CC_CALLBACK_1(LayerManager::menuTranslateCallback, this));
    translateMenu->setPosition( origin.x + 20., origin.y + 60);

    auto copyMenu = MenuItemFont::create("copy", CC_CALLBACK_1(LayerManager::menuCopyCallback, this));
    copyMenu->setPosition( origin.x + 20., origin.y + 40);

    // create menu, it's an autorelease object
    auto menu = Menu::create(createMenu, deleteMenu, drawTestMenu, rotaMenu, translateMenu,
                             copyMenu, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


}

void LayerManager::menuCreateCallback(Ref *pSender)
{

    LOGE("Fun:%s", __FUNCTION__);
    createLayer();

}

void LayerManager::menuDeleteCallback(Ref *pSender)
{
    LOGE("Fun:%s", __FUNCTION__);
    deleteLayer(mCurrentLayer);
}

void LayerManager::menuDrawTestCallback(Ref *pSender)
{
    LOGE("Fun:%s", __FUNCTION__);
    if( mCurrentLayer == NULL)
        return;

    auto s = Director::getInstance()->getWinSize();

    mCurrentLayer->drawTest();
//    mCurrentLayer->drawPoint(Vec2(s.width/2 -100, s.height/2 - 100), 50, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
//
//    mCurrentLayer->drawPoint(Vec2(s.width/2+120, s.height/2+120), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
//
//    // draw 4 small points
//    Vec2 position[] = { Vec2(60,60), Vec2(70,70), Vec2(60,70), Vec2(70,60) };
//    mCurrentLayer->drawPoints( position, 4, 5, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
//
//    // draw a line
//    mCurrentLayer->drawLine(Vec2(0,0), Vec2(s.width, s.height), Color4F(1.0, 0.0, 0.0, 0.5));

    // draw a rectangle
//    mCurrentLayer->drawRect(Vec2(23,23), Vec2(7,7), Color4F(1,1,0,1));
//
//    mCurrentLayer->drawRect(Vec2(15,30), Vec2(30,15), Vec2(15,0), Vec2(0,15), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));


//    Label *label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
//    if (label == nullptr)
//    {
//        problemLoading("'fonts/Marker Felt.ttf'");
//    }
//    else
//    {
//        float fx = 100; //origin.x + visibleSize.width/2;
//        float fy = 10; //origin.y + visibleSize.height - label->getContentSize().height;
//        // position the label on the center of the screen
//        label->setPosition(fx, fy);
//
//        // add the label as a child to this layer
//        mCurrentLayer->addChild(label, 1);
//    }

}

void LayerManager::menuRotaCallback(Ref *pSender)
{
    LOGE("Fun:%s", __FUNCTION__);
    if( mCurrentLayer == NULL)
        return;

    setRotate(15.0, 0, 0);

}

void LayerManager::menuTranslateCallback(Ref *pSender)
{
    LOGE("Fun:%s", __FUNCTION__);
    if( mCurrentLayer == NULL)
        return;
    setTranslate(100, 100, 0);
}

void LayerManager::menuCopyCallback(Ref *pSender)
{
    LOGE("Fun:%s", __FUNCTION__);
    if( mCurrentLayer == NULL)
        return;
    copyLayer( mCurrentLayer );
    return;
}

// on "init" you need to initialize your instance
bool LayerManager::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

//    mh_init(800, 600, 800, 600);
//    mh_resize(800, 600);
    unsigned char pixles[16] =
            {
                    0xff, 0x00, 0xff, 0xff,
                    0xff, 0x00, 0xff, 0xff,
                    0xff, 0x00, 0xff, 0xff,
                    0xff, 0x00, 0xff, 0xff
            };
//    mh_setTexture(0, 2, 2, pixles);
//    mh_setTexture(1, 2, 2, pixles);
//    mh_setTexture(2, 2, 2, pixles);
//    mh_drawBlankCanvas(0.5, 0.5, 0.5);

    /////////////////////////////
//    return  true;
    createMenu();
//    return true;
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label



    // add "HelloWorld" splash screen"
    Sprite *sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

//    drawTest();

    return true;
}

void LayerManager::drawTest()
{
    ////////////////
    auto s = Director::getInstance()->getWinSize();

    auto draw = DrawLayer::create();
    addChild(draw, 10);
    mCurrentLayer = draw;

    draw->drawPoint(Vec2(s.width/2-120, s.height/2-120), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    draw->drawPoint(Vec2(s.width/2+120, s.height/2+120), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    // draw 4 small points
    Vec2 position[] = { Vec2(60,60), Vec2(70,70), Vec2(60,70), Vec2(70,60) };
    draw->drawPoints( position, 4, 5, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    // draw a line
    draw->drawLine(Vec2(0,0), Vec2(s.width, s.height), Color4F(1.0, 0.0, 0.0, 0.5));

    // draw a rectangle
    draw->drawRect(Vec2(23,23), Vec2(7,7), Color4F(1,1,0,1));

    draw->drawRect(Vec2(15,30), Vec2(30,15), Vec2(15,0), Vec2(0,15), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    // draw a circle
    draw->drawCircle(VisibleRect::center() + Vec2(140,0), 100, CC_DEGREES_TO_RADIANS(90), 50, true, 1.0f, 2.0f, Color4F(1.0, 0.0, 0.0, 0.5));

    draw->drawCircle(VisibleRect::center() - Vec2(140,0), 50, CC_DEGREES_TO_RADIANS(90), 30, false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    // Draw some beziers
    draw->drawQuadBezier(Vec2(s.width - 150, s.height - 150), Vec2(s.width - 70, s.height - 10), Vec2(s.width - 10, s.height - 10), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));

    draw->drawQuadBezier(Vec2(0, s.height), Vec2(s.width/2, s.height/2), Vec2(s.width, s.height), 50, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));

    draw->drawCubicBezier(VisibleRect::center(), Vec2(VisibleRect::center().x+30,VisibleRect::center().y+50), Vec2(VisibleRect::center().x+60,VisibleRect::center().y-50),VisibleRect::right(),100, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));

    draw->drawCubicBezier(Vec2(s.width - 250, 40), Vec2(s.width - 70, 100), Vec2(s.width - 30, 250), Vec2(s.width - 10, s.height - 50), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));

    auto array = PointArray::create(20);
    array->addControlPoint(Vec2(0,0));
    array->addControlPoint(Vec2(80,80));
    array->addControlPoint(Vec2(s.width-80,80));
    array->addControlPoint(Vec2(s.width-80,s.height-80));
    array->addControlPoint(Vec2(80,s.height-80));
    array->addControlPoint(Vec2(80,80));
    array->addControlPoint(Vec2(s.width/2, s.height/2));
    draw->drawCardinalSpline(array, 0.5, 50, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));

    auto array2 = PointArray::create(20);
    array2->addControlPoint(Vec2(s.width / 2, 30));
    array2->addControlPoint(Vec2(s.width  -80, 30));
    array2->addControlPoint(Vec2(s.width - 80, s.height - 80));
    array2->addControlPoint(Vec2(s.width / 2, s.height - 80));
    array2->addControlPoint(Vec2(s.width / 2, 30));
    draw->drawCatmullRom(array2, 50, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));

    // open random color poly
    Vec2 vertices[] = { Vec2(0,0), Vec2(50,50), Vec2(100,50), Vec2(100,100), Vec2(50,100) };
    draw->drawPoly( vertices, 5, false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    // closed random color poly
    Vec2 vertices2[] = { Vec2(30,130), Vec2(30,230), Vec2(50,200) };
    draw->drawPoly( vertices2, 3, true, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    // Draw 10 circles
    for( int i=0; i < 10; i++)
    {
        draw->drawDot(Vec2(s.width/2, s.height/2), 10*(10-i), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    }

    auto draw2 = DrawLayer::create();
    addChild(draw2, 10);
    // Draw polygons
    Vec2 points[] = { Vec2(s.height/4,0), Vec2(s.width,s.height/5), Vec2(s.width/3*2,s.height) };
    draw2->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(1,0,0,0.5), 4, Color4F(0,0,1,0.5));

    // star poly (triggers buggs)
    {
        const float o=80;
        const float w=20;
        const float h=50;
        Vec2 star[] = {
                Vec2(o+w,o-h), Vec2(o+w*2, o),                        // lower spike
                Vec2(o + w*2 + h, o+w ), Vec2(o + w*2, o+w*2),        // right spike
                //              {o +w, o+w*2+h}, {o,o+w*2},                 // top spike
                //              {o -h, o+w}, {o,o},                         // left spike
        };

        draw2->drawPolygon(star, sizeof(star)/sizeof(star[0]), Color4F(1,0,0,0.5), 1, Color4F(0,0,1,1));
    }

    // star poly (doesn't trigger bug... order is important un tesselation is supported.
    {
        const float o=180;
        const float w=20;
        const float h=50;
        Vec2 star[] = {
                Vec2(o,o), Vec2(o+w,o-h), Vec2(o+w*2, o),        // lower spike
                Vec2(o + w*2 + h, o+w ), Vec2(o + w*2, o+w*2),    // right spike
                Vec2(o +w, o+w*2+h), Vec2(o,o+w*2),               // top spike
                Vec2(o -h, o+w),                                     // left spike
        };

        draw2->drawPolygon(star, sizeof(star)/sizeof(star[0]), Color4F(1,0,0,0.5), 1, Color4F(0,0,1,1));
    }

    //draw a solid polygon
    Vec2 vertices3[] = {Vec2(60,160), Vec2(70,190), Vec2(100,190), Vec2(90,160)};
    draw2->drawSolidPoly( vertices3, 4, Color4F(1,1,0,1) );

    //draw a solid rectangle
    draw2->drawSolidRect(Vec2(10,10), Vec2(20,20), Color4F(1,1,0,1));

    //draw a solid circle
    draw2->drawSolidCircle( VisibleRect::center() + Vec2(140,0), 40, CC_DEGREES_TO_RADIANS(90), 50, 2.0f, 2.0f, Color4F(0.0, 1.0, 0.0, 1.0));

    // Draw segment
    draw2->drawSegment(Vec2(20,s.height), Vec2(20,s.height/2), 10, Color4F(0, 1, 0, 1));

    draw2->drawSegment(Vec2(10,s.height/2), Vec2(s.width/2, s.height/2), 40, Color4F(1, 0, 1, 0.5));

    // Draw triangle
    draw2->drawTriangle(Vec2(10, 10), Vec2(70, 30), Vec2(100, 140), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));

    for (int i = 0; i < 100; i++) {
        draw2->drawPoint(Vec2(i*7, 5), (float)i/5+1, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    }

    draw->setVisible(false);
    return;
}


void LayerManager::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}


DrawLayer *LayerManager::createLayer()
{
    DrawLayer *draw = DrawLayer::create();
    addChild(draw, 10);
    DrawBean *db = new DrawBean();
    db->setLayer(draw);
    db->setUserOp(LAYER_CREATE);
    mCaretake.SetState(db);
    mCurrentLayer = draw;
    return draw;
}

int LayerManager::deleteLayer(DrawLayer *id)
{
    removeChild(id);
    return -1;
}
int LayerManager::copyLayer(DrawLayer * id)
{
    DrawLayer *player = mCurrentLayer->clone();
    player->setPositionX(100);
//    player->setPositionY(100);
    addChild(player, 10);
    DrawBean *db = new DrawBean();
    db->setLayer(player);
    db->setUserOp(LAYER_CREATE);
    mCaretake.SetState(db);
//    mCurrentLayer->setVisible(false);
    mCurrentLayer = player;

    LOGE("Fun:%s, Line:%d", __FUNCTION__, __LINE__);

    return 1;
}

int LayerManager::clearLayer(int id)
{
    if(mCurrentLayer != NULL ) {
        mCurrentLayer->cleanup();
        return 1;
    }
    return -1;
}
int LayerManager::mergeLayer(DrawLayer *id1, DrawLayer *id2, int merge_mode)
{
    return -1;
}
int LayerManager::swapLayer(int layer1, int layer2)
{
    return -1;
}

DrawLayer *LayerManager::getCurrentLayer(int id)
{
    return mCurrentLayer;
}
int LayerManager::setOpacity(float opacity)
{
    if( mCurrentLayer != NULL )
    {
        mCurrentLayer->setOpacity( opacity);
        return 1;
    }
    return -1;
}
float LayerManager::getOpacity()
{
    if( mCurrentLayer != NULL ){
        return mCurrentLayer->getOpacity();
    }
    return 0.0f;
}
int LayerManager::setVisiable(int id, bool visiable)
{
    if( mCurrentLayer != NULL )
    {
        mCurrentLayer->setVisible( visiable);
        return 1;
    }
    return -1;
}
bool LayerManager::getVisiable(int id)
{
    if( mCurrentLayer != NULL )
    {
        return mCurrentLayer->isVisible();
    }
    return false;
}
int LayerManager::setLock(int id, bool block)
{
    if( mCurrentLayer != NULL )
    {
        mCurrentLayer->setLock(block);
        return 1;
    }
    return -1;
}
bool LayerManager::getLock(int id)
{
    if( mCurrentLayer != NULL )
        return mCurrentLayer->isLock();
    return false;
}
int LayerManager::setTranslate(float x, float y, float z)
{
    if( mCurrentLayer != NULL )
    {
        mCurrentLayer->setPosition( x, y );
        return 1;
    }
    return -1;
}
//vec3 getTranslate();//当前图层，获取当前图层的位置，返回图层所在的位置
int LayerManager::setScale(float fx, float fy, float scale)
{
    if(mCurrentLayer != NULL )
    {
        mCurrentLayer->setScale(fx, fy);
        return 1;
    }
    return -1;
}
//vec3 getScale(); // 获取当前图层的缩放比例，
int LayerManager::setRotate(float cx, float cy, float angle)
{
    if( mCurrentLayer != NULL ) {
        mCurrentLayer->setRotation(cx);
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

cocos2d::Vec3 LayerManager::getAllTranslate()
{
    return cocos2d::Vec3(0.0f, 0.0f, 0.0f);
}

int LayerManager::setAllScale(float cx, float cy, float scale)
{
    return -1;
}
cocos2d::Vec3 LayerManager::getAllScale()
{
    return cocos2d::Vec3(0.0f, 0.0f, 0.0f);
}

int LayerManager::setAllRotate(float cx, float cy, float angle)
{
    return -1;
}
cocos2d::Vec3 LayerManager::getAllRotate()
{
    return cocos2d::Vec3(0.0f, 0.0f, 0.0f);
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


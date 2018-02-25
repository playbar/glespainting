//
//  Caretaker.hpp
//  haowanlib
//
//

#ifndef Caretaker_hpp
#define Caretaker_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>
#include <platform/CCStdC.h>

using namespace std;

NS_CC_BEGIN
    class Node;
NS_CC_END

enum USER_OP{
    LAYER_CREATE = 800000,
    LAYER_DELETE = 880000
};


struct Point{
    int mx;
    int my;
    Point(int x, int y)
    {
        mx = x;
        my = y;
    }
};


class DrawBean{
public:

    DrawBean():mstart(0,0), mend(0, 0){
        mColor = 0xff;
        malpha = 255;
        msize = 5;
        mmode = 0;
        mtextureid = 0;
        mbyteData = NULL;
    }

    ~DrawBean()
    {
        if( mbyteData != NULL )
        {
            delete []mbyteData;
            mbyteData = NULL;
        }
        mpLayer = NULL;
    }

    void addPoint(Point &point){
        mpointList.emplace_back(point);
    }

    void setLayer(cocos2d::Node *player)
    {
        mpLayer = player;
    }

    cocos2d::Node *getLayer()
    {
        return mpLayer;
    }

    int getTextureid() {
        return mtextureid;
    }

    void setTextureid(int textureid) {
        mtextureid = textureid;
    }

    int getPointNum() {
        return mpointNum;
    }

    void setPointNum(int pointNum) {
        mpointNum = pointNum;
    }

    char *getByteData() {
        return mbyteData;
    }

    void setByteData(char *byteData) {
        mbyteData = byteData;
    }

    Point getStart() {
        return mstart;
    }

    void setStart(Point& start) {
        mstart = start;
    }

    Point getEnd() {
        return mend;
    }

    void setEnd(Point end) {
        mend = end;
        setPointNum(mpointList.size());
        packageNativeData();

    }

    int getColor() {
        return mColor;
    }

    void setColor(int color) {
        mColor = color;
    }

    int getAlpha() {
        return malpha;
    }

    void setAlpha(int alpha) {
        malpha = alpha;
    }

    int getSize() {
        return msize;
    }

    void setSize(int size) {
        msize = size;
    }

    int getMode() {
        return mmode;
    }

    void setMode(int mode) {
        mmode = mode;
    }

    std::vector<Point>& getPointList() {
        return mpointList;
    }

    void setPointList(std::vector<Point> pointList) {
        mpointList = pointList;
    }

    void setUserOp(USER_OP op)
    {
        mUserop = op;
    }

    /**封装底层数据
     * 数据结构：点数|颜色|宽度|纹理id|类型|点数据
     */
    void packageNativeData()
    {
        int currPos = 0;
        int totalLength = (getPointList().size() * 2 + 5) * 4;
        mbyteData = new char[totalLength];
        currPos = addToByte(currPos, mpointNum);
        currPos = addToByte(currPos, mColor);
        currPos = addToByte(currPos, msize);
        currPos = addToByte(currPos,mtextureid);
        currPos = addToByte(currPos,mmode);
        for(int i = 0;i < mpointList.size();i++){
            currPos = addToByte(currPos,mpointList[i].mx);
            currPos = addToByte(currPos,mpointList[i].my);
        }

    }

    int addToByte(int pos,int data){
        mbyteData[pos] = (data & 0xFF);
        mbyteData[pos+1] = ((data >> 8) & 0xFF);
        mbyteData[pos+2] = ((data >> 16) & 0xFF);
        mbyteData[pos+3] = ((data >> 24) & 0xFF);
        return pos + 4;
    }
    //    private int addToByteFloat(int pos,Float data){
    //        byteData[pos] = (byte) (data & 0xFF);
    //        byteData[pos+1] = (byte) ((data >> 8) & 0xFF);
    //        byteData[pos+2] = (byte) ((data >> 16) & 0xFF);
    //        byteData[pos+3] = (byte) ((data >> 24) & 0xFF);
    //        return pos + 4;
    //    }

    char * i2b(int i) {
        static char result[4] = {0};
        result[0] = (i >> 24) & 0xFF;
        result[1] =(i >> 16) & 0xFF;
        result[2] = (i >> 8) & 0xFF;
        result[3] =  i & 0xFF;
        return result;
    }
    static int b2i(char b[4]) {
        int value = 0;
        for (int i = 0; i < 4; i++) {
            int shift = (4 - 1 - i) * 8;
            value += (b[i] & 0x000000FF) << shift;
        }
        return value;
    }

    char *i2b_1(int i) {
        static char result[4] = {0};
        result[3] = (i >> 24) & 0xFF;
        result[2] =(i >> 16) & 0xFF;
        result[1] = (i >> 8) & 0xFF;
        result[0] =  i & 0xFF;
        return result;

    }
    static int b2i_1(char b[4]) {
        int value = 0;
        for (int i = 0; i < 4; i++) {
            int shift = (4 - 1 - i) * 8;
            value += (b[i] & 0x000000FF) << shift;
        }
        return value;
    }



private:
    USER_OP mUserop; // 用户操作动作
    cocos2d::Node *mpLayer; //当前活动图层
    int mColor;//画笔颜色
    int malpha;//画笔透明度
    int msize;//画笔尺寸
    int mmode;//画笔模式
    int mtextureid;//纹理id
    int mpointNum;//一笔绘制的点数量
    Point mstart;//起点数据
    Point mend;//终点数据
    std::vector<Point> mpointList;//比划过程数据
    //    ByteBuffer byteBuffer = ByteBuffer.allocate(10000000);
    char * mbyteData;//底层使用数据

};

class Caretaker
{
public:
    Caretaker()
    {
        mTotalStep = 0;
        mCurrentStep = 0;
    }
    DrawBean* GetState(int key)
    {
        return mData[key];
    }
    void SetState(DrawBean* pMemento)
    {
        if( mTotalStep > mCurrentStep )
        {
            for (int i = mTotalStep + 1; i < mCurrentStep; ++i) {
                DrawBean *pTemp = mData[i];
                delete pTemp;
            }
        }
        ++mCurrentStep;
        mData[mCurrentStep] = pMemento;
        mTotalStep = mCurrentStep;
    }

    DrawBean *previous(unsigned int step = 1)
    {
        if( mCurrentStep - step < 1 )
        {
            return mData[1];
        } else{
            mCurrentStep = mCurrentStep - step;
            return mData[mCurrentStep];
        }
    }

    DrawBean *next(unsigned int step = 1)
    {
        if( mCurrentStep + step > mTotalStep )
        {
            return mData[mTotalStep];
        } else{
            mCurrentStep = mCurrentStep + step;
            return mData[mCurrentStep];
        }
    }

    ~Caretaker()
    {
        auto it = mData.begin();
        for(; it != mData.end(); it++)
        {
            if( it->second != nullptr)
            {
                delete it->second;
            }
        }
        mData.clear();
    }
    unsigned int getStep()
    {
        return mTotalStep;
    }

private:
    unordered_map<int, DrawBean*> mData;
    unsigned int mTotalStep;
    unsigned int mCurrentStep;
};


#endif /* Caretaker_hpp */

package com.gles.painting.bean;

import java.io.Serializable;
import java.util.ArrayList;

/**
 * Created by Mypc on 2018/1/14 0014.
 */

public class DrawBean implements Serializable{
    int color = 0xff;//画笔颜色
    int alpha = 255;//画笔透明度
    int size = 5;//画笔尺寸
    int mode = 0;//画笔模式
    int textureid = 0;//纹理id
    int pointNum;//一笔绘制的点数量
    Point start;//起点数据
    Point end;//终点数据
    ArrayList<Point> pointList;//比划过程数据
//    ByteBuffer byteBuffer = ByteBuffer.allocate(10000000);
    public byte[] byteData;//底层使用数据

    /**
     * 添加点数据到列表
     * @param point
     */
    public void addPoint(Point point){

        if(pointList == null){
            pointList = new ArrayList<>();
        }
        pointList.add(point);

//        byteBuffer.asIntBuffer();
//        byteBuffer.array();


    }

    public int getTextureid() {
        return textureid;
    }

    public void setTextureid(int textureid) {
        this.textureid = textureid;
    }

    public int getPointNum() {
        return pointNum;
    }

    public void setPointNum(int pointNum) {
        this.pointNum = pointNum;
    }

    public byte[] getByteData() {
        return byteData;
    }

    public void setByteData(byte[] byteData) {
        this.byteData = byteData;
    }

    public Point getStart() {
        return start;
    }

    public void setStart(Point start) {
        this.start = start;
    }

    public Point getEnd() {
        return end;
    }

    public void setEnd(Point end) {
        this.end = end;
        if(pointList != null){
            setPointNum(pointList.size());
        }
        packageNativeData();

    }

    public int getColor() {
        return color;
    }

    public void setColor(int color) {
        this.color = color;
    }

    public int getAlpha() {
        return alpha;
    }

    public void setAlpha(int alpha) {
        this.alpha = alpha;
    }

    public int getSize() {
        return size;
    }

    public void setSize(int size) {
        this.size = size;
    }

    public int getMode() {
        return mode;
    }

    public void setMode(int mode) {
        this.mode = mode;
    }

    public ArrayList<Point> getPointList() {
        return pointList;
    }

    public void setPointList(ArrayList<Point> pointList) {
        this.pointList = pointList;
    }

    /**封装底层数据
     * 数据结构：点数|颜色|宽度|纹理id|类型|点数据
     */
    private void packageNativeData(){
        int currPos = 0;
        int totalLength = (getPointList().size() * 2 + 5) * 4;
        byteData = new byte[totalLength];
        currPos = addToByte(currPos,pointNum);
        currPos = addToByte(currPos,color);
        currPos = addToByte(currPos,size);
        currPos = addToByte(currPos,textureid);
        currPos = addToByte(currPos,mode);
        for(int i = 0;i < pointList.size();i++){
            currPos = addToByte(currPos,pointList.get(i).x);
            currPos = addToByte(currPos,pointList.get(i).y);
        }

    }

    private int addToByte(int pos,int data){
        byteData[pos] = (byte) (data & 0xFF);
        byteData[pos+1] = (byte) ((data >> 8) & 0xFF);
        byteData[pos+2] = (byte) ((data >> 16) & 0xFF);
        byteData[pos+3] = (byte) ((data >> 24) & 0xFF);
        return pos + 4;
    }
//    private int addToByteFloat(int pos,Float data){
//        byteData[pos] = (byte) (data & 0xFF);
//        byteData[pos+1] = (byte) ((data >> 8) & 0xFF);
//        byteData[pos+2] = (byte) ((data >> 16) & 0xFF);
//        byteData[pos+3] = (byte) ((data >> 24) & 0xFF);
//        return pos + 4;
//    }

    private byte[] i2b(int i) {
        return new byte[]{(byte) ((i >> 24) & 0xFF),
                (byte) ((i >> 16) & 0xFF), (byte) ((i >> 8) & 0xFF),
                (byte) (i & 0xFF)};
    }
    public static int b2i(byte[] b) {
        int value = 0;
        for (int i = 0; i < 4; i++) {
            int shift = (4 - 1 - i) * 8;
            value += (b[i] & 0x000000FF) << shift;
        }
        return value;
    }

    private byte[] i2b_1(int i) {
        return new byte[]{(byte) (i & 0xFF),
                (byte) ((i >> 8) & 0xFF), (byte) ((i >> 16) & 0xFF),
                (byte) ((i >> 24) & 0xFF)};
    }
    public static int b2i_1(byte[] b) {
        int value = 0;
        for (int i = 0; i < 4; i++) {
            int shift = (4 - 1 - i) * 8;
            value += (b[i] & 0x000000FF) << shift;
        }
        return value;
    }
}

package com.haowan.openglnew.bean;

import android.graphics.Bitmap;

import com.haowan.openglnew.R;


/**
 * Created by Mypc on 2018/2/22 0022.
 */

public class DrawLayer {
    private int layerId;
    private int isLocked = 0;//0-不加锁，1-加锁
    private int isVisible = 1;//0- 不可见，1-可见
    private int isMoving = 0;//0-不可移动，1-可移动
    private int alpha = 255;//透明度
    private int alphaRate = 100;//透明度百分比
    private int[] layerMode = {1, R.string.layer_mode_normal};
    private Bitmap subnail;

    public DrawLayer(int layerId){
        this.layerId = layerId;
    }
    public DrawLayer(){
    }

    public int getIsMoving() {
        return isMoving;
    }

    public void setIsMoving(int isMoving) {
        this.isMoving = isMoving;
    }
    public int getMovingOpposite(){
        return isMoving == 0 ? 1 : 0;
    }
    public int getLayerId() {
        return layerId;
    }

    public void setLayerId(int layerId) {
        this.layerId = layerId;
    }

    public int getIsLocked() {
        return isLocked;
    }
    public int getLockedOpposite(){
        return isLocked == 0 ? 1 : 0;
    }

    public void setIsLocked(int isLocked) {
        this.isLocked = isLocked;
    }

    public int getIsVisible() {
        return isVisible;
    }

    public int getVisibleOpposite(){
        return isVisible == 0 ? 1 : 0;
    }

    public void setIsVisible(int isVisible) {
        this.isVisible = isVisible;
    }

    public int getAlpha() {
        return alpha;
    }

    public void setAlpha(int alpha) {
        int alphaRate = (int)((alpha * 1.0f / 255) * 100);
        this.alpha = alpha;
        this.alphaRate = alphaRate;
    }

    public int getAlphaRate() {
        return alphaRate;
    }

    public void setAlphaRate(int alphaRate) {
        int alpha = (int)((alphaRate*1.0f / 100) * 255);
        this.alphaRate = alphaRate;
        this.alpha = alpha;
    }

    public int[] getLayerMode() {
        return layerMode;
    }

    public void setLayerMode(int[] layerMode) {
        this.layerMode = layerMode;
    }

    public Bitmap getSubnail() {
        return subnail;
    }

    public void setSubnail(Bitmap subnail) {
        this.subnail = subnail;
    }
}

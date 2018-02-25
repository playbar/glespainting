package com.haowan.openglnew;

import android.graphics.PointF;
import android.view.MotionEvent;

/**
 * Created by Administrator on 2017/1/12.
 */
public class ArithmeticUtil {

    /**
     * 计算两点之间的距离
     * @param event
     * @return
     */
    public static float spacing(MotionEvent event) {
        float x = event.getX(0) - event.getX(1);
        float y = event.getY(0) - event.getY(1);
        return (float) Math.sqrt(x * x + y * y);
    }

    /**
     * 计算两点之间的距离
     * @param x0
     * @param y0
     * @param x1
     * @param y1
     * @return
     */
    public static double distance(float x0,float y0,float x1,float y1){
        float x = x0 - x1;
        float y = y0 - y1;
        return Math.sqrt(x * x + y * y);
    }

    /**
     * 计算中心点的位置
     * @param point
     * @param event
     */
    public static void midPoint(PointF point, MotionEvent event) {
        float x = event.getX(0) + event.getX(1);
        float y = event.getY(0) + event.getY(1);
        point.set(x / 2, y / 2);
    }

    /**
     * 计算中心点的位置
     * @param point
     * @param point2
     */
    public static PointF midPoint(PointF point, PointF point2) {
        PointF midp = new PointF();
        midp.x = (float) (0.5f*(point.x + point2.x));
        midp.y = (float) (0.5f*(point.y + point2.y));
        return midp;
    }

    /**
     * 求对称点
     * @param centerP 对称中心点
     * @param orignalP
     * @return
     */
    public static PointF symmetryPoint(PointF centerP, PointF orignalP){
        float x = orignalP.x - centerP.x;
        float y = orignalP.y - centerP.y;
        return new PointF(centerP.x - x,centerP.y - y);
    }

}

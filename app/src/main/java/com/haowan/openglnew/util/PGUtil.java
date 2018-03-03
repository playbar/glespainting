package com.haowan.openglnew.util;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.widget.TextView;

/**
 * Created by xcf on 2018/2/27.
 */

public class PGUtil {

    // 获取屏幕宽度
    public static int getScreenWidth(Context context) {
        return context.getResources().getDisplayMetrics().widthPixels;
    }

    // 获取屏幕高度
    public static int getScreenHeight(Context context) {
        return context.getResources().getDisplayMetrics().heightPixels;
    }

    /**
     * string判空操作
     *
     * @param str
     * @return true-空,false-非空
     */
    public static boolean isStringNull(String str) {
        if (str == null || str.trim().length() == 0) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * dip转px
     *
     * @param context
     * @param dipValue
     * @return
     */
    public static int dip2px(Context context, float dipValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dipValue * scale + 0.5f);
    }

    /**
     * px转dip
     *
     * @param context
     * @param pxValue
     * @return
     */
    public static int px2dip(Context context, float pxValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (pxValue / scale + 0.5f);
    }

    /**
     * 设置textview的图片大小
     *
     * @param c
     * @param tv
     * @param drawableid
     * @param size
     * @param pos        (-1-不加图片,0-left,1-up,2-right,3-down)
     */
    public static void setTextViewDrawable(Context c, TextView tv, int drawableid, int size, int pos) {
        if (pos == -1) {
            tv.setCompoundDrawables(null, null, null, null);
        } else {
            Drawable drawable = c.getResources().getDrawable(drawableid);
            int size1 = PGUtil.dip2px(c, size);
            drawable.setBounds(0, 0, size1, size1);
            switch (pos) {
                case 0:
                    tv.setCompoundDrawables(drawable, null, null, null);
                    break;
                case 1:
                    tv.setCompoundDrawables(null, drawable, null, null);
                    break;
                case 2:
                    tv.setCompoundDrawables(null, null, drawable, null);
                    break;
                case 3:
                    tv.setCompoundDrawables(null, null, null, drawable);
                    break;
            }
        }
    }

    /**
     * 分割数据
     *
     * @param msg String
     * @return
     */
    public static int[] getIntArrayByString(String msg) {
        int[] result;
        String[] splitStr;
        try {
            splitStr = msg.split("\\|");
            int slength = splitStr.length;
            result = new int[slength];
            for (int i = 0; i < slength; i++) {
                //			    if (null != splitStr[i] && splitStr[i].matches("^\\d{1,}$"))
                //				if(splitStr[i] != "")
                //			    {
                try {
                    result[i] = Integer.valueOf(splitStr[i]);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                }
                //			    }
            }
            return result;
        } catch (OutOfMemoryError e) {
            result = null;
            Log.i("PGUtil", "--------OutOfMemoryError--------");
            e.printStackTrace();
        } finally {
            msg = null;
            splitStr = null;
            System.gc();
        }
        return null;
    }

    /**
     * 将argb颜色字串转换成int型颜色值
     *
     * @param argb
     * @return
     * @throws IllegalArgumentException
     */
    public static int convertToColorInt(String argb) throws IllegalArgumentException {
        if (!argb.startsWith("#")) {
            argb = "#" + argb;
        }
        return Color.parseColor(argb);
    }

    /**
     * 将int型颜色值转换成argb颜色字串
     *
     * @param color
     * @return
     */
    public static String convertToARGB(int color) {
        String alpha = Integer.toHexString(Color.alpha(color));
        String red = Integer.toHexString(Color.red(color));
        String green = Integer.toHexString(Color.green(color));
        String blue = Integer.toHexString(Color.blue(color));

        if (alpha.length() == 1) {
            alpha = "0" + alpha;
        }

        if (red.length() == 1) {
            red = "0" + red;
        }

        if (green.length() == 1) {
            green = "0" + green;
        }

        if (blue.length() == 1) {
            blue = "0" + blue;
        }

        return "#" + alpha + red + green + blue;
    }

    /**
     * 将int型颜色值转换成rgb字串值
     *
     * @param color
     * @return
     */
    public static String convertToRGB(int color) {
        String red = Integer.toHexString(Color.red(color));
        String green = Integer.toHexString(Color.green(color));
        String blue = Integer.toHexString(Color.blue(color));

        if (red.length() == 1) {
            red = "0" + red;
        }

        if (green.length() == 1) {
            green = "0" + green;
        }

        if (blue.length() == 1) {
            blue = "0" + blue;
        }

        return "#" + red + green + blue;
    }

}

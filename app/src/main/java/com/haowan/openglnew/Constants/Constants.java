package com.haowan.openglnew.Constants;


import com.haowan.openglnew.R;

/**
 * Created by Mypc on 2018/1/14 0014.
 */

public class Constants {
    public static final class PaintMode
    {
        //画笔模式(0-正常模式,1-画模糊图片模式,2-画直线模式,3-柳叶笔模式,4-铅笔(笔画首末变细效果),5-钢笔(笔画首末粗),6-平笔,7-马克笔,8-蜡笔(粉笔,喷枪),9-油笔(毛刷) )
        public static final int PAINT_MODE_NORMAL = 0;
        public static final int PAINT_MODE_BLUR = 1;
        public static final int PAINT_MODE_LINE = 2;
        public static final int PAINT_MODE_LEAF = 3;
        public static final int PAINT_MODE_PENCIL = 4;
        public static final int PAINT_MODE_PEN = 5;
        public static final int PAINT_MODE_PING = 6;
        public static final int PAINT_MODE_MARKER = 7;
        public static final int PAINT_MODE_CRAYON = 8;
        public static final int PAINT_MODE_OIL = 9;

        public static final int PAINT_MODE_FILL = 100;//油漆桶
        public static final int PAINT_MODE_ERASE = 101;//橡皮擦

        public static final int DUICHENG = 200;//对称

    }

    /** 纹理id与相应纹理 */
    public static int[][] textures = {{0,1,2,3,4,5,6,7,8,9},
            {R.drawable._103,R.drawable._103,R.drawable._103,
                    R.drawable._103,R.drawable._103,R.drawable._103,
                    R.drawable._103,R.drawable._103,R.drawable._103,
                    R.drawable._103}};

























}

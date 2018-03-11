package com.gles.painting;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.Toast;

import com.gles.painting.bean.DrawBean;
import com.gles.painting.bean.Point;
import com.gles.painting.util.ArithmeticUtil;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;

/**
 * Created by xcf on 2017/12/3.
 */

public class GLESSurfaceView extends GLSurfaceView {

    private static final String TAG = "GLESSurfaceView";
    MyGestureListener myGestureListener;
    Handler mHandler;
    MyRenderer mRenderer;
    public ArrayList<DrawBean> drawBeans = new ArrayList<>();

    ArrayList<DrawBean> redoBeans = new ArrayList<>();

    public MyGestureListener getMyGestureListener() {
        return myGestureListener;
    }

    public void setMyGestureListener(MyGestureListener myGestureListener) {
        this.myGestureListener = myGestureListener;
    }

    public GLESSurfaceView(Context context) {
        super(context);
        common(context);
    }

    public GLESSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        common(context);
    }

    private void common(Context context){
        setEGLContextClientVersion(2);
        setEGLConfigChooser(new MyConfigChooser());
        mRenderer = new MyRenderer();
        mRenderer.setContext(context);
        setRenderer(mRenderer);
        setRenderMode(RENDERMODE_WHEN_DIRTY);

    }

    float size = 5;
    int color;
    int mode = 0;
    public void setSize(float size){
        this.size = size;
        if(mRenderer != null){
            mRenderer.setSize(size);
        }
    }
    public void setScreenWH(int[] wh){
        if(mRenderer != null){
            mRenderer.setScreenWH(wh);
        }
    }
    public void setColor(int color){
        this.color = color;
        if(mRenderer != null){
            mRenderer.setColor(color);
        }
    }
    public void setMode(int mode){
        this.mode = mode;
        if(mRenderer != null){
            mRenderer.setOpMode(mode);
        }
    }

    public String getSdPath()
    {
        String cachePath = null;
        if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())){
            cachePath = Environment.getExternalStorageDirectory().getAbsolutePath();
        }
        return cachePath;
    }
    public void getImageData(){
        if(mRenderer != null){
            this.queueEvent(new Runnable() {
                @Override
                public void run() {
                    byte[] bytes = mRenderer.getCanvasContent();
                    Bitmap stitchBmp = Bitmap.createBitmap(MainActivity.screenWH[0], MainActivity.screenWH[1], Bitmap.Config.ARGB_8888);
                    stitchBmp.copyPixelsFromBuffer(ByteBuffer.wrap(bytes));
                    saveBitmap(stitchBmp,getSdPath()+"/tempopengl/","saveImg.png", Bitmap.CompressFormat.PNG,100);
                    Log.i(TAG,"-------getImageData,save success-------");
                }
            });
        }
    }

    public boolean saveBitmap(Bitmap bitmap, String path, String fileName, Bitmap.CompressFormat format, int quality) {
        boolean flag = false;
        File desFile = createFile(path, fileName);
        BufferedOutputStream bos = null;
        try {
            bos = new BufferedOutputStream(new FileOutputStream(desFile));
            flag = bitmap.compress(format, quality, bos);
            bos.flush();
            bitmap.recycle();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            flag = false;
        } catch (IOException e) {
            flag = false;
            e.printStackTrace();
        } finally {
            if (bos != null) {
                try {
                    bos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        return flag;
    }

    /**
     * 创建文件
     * @param allFolder 除文件名外的路劲
     * @param fileName
     * @return
     */
    public File createFile(String allFolder,String fileName){
        File mFile = new File(allFolder);
        if(!mFile.exists()){
            mFile.mkdirs();
        }
        File tempFile = new File(allFolder,fileName);
        if(tempFile.exists()){
            tempFile.delete();
        }
        try {
            tempFile.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return tempFile;
    }


    public void onPause(){
        super.onPause();
    }
    public void onResume(){
        super.onResume();
    }

    @Override
    public boolean onTouchEvent(final MotionEvent pMotionEvent)
    {
        // these data are used in ACTION_MOVE and ACTION_CANCEL
        final int pointerNumber = pMotionEvent.getPointerCount();
        if(pointerNumber == 1) {
            if(!isRateMode) {
                drawMode(pMotionEvent, 1);
            }else{
                refreshMode(pMotionEvent);
            }
        }else{
            scaleMode(pMotionEvent);
        }

        return true;
    }

    private void refreshMode(MotionEvent pMotionEvent){
        switch(pMotionEvent.getAction()){
            case MotionEvent.ACTION_UP:
                isRateMode = false;
                break;
        }
    }

    private float getMid(float f0,float f1){
        return (f0 + f1)/2;
    }

    /**
     * 上一次的触摸点
     */
    public float x0, y0, x1, y1;
    /**
     * 是否缩放模式
     */
    public boolean isRateMode = false;

    private void saveStart(){
        DrawBean bean = new DrawBean();

    }

    /**
     * 两点之间的距离
     */
    float oldDist = 1f;
    private void scaleMode(MotionEvent pMotionEvent) {
        switch(pMotionEvent.getAction() & MotionEvent.ACTION_MASK){
            case MotionEvent.ACTION_POINTER_DOWN:
                isRateMode = true;
                oldDist = ArithmeticUtil.spacing(pMotionEvent);
                final float midx = getMid(pMotionEvent.getX(0),pMotionEvent.getX(1));
                final float midy = getMid(pMotionEvent.getY(0),pMotionEvent.getY(1));
                mRenderer.setOpMode(50);
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        mRenderer.handleActionDown(0,midx,midy);
                    }
                });
                requestRender();
                break;
            case MotionEvent.ACTION_POINTER_UP:
                isRateMode = true;
                mRenderer.setOpMode(0);
                break;
            case MotionEvent.ACTION_MOVE:
                //平移
                final float mx = getMid(pMotionEvent.getX(0),pMotionEvent.getX(1));
                final float my = getMid(pMotionEvent.getY(0),pMotionEvent.getY(1));

                //缩放
                float tempX0 = pMotionEvent.getX(0);
                float tempY0 = pMotionEvent.getY(0);
                float tempX1 = pMotionEvent.getX(1);
                float tempY1 = pMotionEvent.getY(1);
                if (x0 + y0 + x1 + y1 == 0) {
                    x0 = tempX0;
                    x1 = tempX1;
                    y0 = tempY0;
                    y1 = tempY1;
                    return;
                }
                double tempRate1 = ArithmeticUtil.distance(tempX0, tempY0, tempX1, tempY1) - ArithmeticUtil.distance(x0, y0, x1, y1);
                float scale;
                if (Math.abs(tempRate1) > 0.01) {
                    float tempDist = ArithmeticUtil.spacing(pMotionEvent);
                    if (tempDist > oldDist) {
                        scale = tempDist / oldDist;
                    } else {
                        scale = tempDist / oldDist;
                    }
                    oldDist = tempDist;
                } else {
                    scale = 1.0f;
                }
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        mRenderer.handleActionMove1(null,mx,my);
                    }
                });
                mRenderer.setScaleRate(scale);
                requestRender();

                tempScale *= scale;
                Log.i(TAG,"------------tempScale:"+tempScale);


                break;
        }

    }
    private float tempScale = 1.0f;

    DrawBean mDrawBean;
    private void drawMode(MotionEvent pMotionEvent,int pointerNumber){
        final int[] ids = new int[pointerNumber];
        final float[] xs = new float[pointerNumber];
        final float[] ys = new float[pointerNumber];

        for (int i = 0; i < pointerNumber; i++) {
            ids[i] = pMotionEvent.getPointerId(i);
            xs[i] = pMotionEvent.getX(i);
            ys[i] = pMotionEvent.getY(i);
        }

        switch (pMotionEvent.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                final int idDown = pMotionEvent.getPointerId(0);
                final int xDown = (int) xs[0];
                final int yDown = (int) ys[0];
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        if(mode == 100){
                            int color = mRenderer.pick((int)xs[0],(int)ys[0]);
                            int cb = color >> 16 & 0xff;
                            int cg = color >> 8 & 0xff;
                            int cr = color & 0xff;
                            final int tempColor = Color.rgb(cr,cg,cb);
                            mHandler.post(new Runnable() {
                                @Override
                                public void run() {
                                    if(myGestureListener != null) {
                                        myGestureListener.onGetColorBegin((int) xs[0], (int) ys[0], tempColor);
                                    }
                                }
                            });

                            return;
                        }

                        mRenderer.setOpMode(mode);

                        float[] f = RenderLib.getCanvasCoord(xDown, yDown);
                        mRenderer.handleActionDown(idDown, f[0], f[1]);

                        mDrawBean = new DrawBean();
                        Point mPoint = new Point((int)f[0],(int)f[1]);
                        mDrawBean.setSize((int)size);
                        mDrawBean.setColor(color);
                        mDrawBean.setStart(mPoint);
                        mDrawBean.setMode(mode);
                        mDrawBean.addPoint(mPoint);
                        if(mode != 3) {
                            mDrawBean.setTextureid(mode);
                        }else{
                            mDrawBean.setTextureid(0);
                        }
                    }
                });
                requestRender();


                break;

            case MotionEvent.ACTION_MOVE:

                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        if(mode == 100){
                            int color = mRenderer.pick((int)xs[0],(int)ys[0]);
                            int cb = color >> 16 & 0xff;
                            int cg = color >> 8 & 0xff;
                            int cr = color & 0xff;
                            final int tempColor = Color.rgb(cr,cg,cb);
                            mHandler.post(new Runnable() {
                                @Override
                                public void run() {
                                    if(myGestureListener != null){
                                        myGestureListener.onGetColor((int)xs[0],(int)ys[0],tempColor);
                                    }
                                }
                            });
                            return;
                        }

                        float[] f = RenderLib.getCanvasCoord(xs[0],ys[0]);
                        xs[0] = f[0];
                        ys[0] = f[1];
                        mRenderer.handleActionMove(ids, xs, ys);
                        if(mode != 2) {
                            mDrawBean.addPoint(new Point((int) xs[0], (int) ys[0]));
                        }
                    }
                });
                if(mode != 100) {
                    requestRender();
                }



                break;

            case MotionEvent.ACTION_UP:
                // there are only one finger on the screen
                final int idUp = pMotionEvent.getPointerId(0);
                final float xUp = xs[0];
                final float yUp = ys[0];
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        if(mode == 100){
                            int color = mRenderer.pick((int)xs[0],(int)ys[0]);
                            int cb = color >> 16 & 0xff;
                            int cg = color >> 8 & 0xff;
                            int cr = color & 0xff;
                            final int tempColor = Color.rgb(cr,cg,cb);
                            mHandler.post(new Runnable() {
                                @Override
                                public void run() {
                                    if(myGestureListener != null){
                                        myGestureListener.onGetColorEnd((int)xs[0],(int)ys[0],tempColor);
                                    }
                                }
                            });
                            return;
                        }
                        float[] f = RenderLib.getCanvasCoord(xUp, yUp);
                        mRenderer.handleActionUp(idUp, f[0], f[1]);

                        Point mEndPoint = new Point((int)f[0],(int)f[1]);
                        mDrawBean.addPoint(mEndPoint);
                        mDrawBean.setEnd(mEndPoint);
                        drawBeans.add(mDrawBean);
//                        Log.i("GLESSurface","------>mDrawBean:"+mDrawBean.getPointNum());
                    }
                });
                requestRender();


                if(drawBeans.size() % 20 == 0){
                    Log.i("GLESSURFACE","------size:"+drawBeans.size());
//                    JNILib.snap(drawBeans.size() / 20);

                }

                break;
        }
    }

    byte[] mbyte;
    public void undo(){
//        int cnt = drawBeans.size() % 20;
//        if(cnt == 0){
//            cnt = 20;
//        }
        if(drawBeans.size() > 0) {
            mbyte = new byte[]{};
            long time = System.currentTimeMillis();
            for (int i = 0; i < drawBeans.size()-1; i++) {
                byte[] b = drawBeans.get(i).byteData;
                Log.i("GlesSurface","-------b.length:"+b.length);
                byte[] tempByte = new byte[mbyte.length + b.length];
                System.arraycopy(mbyte, 0, tempByte, 0, mbyte.length);
                System.arraycopy(b, 0, tempByte, tempByte.length - b.length, b.length);
                mbyte = tempByte;
            }


            if (drawBeans.size() > 0) {
                redoBeans.add(drawBeans.remove(drawBeans.size() - 1));
            }

            mRenderer.setOpMode(10);
            mRenderer.beanSize = drawBeans.size();
            mRenderer.mbyte = mbyte;
            mRenderer.byteSize = mbyte.length;

//            this.queueEvent(new Runnable() {
//                @Override
//                public void run() {
////                    long time = System.currentTimeMillis();
////                    JNILib.undo(-1, drawBeans.size() - 1, mbyte, mbyte.length);
//                    if (drawBeans.size() > 0) {
//                        redoBeans.add(drawBeans.remove(drawBeans.size() - 1));
//                    }
//
//                    mRenderer.setOpMode(10);
//                    mRenderer.beanSize = drawBeans.size();
//                    mRenderer.mbyte = mbyte;
//                    mRenderer.byteSize = mbyte.length;
//
//
//                }
//            });
            requestRender();
        }else {
            Toast.makeText(getContext(),"无回退数据",Toast.LENGTH_SHORT).show();
        }

    }

    public void exit(){
        this.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRenderer.exit();
            }
        });
    }

    public void redo(){
        if(redoBeans.size() > 0) {
//            this.queueEvent(new Runnable() {
//                @Override
//                public void run() {
//                    byte[] b = redoBeans.get(redoBeans.size() - 1).byteData;
//                    JNILib.redo(b, b.length);
//                    drawBeans.add(redoBeans.remove(redoBeans.size() - 1));
//                }
//            });
            mRenderer.setOpMode(11);
            byte[] b = redoBeans.get(redoBeans.size() - 1).byteData;
            mRenderer.mbyte = b;
            mRenderer.byteSize = b.length;
            drawBeans.add(redoBeans.remove(redoBeans.size() - 1));
            requestRender();
        }else {
            Toast.makeText(getContext(),"无恢复数据",Toast.LENGTH_SHORT).show();
        }
    }


    public void setData(){
        if(drawBeans.size() > 0) {
            mbyte = new byte[]{};
            long time = System.currentTimeMillis();
            for (int i = 0; i < drawBeans.size(); i++) {
                byte[] b = drawBeans.get(i).byteData;
                byte[] tempByte = new byte[mbyte.length + b.length];
                System.arraycopy(mbyte, 0, tempByte, 0, mbyte.length);
                System.arraycopy(b, 0, tempByte, tempByte.length - b.length, b.length);
                mbyte = tempByte;
            }

            this.queueEvent(new Runnable() {
                @Override
                public void run() {
                    mRenderer.setOpMode(20);
                }
            });

            requestRender();

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            mRenderer.setOpMode(12);
            mRenderer.beanSize = drawBeans.size();
            mRenderer.mbyte = mbyte;
            mRenderer.byteSize = mbyte.length;

            requestRender();

        }else {
            Toast.makeText(getContext(),"无数据",Toast.LENGTH_SHORT).show();
        }

    }


    int i = 0;
    public void play(){
        long time = System.currentTimeMillis();
        for(i = 0;i < drawBeans.size();i++){
//            Log.i(TAG,"------i:"+i);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            this.queueEvent(new Runnable() {
                @Override
                public void run() {
                    mRenderer.play(1);
                }
            });
            requestRender();

        }
        requestRender();

        Log.i(TAG,"----------play use time:"+(System.currentTimeMillis() - time));

    }



    public interface MyGestureListener {

        void onGetColor(int x, int y, int color);

        void onGetColorBegin(int x, int y, int color);

        void onGetColorEnd(int x, int y, int color);

        void onGetColorCancel();

        void strokeNum(int size);


    }


}

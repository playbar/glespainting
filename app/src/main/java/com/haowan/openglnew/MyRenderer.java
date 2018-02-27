package com.haowan.openglnew;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView;
import android.util.Log;

import com.haowan.opengl_new.R;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyRenderer implements GLSurfaceView.Renderer {
    private static final String TAG = "MyRenderer";
    private int _act;
    private float _x, _y;
    private float _k;

    private float _x0, _y0, _x1, _y1; //last point and current point

    private boolean _first;

    private int _vx, _vy, _vw, _vh;

    private int _mode;  //0:draw, 1:pan, 2:zoom-in 3:zoom-out,4:change-canvas

    private float size = 5;
    private int[] screenWH = {1080, 1800};
    private int color = 0xff000000;

    Context _context;

    public float getSize() {
        return size;
    }

    public void setSize(float size) {
        this.size = size;
    }

    public int getColor() {
        return color;
    }

    public void setColor(int color) {
        Log.i("MyRender","-------------setColor");
        this.color = color;
    }

    public int[] getScreenWH() {
        return screenWH;
    }

    public void setScreenWH(int[] screenWH) {
        this.screenWH = screenWH;
    }

    public void setOpMode(int mode) {
//        Log.i(TAG,"-----MyRender-----setOpMode"+mode);
        _mode = mode;
    }

    public void setContext(Context context) { _context = context; }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        RenderLib.init();

//      _act = -1; _first=true; _mode=0; _k=1.2f;
//      _x0=_y0=_x1=_y1=0;
//      _vx = 0; _vy = 0; _vw = 1080; _vh = 1800;
//        JNILib.init(MainActivity.screenWH[0],MainActivity.screenWH[1], screenWH[0], screenWH[1]);  //canvas width/height, viewport's origin and width/height
//      {
//        final BitmapFactory.Options options = new BitmapFactory.Options();
//        options.inScaled = false;   // No pre-scaling
//          {
//              //初始化普通画笔纹理
//              final Bitmap bm = BitmapFactory.decodeResource(_context.getResources(), R.drawable.brush_2, options);
//              int w = bm.getWidth();
//              int h = bm.getHeight();
//              int[] pixels = new int[w * h];
//              bm.getPixels(pixels, 0, w, 0, 0, w, h);
//              JNILib.setTexture(0, w, h, pixels);  //normal pen
//          }
//        {
//            //初始化模糊画笔纹理
//          final Bitmap bm = BitmapFactory.decodeResource(_context.getResources(), R.drawable._104, options);
//          int w = bm.getWidth();
//          int h = bm.getHeight();
//          int[] pixels = new int[w * h];
//          bm.getPixels(pixels, 0, w, 0, 0, w, h);
//            JNILib.setTexture(1, w, h, pixels);  //normal pen
//        }
//          {
//              //初始化直线画笔纹理
//              final Bitmap bm = BitmapFactory.decodeResource(_context.getResources(), R.drawable.h, options);
//              int w = bm.getWidth();
//              int h = bm.getHeight();
//              int[] pixels = new int[w * h];
//              bm.getPixels(pixels, 0, w, 0, 0, w, h);
//              JNILib.setTexture(2, w, h, pixels);  //normal pen
//          }
//      }
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
//      _vw = width; _vh = height;
//        JNILib.resize(_vx, _vy, _vw, _vh);
        RenderLib.resize(width, height);
//        JNILib.resize(width, height);
    }

    public void onDrawFrame(GL10 gl) {
        RenderLib.step();
//        Log.i(TAG,"------onDrawFrame:currThread:"+Thread.currentThread());
//        Log.i(TAG, "_x=" + _x + ", _y=" + _y);
////        Log.i(TAG,"---------mode:"+_mode);
//        if(_first){
//            JNILib.drawBlankCanvas(0.9f,0.9f,0.9f);
//            _first = false;
//        }
//        else {
//            if(_mode == 4){
//                JNILib.changeCanvas(screenWH[0],screenWH[1],0xffffffff);
//                _mode = 0;
//            }else if(_mode==0){
////              float[] results = JNILib.getCanvasCoord(_x,_y);
////              _x = results[0]; _y = results[1];
//                if(_act==-1) return;
//                JNILib.drawNormalLine(_act, _x, _y, size, color, _mode);
//            }else if(_mode==1){
//                if(_act==-1) return;
//                JNILib.drawBlurLine(_act, _x, _y, size, color, _mode);
//            }else if(_mode == 2){
//                if(_act==-1) return;
//                JNILib.drawSegLine(_act, _x, _y, size, color, _mode);
//            }else if(_mode == 3){
//                if(_act==-1) return;
//                JNILib.drawLeaf(_act, _x, _y, size, color, _mode);
//
//            }else if(_mode==50){
//                JNILib.pan(_x0,_y0,_x1,_y1);
//                JNILib.zoom(_k,_x0,_y0);
//
//            }else if(_mode == 10){
//                JNILib.undo(-1, beanSize, mbyte, mbyte.length);
//            }else if(_mode == 11){
//                JNILib.redo(mbyte, mbyte.length);
//            }else if(_mode == 12){
//
////              JNILib.drawBlankCanvas(0.8f,0.8f,0.8f);
//                JNILib.playData(beanSize, mbyte, mbyte.length);
//                long time = System.currentTimeMillis();
//                JNILib.playDraw(beanSize);
//                Log.i(TAG,"---------------play use time:"+(System.currentTimeMillis() - time));
//
//            }else if(_mode == 20){
//                Log.i(TAG,"----------mode=20,清空------");
//                JNILib.drawBlankCanvas(0.8f,0.8f,0.8f);
//            }
//        }
    }

    public int beanSize = 0;
    public byte[] mbyte = new byte[]{};
    public int byteSize = 0;



    public void setScaleRate(float rate){
        _k = rate;
    }

    public void handleActionDown(int id, float x, float y) {
      _x = x; _y = y; _act = 0;
      _x0=_x1= x; _y0=_y1=y;
    }

    public void handleActionMove(int[] ids, float[] xs, float[] ys) {
      _x = xs[0]; _y = ys[0]; _act = 1;
      _x0=_x1;    _y0=_y1;
      _x1= xs[0]; _y1=ys[0];
//        Log.i("XCF","--handleActionMove---x0:"+_x0+",y0:"+_y0+",x1:"+_x1+",y1:"+_y1);
    }

    public void handleActionMove1(int[] ids, float xs, float ys) {
        _x = xs; _y = ys; _act = 1;
        _x0=_x1;    _y0=_y1;
        _x1= xs; _y1=ys;
        Log.i("XCF","--handleActionMove1---x0:"+_x0+",y0:"+_y0+",x1:"+_x1+",y1:"+_y1);
    }

    public void handleActionUp(int id, float x, float y) {
      _x = x; _y = y; _act = 2;
    }

    public void exit(){
        JNILib.exit();
    }


    public void play(int i){
//        Log.i("GLes","----render,play111--i:"+i);
//        long time = System.currentTimeMillis();
        JNILib.playDraw(i);
//        Log.i("xxxxxxx","--------play use time:"+(System.currentTimeMillis() - time));
//        Log.i("GLes","----render,play222--i:"+i);
    }

    public int pick(int x,int y){
        int color = JNILib.pick(x,y);
        Log.i(TAG,"------color:"+color+",color16:"+Integer.toHexString(color));
        return color;
    }

    public byte[] getCanvasContent(){
        return JNILib.getCanvasContent();
    }

  }

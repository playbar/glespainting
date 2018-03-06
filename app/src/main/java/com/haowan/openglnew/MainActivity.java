package com.haowan.openglnew;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.Toast;

import com.haowan.openglnew.bean.DrawBean;
import com.haowan.openglnew.view.MyToast;

import org.cocos2dx.lib.Cocos2dxHelper;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

public class MainActivity extends Activity implements View.OnClickListener,
        Cocos2dxHelper.Cocos2dxHelperListener
{

    private static final String TAG = "MainActivity";

    public static int[] colors = {0xff000099, Color.WHITE, 0xffef7474, 0xffeadf1c,
            0xff8bbd5a, 0xff3e7cbf, 0xffd781f2, 0xfff7c09b, 0xfffedcba};

    public static int[][] ScreenRate = {{800, 1200},{720,1280},{2400,2400}};
    public static int[] screenWH = {800,1200};

    RelativeLayout surfaceRoot;
    GLESSurfaceView surfaceView;
    Button btn2,btn3,btn4,btn5,btn6,btn7,btn8,btn9,btn10,btn11,btn12,btn13;
    SeekBar size,alpha;
    MyToast mToast;
    private int mAlpha = 255;

    @Override
    protected void onDestroy() {
        super.onDestroy();
        surfaceView.exit();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Cocos2dxHelper.init(this);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);
        surfaceRoot = (RelativeLayout)findViewById(R.id.surface_root);
        mToast = new MyToast(this);

        path = getSdPath()+"/opengltestfile.txt";
        File file = new File(path);
        if(!file.exists()){
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        initSurfaceView();

        btn2 = (Button)findViewById(R.id.btn2);
        btn3 = (Button)findViewById(R.id.btn3);
        btn4 = (Button)findViewById(R.id.btn4);
        btn5 = (Button)findViewById(R.id.btn5);
        btn6 = (Button)findViewById(R.id.btn6);
        btn7 = (Button)findViewById(R.id.btn7);
        btn8 = (Button)findViewById(R.id.btn8);
        btn9 = (Button)findViewById(R.id.btn9);
        btn10 = (Button)findViewById(R.id.btn10);
        btn11 = (Button)findViewById(R.id.btn11);
        btn12 = (Button)findViewById(R.id.btn12);
        btn13 = (Button)findViewById(R.id.btn13);
        btn2.setOnClickListener(this);
        btn3.setOnClickListener(this);
        btn4.setOnClickListener(this);
        btn5.setOnClickListener(this);
        btn6.setOnClickListener(this);
        btn7.setOnClickListener(this);
        btn8.setOnClickListener(this);
        btn9.setOnClickListener(this);
        btn10.setOnClickListener(this);
        btn11.setOnClickListener(this);
        btn12.setOnClickListener(this);
        btn13.setOnClickListener(this);

        size = (SeekBar)findViewById(R.id.size);
        size.setProgress(5);
        size.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                if(surfaceView != null) {
                    surfaceView.setSize(i);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        alpha = (SeekBar)findViewById(R.id.alpha);
        alpha.setProgress(255);
        alpha.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                mAlpha = i;
                int color = colors[colorPos];
                color = Color.argb(mAlpha,Color.red(color),Color.green(color),Color.blue(color));
                surfaceView.setColor(color);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

//        自动播放
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                surfaceView.setData();
                surfaceView.play();
            }
        },500);

    }

    private void initSurfaceView(){
        surfaceView = new GLESSurfaceView(this);
        surfaceRoot.removeAllViews();
        surfaceRoot.addView(surfaceView);
        surfaceView.setColor(colors[colorPos]);
        getData();
    }
    private int screenSizePos = 0;
    private int colorPos = 0;
    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.btn2:
//                surfaceView.mRenderer.setOpMode(0);
                surfaceView.setMode(0);
                break;
            case R.id.btn3://直线
//                surfaceView.mRenderer.setOpMode(5);
            surfaceView.setMode(2);
                break;

            case R.id.btn4:
                Log.i("MainActivity","-------"+colors[colorPos]);
                surfaceView.undo();
                break;
            case R.id.btn5:
                surfaceView.redo();
                break;

            case R.id.btn6:
//                surfaceView.mRenderer.exit();
                screenSizePos++;
                if(screenSizePos > 2){
                    screenSizePos = 0;
                }
                screenWH = ScreenRate[screenSizePos];
//                initSurfaceView();
                Toast.makeText(this,screenWH[0]+"X"+screenWH[1],Toast.LENGTH_SHORT).show();
                surfaceView.setScreenWH(screenWH);
                surfaceView.mRenderer.setOpMode(4);
                surfaceView.requestRender();
                break;
            case R.id.btn7:
                saveData();
                break;
            case R.id.btn8:
                surfaceView.setData();
//                surfaceView.play();
                break;
            case R.id.btn9:
                colorPos++;
                if(colorPos >= colors.length){
                    colorPos = 0;
                }
                int color = colors[colorPos];
                color = Color.argb(mAlpha,Color.red(color),Color.green(color),Color.blue(color));
                surfaceView.setColor(color);

                break;
            case R.id.btn10:
                surfaceView.setMode(1);
                break;
            case R.id.btn11:
                surfaceView.setMode(3);
                break;
            case R.id.btn12:
                surfaceView.setMode(100);
                break;
            case R.id.btn13:
                surfaceView.getImageData();
                break;
        }
    }

    private String path;

    private void saveData(){
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(new FileOutputStream(path,false));
            oos.writeObject(surfaceView.drawBeans);
            oos.flush();
            Toast.makeText(this,"保存成功",Toast.LENGTH_SHORT).show();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                if(oos != null){
                    oos.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void getData(){
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(new FileInputStream(path));
            surfaceView.drawBeans = (ArrayList<DrawBean>)ois.readObject();
        } catch (IOException e) {
            e.printStackTrace();
        }catch (ClassNotFoundException e) {
            e.printStackTrace();
        }finally {
            if(ois != null){
                try {
                    ois.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
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

    @Override
    public void showDialog(String pTitle, String pMessage) {

    }

    @Override
    public void runOnGLThread(Runnable pRunnable) {

    }
}

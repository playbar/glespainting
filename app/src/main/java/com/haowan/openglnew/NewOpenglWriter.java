package com.haowan.openglnew;

import android.app.Activity;
import android.app.Application;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.RequiresApi;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.haowan.opengl_new.R;
import com.haowan.openglnew.Constants.Constants;
import com.haowan.openglnew.adapter.ColorAdapter;
import com.haowan.openglnew.adapter.LayerAdapter;
import com.haowan.openglnew.bean.DrawBean;
import com.haowan.openglnew.bean.DrawLayer;
import com.haowan.openglnew.dialog.LayerSettingPopWindow;
import com.haowan.openglnew.dialog.PaintPopWindow;
import com.haowan.openglnew.dialog.PickColorPopWindow;
import com.haowan.openglnew.dialog.RedoPopWindow;
import com.haowan.openglnew.util.ColorUtil;
import com.haowan.openglnew.util.PGUtil;
import com.haowan.openglnew.view.MyToast;
import com.haowan.openglnew.view.colorpick.ColorPickerDialog;
import com.mobeta.android.dslv.DragSortListView;

import org.cocos2dx.lib.Cocos2dxHelper;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by xcf on 2017/12/3.
 */

public class NewOpenglWriter extends Activity implements View.OnClickListener,
        PaintPopWindow.PaintOperate,
        RedoPopWindow.RedoCallback,
        GLESSurfaceView.MyGestureListener,
        LayerSettingPopWindow.LayerCallback,
        Cocos2dxHelper.Cocos2dxHelperListener
{

    private int screenWidth,screenHeight;
    private int thresholdX,thresholdYBottom,thresholdYTop;//图层view半透明阀值

    /**
     * 画笔的颜色
     */
    private int color[] = {Color.BLACK, Color.WHITE, 0xffef7474, 0xffeadf1c,
            0xff8bbd5a, 0xff3e7cbf, 0xffd781f2, 0xfff7c09b, 0xfffedcba};
    private int eraser = Color.WHITE;//Color.BLACK=-1677721   Color.GRAY=-7829368
    /**
     * 使用过的颜色
     */
    private int[] useColor = {
            0xff868686, 0xff868686, 0xff868686, 0xff868686, 0xff868686,
            0xff868686, 0xff868686, 0xff868686, 0xff868686, 0xff868686,
            0xff868686, 0xff868686, 0xff868686, 0xff868686, 0xff868686,
            0xff868686, 0xff868686, 0xff868686, 0xff868686, 0xff868686,
            0xff868686, 0xff868686, 0xff868686, 0xff868686, 0xff868686};
    GridView baseColorView;
    GridView usedGrid;
    ColorAdapter usedGridAdapter;
    int currColor = 0xff0000ff;
    private int mAlpha = 255;


    //更多弹出框
    PaintPopWindow ppw;
    RedoPopWindow redoPopWindow;
    PickColorPopWindow pickColorWindow;

    //绘画界面
    RelativeLayout surfaceRoot;
    GLESSurfaceView surfaceView;

    //粗细seekbar父容器
    private RelativeLayout size_layout;
    //透明度seekbar父容器
    private RelativeLayout alpha_layout;
    private SeekBar mSizeBar;
    private SeekBar mAlphaBar;


    private View bottomBar;//底部按钮条父容器
    private View mMoreTab;//更多
    private TextView note_layer_tab;//图层按钮
    private View mOneBackTab;//单步回退
    private View normalPaint;//普通画笔

    /**取色相关*/
    private View mPickColorTab;
    private ImageView mPickImage;
    private ImageView mPickColor;
    private TextView mPickText;
    private View note_color_bar;//颜色条
    MyToast mToast;//画笔颜色提示

    //图层相关view
    private LinearLayout layer_layout;
    private DragSortListView layer_listview;
    private LayerAdapter layerAdapter;
    private List<DrawLayer> layerList;
    private ImageView layer_add;

    @Override
    protected void onDestroy() {
        super.onDestroy();
        dismissRedo();
        surfaceView.exit();
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Cocos2dxHelper.init(this);
        setContentView(R.layout.note_for_write_new);
        initData();
        initView();
    }

    private void initData(){
        DisplayMetrics dm = new DisplayMetrics();
        WindowManager windowManager = (WindowManager) getSystemService(Application.WINDOW_SERVICE);
        Display d = windowManager.getDefaultDisplay();
        d.getMetrics(dm);
        screenWidth = dm.widthPixels;
        screenHeight = dm.heightPixels;

        thresholdX = PGUtil.dip2px(this,80);
        thresholdYBottom = screenHeight - PGUtil.dip2px(this,180);
        thresholdYTop = thresholdYBottom - PGUtil.dip2px(this,360);

        path = getSdPath()+"/opengltestfile.txt";
        File file = new File(path);
        if(!file.exists()){
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        layerList = new ArrayList<>();
        //默认加入一个图层
        for(int i = 0;i < 1;i++){
            layerList.add(new DrawLayer(i));
        }

    }

    private void initView(){
        initNormalView();
        initSeekbar();
        initLayerView();
        initSurfaceView();
        initColorView();
    }

    private void initNormalView(){

        normalPaint = findViewById(R.id.normal_paint);
        normalPaint.setOnClickListener(this);
        bottomBar = findViewById(R.id.note_bottom_bar);
        mMoreTab = findViewById(R.id.note_mood_tab);
        mMoreTab.setOnClickListener(this);
        note_layer_tab = (TextView)findViewById(R.id.note_layer_tab);
        note_layer_tab.setOnClickListener(this);
        mOneBackTab = findViewById(R.id.note_one_back);
        mOneBackTab.setOnClickListener(this);
        //取色相关
        mPickColorTab = findViewById(R.id.note_pickcolor_tab);
        mPickColorTab.setOnClickListener(this);
        mPickImage = (ImageView) findViewById(R.id.pickcolor_image);
        mPickColor = (ImageView) findViewById(R.id.pickcolor_color);
        mPickText = (TextView) findViewById(R.id.pickcolor_text);
    }
    /**
     * 初始化进度条
     */
    private void initSeekbar(){
        mSizeBar = (SeekBar)findViewById(R.id.size_seekbar);
        mAlphaBar = (SeekBar)findViewById(R.id.alpha_seekbar);
        mSizeBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                if(surfaceView != null) {
                    surfaceView.setSize(i);
                }
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                dismissRedo();
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
        mAlphaBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                mAlpha = i;
                setColor(currColor);
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                dismissRedo();
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

    }

    /**
     * 初始化图层
     */
    private void initLayerView(){
        layer_layout = (LinearLayout)findViewById(R.id.layer_layout);
        layer_add = (ImageView)findViewById(R.id.layer_add);
        layer_add.setOnClickListener(this);
        layer_listview = (DragSortListView)findViewById(R.id.layer_listview);
        layer_listview.setDropListener(onDrop);
        layerAdapter = new LayerAdapter(this,layerList,this);
        layer_listview.setAdapter(layerAdapter);
        layer_listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, final View view, int i, long l) {
                LayerSettingPopWindow pop = new LayerSettingPopWindow(NewOpenglWriter.this,NewOpenglWriter.this, i, layerList.get(i));
                pop.show(view);
            }
        });
    }
    private DragSortListView.DropListener onDrop = new DragSortListView.DropListener() {
        @Override
        public void drop(int from, int to) {
            DrawLayer item = layerAdapter.getItem(from);
            layerAdapter.remove(item);
            layerAdapter.insert(item, to);
            //TODO 图层交换
        }
    };

    /**
     * 初始化绘画view
     */
    private void initSurfaceView(){
        surfaceView = new GLESSurfaceView(this)
        ;
        surfaceRoot = (RelativeLayout)findViewById(R.id.paint_for_note);
        surfaceRoot.removeAllViews();
        surfaceRoot.addView(surfaceView);
        surfaceView.setColor(currColor);
        surfaceView.setMyGestureListener(this);
        surfaceView.setOnTouchListener(new View.OnTouchListener() {
            @RequiresApi(api = Build.VERSION_CODES.HONEYCOMB)
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()){
                    case MotionEvent.ACTION_DOWN:
                        dismissRedo();
                        break;
                    case MotionEvent.ACTION_MOVE:
                        if(motionEvent.getX() <= thresholdX && (motionEvent.getY() > thresholdYTop && motionEvent.getY() < thresholdYBottom)){
                            layer_layout.setAlpha(0.3f);
                        }
                        break;
                    case MotionEvent.ACTION_UP:
                        layer_layout.setAlpha(1f);
                        break;
                }
                return false;
            }
        });
        getData();
    }

    /**
     * 初始化颜色view
     */
    private void initColorView() {
        mToast = new MyToast(this);
        int itemWidth = (screenWidth - PGUtil.dip2px(this,30)) / 10;
        note_color_bar = findViewById(R.id.note_color_bar);
        //基本色gridview设置
        baseColorView = (GridView) findViewById(R.id.gridcolor1);
        baseColorView.getLayoutParams().width = itemWidth * 9;
        baseColorView.setAdapter(new ColorAdapter(this, color, itemWidth, true));
        baseColorView.setOnItemClickListener(colorItemclick);

        HorizontalScrollView user_grid_root = (HorizontalScrollView)findViewById(R.id.user_grid_root);
        user_grid_root.getLayoutParams().width = itemWidth * 9;
        //获取记忆色
        userColor();
        // 记忆色gridview设置
        usedGrid = (GridView) findViewById(R.id.usedGrid1);
        usedGrid.setNumColumns(25);
        usedGrid.getLayoutParams().width = itemWidth * 25;
        usedGridAdapter = new ColorAdapter(this, useColor, itemWidth, false);
        usedGrid.setAdapter(usedGridAdapter);
        usedGrid.setOnItemClickListener(colorItemclick);

        //调色盘
        ImageView palette_iv = (ImageView) findViewById(R.id.palette_iv);
        palette_iv.getLayoutParams().height = itemWidth - 15;
        RelativeLayout palette_relative = (RelativeLayout) findViewById(R.id.palette_relative);
        palette_relative.getLayoutParams().height = itemWidth;
        palette_relative.setOnClickListener(this);

        //橡皮擦
        ImageView eraser_iv = (ImageView) findViewById(R.id.eraser_iv);
        eraser_iv.getLayoutParams().height = itemWidth - 15;
        RelativeLayout eraser_relative = (RelativeLayout) findViewById(R.id.eraser_relative);
        eraser_relative.getLayoutParams().height = itemWidth;
        eraser_relative.setOnClickListener(this);
    }

    AdapterView.OnItemClickListener colorItemclick = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
                                long arg3) {
            switch (arg0.getId()) {
                case R.id.gridcolor1:
                    currColor = color[arg2];
                    mToast.showColorToast(currColor);
                    mPickColor.setColorFilter(currColor);
                    setColor(currColor);
                    dismissColorBar();
                    break;
                case R.id.usedGrid1:
                    currColor = useColor[arg2];
                    mToast.showColorToast(currColor);
                    mPickColor.setColorFilter(currColor);
                    setColor(currColor);
                    dismissColorBar();
                    break;
                default:
                    break;
            }
        }
    };
    private void setColor(int currColor){
        int color = Color.argb(mAlpha,Color.red(currColor),Color.green(currColor),Color.blue(currColor));
        surfaceView.setColor(color);
    }

    /**
     * 取色
     */
    private void pickColor() {
        // 取色模式
        if (note_color_bar.isShown()) {
            dismissColorBar();
        } else {
            surfaceView.setMode(100);
            showColorBar();
        }
    }
    //显示取色条
    private void showColorBar(){
        mPickImage.setImageResource(R.drawable.icon_edit_colorpick_cancel);
        mPickText.setText(R.string.per_pickcolor_cancel);
        note_color_bar.setVisibility(View.VISIBLE);
    }

    //隐藏取色条
    private void dismissColorBar(){
        mPickImage.setImageResource(R.drawable.icon_edit_colorpick);
        mPickText.setText(R.string.per_pickcolor);
        note_color_bar.setVisibility(View.INVISIBLE);
        surfaceView.setMode(0);
    }

    /**
     * 刷新使用颜色列表
     */
    private void refreshUsedColor() {
        userColor();
        if (usedGridAdapter != null) {
            usedGridAdapter.notifyDataSetChanged();
        }
    }

    /**
     * 获取记忆色
     */
    private void userColor() {
        // 记忆色
        String usedColor = ColorUtil.readColortoFile(this);
        if (!PGUtil.isStringNull(usedColor)) {
            int[] tempColor = PGUtil.getIntArrayByString(usedColor);
            for (int i = tempColor.length - 1; i >= 0; i--) {
                useColor[tempColor.length - 1 - i] = tempColor[i];
            }
        }
    }




    private void showMore() {
        if (ppw == null) {
            // 基本功能
            int[][] baseFunc = new int[][]{{R.drawable.paint_fun_delete_selector, R.drawable.base_commit_selector,
                    R.drawable.base_save_selector, R.drawable.base_share_selector},
                    {R.string.per_delete, R.string.per_commit, R.string.per_just_save, R.string.share}};
            // 画笔高级功能
            int[][] paintFun = new int[][]{
                    {R.drawable.blur, R.drawable.paint_fun_line,R.drawable.paint_leaf, R.drawable.brush_69_paint, R.drawable.brush_112_paint,
                            R.drawable.brush_19_paint, R.drawable.symmetry_preview, R.drawable.repair_icon, R.drawable.play_review},//,R.drawable.paint_pencil,R.drawable.paint_pencil
                    {R.string.blur_paint, R.string.draw_line, R.string.expend_paint, R.string.paint_crayon, R.string.paint_marker, R.string.paint_oil, R.string.mode_symmetry, R.string.repeat_draft, R.string.play_review}};//,R.string.paint_pen,R.string.paint_flat
            //更多对话框显示模式(接龙子作品则不显示画布比例调节选项)
            int mode = 0;
            ppw = new PaintPopWindow(this, mode, baseFunc, paintFun, this);
        }
        ppw.show(bottomBar, mMoreTab.getHeight());
    }

    private void showRedo(){
        if(redoPopWindow == null){
            redoPopWindow = new RedoPopWindow(this,mOneBackTab,this);
        }
        if(!redoPopWindow.isShowing()) {
            redoPopWindow.show();
        }
    }

    private void dismissRedo(){
        if(redoPopWindow != null && redoPopWindow.isShowing()){
            redoPopWindow.dismiss();
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.note_mood_tab: // 更多
                dismissRedo();
                dismissColorBar();
                showMore();
                break;
            case R.id.note_layer_tab://图层
                dismissRedo();
                dismissColorBar();
                if(layer_layout.isShown()){
                    layer_layout.setVisibility(View.INVISIBLE);
                }else{
                    layer_layout.setVisibility(View.VISIBLE);
                }
                break;
            case R.id.note_pickcolor_tab://取色
                dismissRedo();
                pickColor();
                break;
            case R.id.palette_relative://调色盘
                showCPDialog(null);
                dismissColorBar();
                break;
            case R.id.eraser_relative://橡皮擦(直接使用白色作为橡皮擦)
                currColor = eraser;
                mPickColor.setColorFilter(currColor);
                setColor(currColor);
                dismissColorBar();
                break;
            case R.id.note_one_back://回退
                dismissColorBar();
                surfaceView.undo();
                showRedo();
                break;
            case R.id.normal_paint://切换普通笔
                setPaintMode(Constants.PaintMode.PAINT_MODE_NORMAL);
                break;
            case R.id.layer_add:
                addLayer();
                break;

        }
    }

    //取色器
    private ColorPickerDialog cpDialog;
    protected void showCPDialog(Bundle state) {
        cpDialog = new ColorPickerDialog(this, currColor);
        cpDialog.setOnColorChangedListener(
                new ColorPickerDialog.OnColorChangedListener() {
                    @Override
                    public void onColorChanged(int color) {
                        currColor = color;
                        ColorUtil.WriteColortoFile(NewOpenglWriter.this, "" + color);
                        refreshUsedColor();
                        mPickColor.setColorFilter(currColor);
                        setColor(currColor);
                    }
                });
        cpDialog.setAlphaSliderVisible(true);
        cpDialog.setHexValueEnabled(true);
        if (state != null) {
            cpDialog.onRestoreInstanceState(state);
        }
        cpDialog.show();
    }

    private void addLayer(){
        int mid = 0;
        if(!layerList.isEmpty()){
            mid = layerList.get(layerList.size() - 1).getLayerId();
        }
        layerList.add(new DrawLayer(mid+1));
        layerAdapter.notifyDataSetChanged();
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
    public void redo() {
        surfaceView.redo();
    }

    @Override
    public void clearNote() {
    }

    @Override
    public void commitNote() {

    }

    @Override
    public void saveNote() {
        saveData();
    }

    @Override
    public void shareNote() {

    }

    @Override
    public void createCanvasByScale(int[] scale) {
        surfaceView.setScreenWH(scale);
        surfaceView.mRenderer.setOpMode(4);
        surfaceView.requestRender();
    }

    @Override
    public void setPaintMode(int paintMode) {
        surfaceView.setMode(paintMode);
        if(paintMode == Constants.PaintMode.PAINT_MODE_NORMAL){
            normalPaint.setVisibility(View.INVISIBLE);
        }else {
            normalPaint.setVisibility(View.VISIBLE);
        }
    }

    @Override
    public void symmetry() {

    }

    @Override
    public void repairDraft() {

    }

    @Override
    public void playDraft() {
        surfaceView.setData();
    }

    @Override
    public void onGetColor(int x, int y, int color) {
        if(pickColorWindow != null) {
            pickColorWindow.update(x, y, color);
        }
    }

    @Override
    public void onGetColorBegin(int x, int y, int color) {
        if(pickColorWindow == null){
            pickColorWindow = new PickColorPopWindow(this,surfaceView);
        }
        pickColorWindow.show(x,y,color);
    }

    @Override
    public void onGetColorEnd(int x, int y, int color) {
        if(pickColorWindow != null) {
            pickColorWindow.dismiss();
        }
        mPickColor.setColorFilter(color);
        setColor(color);
        dismissColorBar();
    }

    @Override
    public void onGetColorCancel() {

    }

    @Override
    public void strokeNum(int size) {

    }

//---------------------------------图层相关回调----------------------------------
    @Override
    public void layerMode(int position, int[] mode) {
//        Toast.makeText(this, "layerMode,position:"+position, Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {
            layerList.get(position).setLayerMode(mode);
        }

    }

    @Override
    public void layerLock(int position) {
//        Toast.makeText(this, "layerLock,position:"+position, Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {
            layerList.get(position).setIsLocked(layerList.get(position).getLockedOpposite());
            layerAdapter.notifyDataSetChanged();
        }

    }

    @Override
    public void layerCopy(int position) {
//        Toast.makeText(this, "layerCopy", Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {

        }
    }

    @Override
    public void layerMerge(int position) {
//        Toast.makeText(this, "layerMerge", Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {

        }
    }

    @Override
    public void layerLockObj(int position) {
//        Toast.makeText(this, "layerLockObj", Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {

        }
    }

    @Override
    public void layerMove(int position) {
//        Toast.makeText(this, "layerMove", Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {
            layerList.get(position).setIsMoving(layerList.get(position).getMovingOpposite());
        }
    }

    @Override
    public void layerDelete(int position) {
//        Toast.makeText(this, "layerDelete", Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {
            if(layerList.size() == 1){
                Toast.makeText(this, "不可以删除最后一个图层", Toast.LENGTH_SHORT).show();
            }else {
                layerAdapter.remove(layerAdapter.getItem(position));
            }
        }
    }

    @Override
    public void layerAlphaChange(int position,int mAlpha) {
//        Toast.makeText(this, "layerAlphaChange,position:"+position, Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {
            layerList.get(position).setAlpha(mAlpha);
            layerAdapter.notifyDataSetChanged();
        }
    }

    @Override
    public void layerSee(int position) {
//        Toast.makeText(this, "layerSee,position:"+position, Toast.LENGTH_SHORT).show();
        if(isListNotNull(position)) {
            layerList.get(position).setIsVisible(layerList.get(position).getVisibleOpposite());
            layerAdapter.notifyDataSetChanged();
        }
    }

    private boolean isListNotNull(int position){
        if(layerList != null && position < layerList.size()){
            return true;
        }
        return false;
    }

    @Override
    public void showDialog(String pTitle, String pMessage) {

    }

    @Override
    public void runOnGLThread(Runnable pRunnable) {

    }
}

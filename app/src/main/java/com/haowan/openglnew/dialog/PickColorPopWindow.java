package com.haowan.openglnew.dialog;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.PopupWindow;

import com.haowan.openglnew.R;
import com.haowan.openglnew.util.PGUtil;

/**
 * Created by 熊翠藩 on 2017/1/9.
 */
public class PickColorPopWindow extends PopupWindow {
    private Context context;
    private LayoutInflater inflater;
    private View paintView;
    private static int[] pVLocation = new int[2];
    private int windowSize;
    private View rootView;
    private ImageView imageView;


    public PickColorPopWindow(Context c, View paintView){
        super(c);
        this.context = c;
        this.paintView = paintView;
        this.paintView.getLocationInWindow(pVLocation);
        windowSize = PGUtil.getScreenWidth(c)/2;
        initView();
    }

    private void initView(){
        inflater = LayoutInflater.from(context);
        rootView = inflater.inflate(R.layout.get_color_pop, null);
        imageView = (ImageView)rootView.findViewById(R.id.getcolor_color);
        this.setBackgroundDrawable(new BitmapDrawable());
        // 设置SelectPicPopupWindow的View
        this.setContentView(rootView);
        // 设置SelectPicPopupWindow弹出窗体的宽
        this.setWidth(windowSize);
        // 设置SelectPicPopupWindow弹出窗体的高
        this.setHeight(windowSize);
        this.setFocusable(false);
    }

    public void show(int x,int y,int color){
        imageView.setColorFilter(color);
        int i = x - this.getWidth() / 2 + pVLocation[0];
        int j = y - this.getHeight() / 2 + pVLocation[1];
        showAtLocation(paintView, Gravity.NO_GRAVITY,i,j);
    }

    public void update(int x,int y,int color){
        int i = x - this.getWidth() / 2 + pVLocation[0];
        int j = y - this.getHeight() / 2 + pVLocation[1];
        imageView.setColorFilter(color);
        update(i, j, -1, -1);
    }

}

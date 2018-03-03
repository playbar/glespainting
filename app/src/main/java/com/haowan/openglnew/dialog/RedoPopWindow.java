package com.haowan.openglnew.dialog;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.PopupWindow;

import com.haowan.opengl_new.R;

/**
 * 单步恢复按钮显示框
 * Created by xcf on 2018/2/28.
 */

public class RedoPopWindow extends PopupWindow implements OnClickListener{

    private LayoutInflater inflater;
    private RedoCallback redoCallback;
    private View outView;

    public void setRedoCallback(RedoCallback redoCallback) {
        this.redoCallback = redoCallback;
    }

    public RedoPopWindow(Context context, View outView, RedoCallback redoCallback){
        inflater = LayoutInflater.from(context);
        this.redoCallback = redoCallback;
        this.outView = outView;
        initView();
    }

    private void initView(){
        View view = inflater.inflate(R.layout.recover, null);
        ImageButton btn = (ImageButton) view.findViewById(R.id.recover_image);
        btn.setOnClickListener(this);
        this.setBackgroundDrawable(new BitmapDrawable());
        this.setWidth(outView.getWidth());
        this.setHeight(outView.getHeight());
        this.setFocusable(false);
        this.setOutsideTouchable(false);
        this.setContentView(view);
    }

    public void show(){
        int[] location = new int[2];
        outView.getLocationOnScreen(location);
        int x = location[0];
        showAtLocation(outView, Gravity.BOTTOM | Gravity.LEFT, x, outView.getHeight() + 3);
    }

    @Override
    public void dismiss() {
        super.dismiss();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.recover_image:
                if(redoCallback != null){
                    redoCallback.redo();
                }
                break;
        }
    }

    public interface RedoCallback{
        void redo();
    }

}

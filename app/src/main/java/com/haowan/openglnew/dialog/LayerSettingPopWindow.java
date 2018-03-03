package com.haowan.openglnew.dialog;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AbsListView;
import android.widget.ImageView;
import android.widget.PopupWindow;
import android.widget.SeekBar;
import android.widget.TextView;

import com.haowan.opengl_new.R;
import com.haowan.openglnew.bean.LayerUiBean;
import com.haowan.openglnew.util.PGUtil;

/**
 * Created by Mypc on 2018/2/24 0024.
 */

public class LayerSettingPopWindow extends PopupWindow implements View.OnClickListener,LayerModeCheckDialog.LayerModeCallback {
    private Context context;
    private LayerCallback callback;
    private int position;
    private LayerUiBean layerBean;

    public void setCallback(LayerCallback callback) {
        this.callback = callback;
    }

    public LayerSettingPopWindow(Context context, LayerCallback callback, int position, LayerUiBean layerBean){
        super(context);
        this.context = context;
        this.callback = callback;
        this.position = position;
        this.layerBean = layerBean;
        initView();
    }
    TextView modeView;
    private void initView(){
        LayoutInflater inflater = LayoutInflater.from(context);
        View view = inflater.inflate(R.layout.layer_setting_layout,null);
        modeView = (TextView)view.findViewById(R.id.layer_mode);
        modeView.setText(layerBean.getLayerMode()[1]);
        PGUtil.setTextViewDrawable(context,modeView,R.drawable.new_arrow_down,PGUtil.dip2px(context,6),2);
        modeView.setOnClickListener(this);
        ImageView layer_lock = (ImageView)view.findViewById(R.id.layer_lock);
        ImageView layer_copy = (ImageView)view.findViewById(R.id.layer_copy);
        ImageView layer_down = (ImageView)view.findViewById(R.id.layer_down);
        ImageView layer_lock_obj = (ImageView)view.findViewById(R.id.layer_lock_obj);
        ImageView layer_move = (ImageView)view.findViewById(R.id.layer_move);
        ImageView layer_delete = (ImageView)view.findViewById(R.id.layer_delete);
        final TextView layer_alpha = (TextView)view.findViewById(R.id.layer_alpha);
        SeekBar layer_seekbar = (SeekBar)view.findViewById(R.id.layer_seekbar);


        layer_move.setBackgroundResource(layerBean.getIsMoving() == 0 ? R.drawable.transparent : R.drawable.shape_bg_f5f5f5_r4);
        layer_lock.setImageResource(layerBean.getIsLocked() == 0 ? R.drawable.layer_unlock : R.drawable.layer_lock);
        layer_seekbar.setProgress(layerBean.getAlpha());
        layer_alpha.setText(layerBean.getAlphaRate() + "%");

        layer_lock.setOnClickListener(this);
        layer_copy.setOnClickListener(this);
        layer_down.setOnClickListener(this);
        layer_lock_obj.setOnClickListener(this);
        layer_move.setOnClickListener(this);
        layer_delete.setOnClickListener(this);
        layer_seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                if(callback != null){
                    callback.layerAlphaChange(position, i);
                    int alphaRate = (int)((i * 1.0f / 255) * 100);
                    layer_alpha.setText(alphaRate + "%");
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        this.setBackgroundDrawable(new BitmapDrawable());
        this.setContentView(view);
        // 设置SelectPicPopupWindow弹出窗体的宽
        this.setWidth(AbsListView.LayoutParams.WRAP_CONTENT);
        // 设置SelectPicPopupWindow弹出窗体的高
        this.setHeight(PGUtil.dip2px(context, 80));
        // 设置SelectPicPopupWindow弹出窗体可点击
        this.setFocusable(true);
        this.setOutsideTouchable(true);
        this.setTouchable(true);
    }

    public void show(View view){
        int[] pos = new int[2];
        view.getLocationInWindow(pos);
        showAtLocation(view, Gravity.NO_GRAVITY,pos[0]+view.getWidth(),pos[1]);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.layer_mode:
//                if(callback != null){
//                    callback.layerMode(position);
//                }
                LayerModeCheckDialog modeCheckDialog = new LayerModeCheckDialog(context, this);
                modeCheckDialog.show();
                break;
            case R.id.layer_lock:
                if(callback != null){
                    callback.layerLock(position);
                }
                dismiss();
                break;
            case R.id.layer_copy:
                if(callback != null){
                    callback.layerCopy(position);
                }
                dismiss();
                break;
            case R.id.layer_down:
                if(callback != null){
                    callback.layerMerge(position);
                }
                dismiss();
                break;
            case R.id.layer_lock_obj:
                if(callback != null){
                    callback.layerLockObj(position);
                }
                dismiss();
                break;
            case R.id.layer_move:
                if(callback != null){
                    callback.layerMove(position);
                }
                dismiss();
                break;
            case R.id.layer_delete:
                if(callback != null){
                    callback.layerDelete(position);
                }
                dismiss();
                break;

        }
    }

    @Override
    public void setMode(int[] mode) {
        if(modeView != null && callback != null) {
            modeView.setText(mode[1]);
            callback.layerMode(position, mode);
        }
    }

    public interface LayerCallback{
        void layerMode(int position,int[] mode);
        void layerLock(int position);
        void layerCopy(int position);
        void layerMerge(int position);
        void layerLockObj(int position);
        void layerMove(int position);
        void layerDelete(int position);
        void layerAlphaChange(int position,int alpha);
        void layerSee(int position);
    }

}

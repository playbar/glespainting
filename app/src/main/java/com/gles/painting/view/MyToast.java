package com.gles.painting.view;

import android.content.Context;
import android.os.Handler;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.gles.painting.R;


public class MyToast {

//	private MyToast mToast;
//	private static Context c;
	LayoutInflater inflater;
	// 画笔粗细提示view
	View sizeView;
	TextView paintSizeValue;
	ImageView pSizeView;
	
	// 画笔透明提示view
	View tranView;
	TextView paint_tran_value;
	ImageView circle1, circle2;
	
	//画笔颜色提示view
	View colorView;
	ImageView circle;
	
//	public static MyToast getInstance(Context context){
//		if(mToast == null){
//			mToast = new MyToast(context);
//		}
//		return mToast;
//	}
	
	public MyToast(Context context){
		inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		paintToast = new Toast(context);
		paintToast.setDuration(Toast.LENGTH_SHORT);
		paintToast.setGravity(Gravity.TOP| Gravity.CENTER_HORIZONTAL, 0, 5);
	}
	
	/**
	 * 画笔颜色该表提示
	 */
	public void showColorToast(int color) {
		// 界面提示view
		if (colorView == null) {
			colorView = inflater.inflate(R.layout.change_color_t, null);
			circle = (ImageView) colorView.findViewById(R.id.color_circle);
		}
		circle.setColorFilter(color);
		showPaintToast( colorView);
		new Handler().postDelayed(new Runnable() {
			@Override
			public void run() {
				dismissToast();
			}
		},200);
	}
	
    private Toast paintToast;
    public void showPaintToast(View v){
//    	if (ct != null)
//        {
//            if (c == null || !ct.equals(c) ||paintToast == null) {
//                    c = ct;
//                    paintToast = new Toast(ct);
//                    paintToast.setView(v);
//                    paintToast.setDuration(Toast.LENGTH_SHORT);
//                    paintToast.setGravity(Gravity.TOP|Gravity.CENTER_HORIZONTAL, 0, 5);
//            }else {
            	paintToast.setView(v);
//            }
            paintToast.show();
//        }
    }
    public void dismissToast(){
    	if(paintToast != null){
    		paintToast.cancel();
    	}
    }

}

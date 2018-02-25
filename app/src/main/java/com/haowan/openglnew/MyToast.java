package com.haowan.openglnew;

import android.content.Context;
import android.os.Handler;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.haowan.opengl_new.R;

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
	
//	/**
//	 * 调节尺寸提示
//	 * @param size
//	 * @param isAuto 是否自动隐藏提示框(true-自动隐藏,false-手动隐藏)
//	 */
//	public void showSizeToast(int size,boolean isAuto){
//		// 界面提示
//		if (sizeView == null) {
//			sizeView = inflater.inflate(R.layout.change_size_t, null);
//			pSizeView = (ImageView) sizeView.findViewById(R.id.image1);
//			paintSizeValue = (TextView) sizeView
//					.findViewById(R.id.paint_param_value);
//		}
//		paintSizeValue.setText(String.valueOf(size));
//		pSizeView.setBackgroundColor(RecordPaintInstance.getInstance().color);
//		LayoutParams lp = pSizeView.getLayoutParams();
//		lp.height = RecordPaintInstance.getInstance().size;
//		pSizeView.setLayoutParams(lp);
//		showPaintToast( sizeView);
//		if(isAuto){
//			new Handler().postDelayed(new Runnable() {
//				@Override
//				public void run() {
//					dismissToast();
//				}
//			},200);
//		}
//	}
	
//	/**
//	 * 调节透明提示
//	 * @param alphaRate
//	 * @param isAuto 是否自动隐藏提示框(true-自动隐藏,false-手动隐藏)
//	 */
//	public void showTranToast(int alphaRate,boolean isAuto){
//		if (tranView == null) {
//			tranView = inflater.inflate(R.layout.change_tran_t, null);
//			paint_tran_value = (TextView) tranView
//					.findViewById(R.id.paint_tran_value);
//			circle1 = (ImageView) tranView.findViewById(R.id.circle1);
//			circle2 = (ImageView) tranView.findViewById(R.id.circle2);
//		}
//		circle2.setColorFilter(RecordPaintInstance.getInstance().color);
//		circle2.setAlpha(RecordPaintInstance.getInstance().alpha);
//		circle2.bringToFront();
//		paint_tran_value.setText(alphaRate + "%");
//		showPaintToast( tranView);
//		if(isAuto){
//			new Handler().postDelayed(new Runnable() {
//				@Override
//				public void run() {
//					dismissToast();
//				}
//			},200);
//		}
//	}
	
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
//		new Handler().postDelayed(new Runnable() {
//			@Override
//			public void run() {
//				dismissToast();
//			}
//		},200);
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

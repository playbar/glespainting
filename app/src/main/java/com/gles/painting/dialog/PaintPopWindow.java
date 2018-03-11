package com.gles.painting.dialog;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.Toast;

import com.gles.painting.Constants.Constants;
import com.gles.painting.R;
import com.gles.painting.adapter.MyBaseAdapter;
import com.gles.painting.adapter.SizeAlphaAdapter;

/**
 * 画笔设置popWindow
 * 
 * @author xiongCuiFan
 * 
 */
public class PaintPopWindow {

	private Context c;
	/** 隐藏模块的编号(0-全部显示,1-隐藏右边基本功能模块,2-隐藏上面基本色模块,3-隐藏画笔高级功能) */
	private int mode;
	private int[][] baseFunc;
	private int[][] paintFun;
	private LayoutInflater inflater;
	private PaintOperate mPaintOperate;

	/** 画布比例资源文件 */
	private int[][] scaleResid = {{R.drawable.canvasscale32, R.string.space,1},
			{R.drawable.canvasscale43,R.string.space,1},
			{R.drawable.canvasscale11,R.string.space,1}};
	/** 画布高宽比例数值 */
	private int[][] scaleData = {{800, 1200},{720,1280},{2400,2400}};

	public PaintOperate getmPaintOperate() {
		return mPaintOperate;
	}

	public void setmPaintOperate(PaintOperate mPaintOperate) {
		this.mPaintOperate = mPaintOperate;
	}

	public int getMode() {
		return mode;
	}

	public void setMode(int mode) {
		this.mode = mode;
	}

	/**
	 * 构造器
	 * 
	 * @param context
	 * @param dismissMode
	 *            隐藏模块的编号(0-全部显示,1-隐藏右边基本功能模块,2-隐藏上面基本色模块,3-隐藏画笔高级功能)
	 */
	public PaintPopWindow(Context context, int dismissMode, int[][] basicFun,
						  int[][] paintFun, PaintOperate paintOperate) {
		c = context;
		mode = dismissMode;
		baseFunc = basicFun;
		this.paintFun = paintFun;
		inflater = LayoutInflater.from(c);
		mPaintOperate = paintOperate;
	}

	public View getView() {
		View view = inflater.inflate(R.layout.setpaint_dialog, null);

		// 基本功能
		ListView other_func_listview = (ListView) view
				.findViewById(R.id.other_func_listview);
		other_func_listview.setAdapter(new MyBaseAdapter(c, baseFunc));
		other_func_listview.setOnItemClickListener(itemclick);

		GridView paint_func = (GridView) view.findViewById(R.id.paint_func);
		paint_func.setAdapter(new MyBaseAdapter(c, paintFun));
		paint_func.setOnItemClickListener(itemclick);
		
		GridView canvas_scale = (GridView)view.findViewById(R.id.canvas_scale);
		if(mode == 0){
			canvas_scale.setVisibility(View.VISIBLE);
			canvas_scale.setAdapter(new SizeAlphaAdapter(c, scaleResid));
			canvas_scale.setOnItemClickListener(itemclick);
		}else{
			//隐藏画布比例设置
			canvas_scale.setVisibility(View.GONE);
		}


		return view;
	}

	private PopupWindow pop;

	public void show(View v, int height) {
		dismiss();
		pop = new PopupWindow(getView(), LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT, true);
		pop.setBackgroundDrawable(new BitmapDrawable());
		pop.showAtLocation(v, Gravity.BOTTOM | Gravity.RIGHT, 0, height + 3);
		Log.i("PaintPopWindow", "------------height:" + height);
	}

	public void dismiss() {
		if (pop != null && pop.isShowing()) {
			pop.dismiss();
			pop = null;
		}
	}

	OnItemClickListener itemclick = new OnItemClickListener() {
		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
								long arg3) {
			switch (arg0.getId()) {
			case R.id.other_func_listview:// 基本功能
				switch (arg2) {
				case 0:// 删除
					if (mPaintOperate != null) {
						mPaintOperate.clearNote();
					}
					dismiss();
					break;
				case 1:// 提交
					if (mPaintOperate != null) {
						mPaintOperate.commitNote();
					}
					dismiss();
					break;
				case 2:// 保存
					if (mPaintOperate != null) {
						mPaintOperate.saveNote();
					}
					dismiss();
					break;
				case 3:// 分享
					if (mPaintOperate != null) {
						mPaintOperate.shareNote();
					}
					dismiss();
					break;
				}
				break;
			case R.id.canvas_scale:// 画布比例
//				showCanvasScaleDialog(scaleData[arg2]);
				if (mPaintOperate != null) {
					mPaintOperate.createCanvasByScale(scaleData[arg2]);
				}
				dismiss();
				break;
			case R.id.paint_func:// 画笔功能
				switch (arg2) {
				case 0:// 画模糊
					setPaintMode(Constants.PaintMode.PAINT_MODE_BLUR);
					break;
				case 1:// 直线
					setPaintMode(Constants.PaintMode.PAINT_MODE_LINE);
					break;
				case 2:// 柳叶笔
					setPaintMode(Constants.PaintMode.PAINT_MODE_LEAF);
					break;
				case 3://蜡笔
					setPaintMode(Constants.PaintMode.PAINT_MODE_CRAYON);
					break;
				case 4://马克笔
					setPaintMode(Constants.PaintMode.PAINT_MODE_MARKER);
					break;
				case 5://油笔
					setPaintMode(Constants.PaintMode.PAINT_MODE_OIL);
					break;
				// case 4://铅笔
				// pencilMode();
				// break;
				// case 5://钢笔
				// penMode();
				// break;
				// case 6://平笔
				// flatMode();
				// break;
				case 6:
					if (mPaintOperate != null) {
						mPaintOperate.symmetry();
					}
					dismiss();
					break;
				case 7:
					if(mPaintOperate != null){
						mPaintOperate.repairDraft();
					}
					dismiss();
					break;
				case 8:
					if(mPaintOperate != null){
						mPaintOperate.playDraft();
					}
					dismiss();
					break;

				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	};

//	/**
//	 * 柳叶笔修复提示
//	 */
//	private void showLeafToastDialog(){
//		final CommonDialog dialog = new CommonDialog(c);
//		View view = LayoutInflater.from(c).inflate(R.layout.one_button_dialog,null);
//		TextView tv = (TextView)view.findViewById(R.id.sd_text);
//		tv.setText(R.string.leaf_toast_text);
//		Button confirm_button = (Button)view.findViewById(R.id.confirm_button);
//		confirm_button.setOnClickListener(new OnClickListener() {
//			@Override
//			public void onClick(View view) {
//				dialog.dismiss();
//			}
//		});
//		dialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
//			@Override
//			public void onDismiss(DialogInterface dialogInterface) {
//				HuabaApplication.mSettings.edit().putBoolean(HuabaApplication.LEAF_REPAIR_TOAST_KEY,true).commit();
//			}
//		});
//		dialog.setContentView(view);
//		dialog.setCancelable(true);
//		dialog.show();
//
//	}

//	private void showCanvasScaleDialog(final float scaleRate){
//		UiUtil.showTipDialogWithoutImage(c, UiUtil.getString(R.string.reminder), UiUtil.getString(R.string.canvas_scale_toast),
//				UiUtil.getString(R.string.cancel), UiUtil.getString(R.string.confirm), false, 0, 0,new BaseDialog.OnDialogOperateListener() {
//					@Override
//					public void onLeftBtnClicked() {
//						dismiss();
//					}
//
//					@Override
//					public void onRightBtnClicked(Object object) {
//						if (mPaintOperate != null) {
//							mPaintOperate.createCanvasByScale(scaleRate);
//						}
//						dismiss();
//					}
//
//					@Override
//					public void onCloseBtnClicked() {
//
//					}
//				});
//
//
//		/*final CommonDialog dialog = new CommonDialog(c);
//		View view = LayoutInflater.from(c).inflate(R.layout.two_button_dialog, null);
//		TextView tv = (TextView)view.findViewById(R.id.delete);
//		tv.setText(R.string.canvas_scale_toast);
//		Button confirm_button = (Button)view.findViewById(R.id.confirm_button);
//		Button cancel_button = (Button)view.findViewById(R.id.cancel_button);
//		confirm_button.setOnClickListener(new OnClickListener() {
//			@Override
//			public void onClick(View arg0) {
//				if(mPaintOperate != null){
//					mPaintOperate.createCanvasByScale(scaleRate);
//				}
//				dialog.dismiss();
//				dismiss();
//			}
//		});
//		cancel_button.setOnClickListener(new OnClickListener() {
//			@Override
//			public void onClick(View arg0) {
//				dialog.dismiss();
//				dismiss();
//			}
//		});
//		dialog.setContentView(view);
//		dialog.setCancelable(true);
//		dialog.setCanceledOnTouchOutside(true);
//		dialog.show();*/
//	}

	private void setPaintMode(int paintmode){
		if (mPaintOperate != null) {
			mPaintOperate.setPaintMode(paintmode);
		} else {
			Toast.makeText(c,R.string.not_support,Toast.LENGTH_SHORT).show();
		}
		dismiss();
	}
	
	/**
	 * 绘画界面回调
	 * 
	 * @author xiongCuiFan
	 * 
	 */
	public interface PaintOperate {
		
		//--------------基本功能---------------
		void clearNote();//清空画布
		
		void commitNote();//提交作品
		
		void saveNote();//保存作品
		
		void shareNote();//分享作品
		
		//--------------创建画布----------------
		/** 根据指定的比例创建画布 */
		void createCanvasByScale(int[] scale);

		//--------------画笔功能----------------

		/**
		 * 设置画笔模式
		 */
		void setPaintMode(int paintMode);
		
		/** 图片对称 */
		void symmetry();

		/** 修复草稿 */
		void repairDraft();
		/** 播放草稿 */
		void playDraft();
		
	}

}

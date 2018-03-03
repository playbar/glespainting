package com.haowan.openglnew.view;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ImageView;

public class RoundImageView extends ImageView {
//	private int type;
//	private static final int TYPE_CIRCLE = 0;
//	private static final int TYPE_ROUND = 1;
//
//	private static final int BODER_RADIUS_DEFAULT = 10;
//	private int mBorderRadius;
//
//	private Paint mBitmapPaint;
//	private int mRadius;
//	private Matrix mMatrix;
//	private BitmapShader mBitmapShader;
//	private int mWidth;
//	private RectF mRoundRect;
//
//	public RoundImageView(Context context, AttributeSet attrs) {
//		super(context, attrs);
//		mMatrix = new Matrix();
//		mBitmapPaint = new Paint();
//		mBitmapPaint.setAntiAlias(true);
//
//		TypedArray a = context.obtainStyledAttributes(attrs,
//				R.styleable.RoundImageView);
//
//		mBorderRadius = a.getDimensionPixelSize(
//				R.styleable.RoundImageView_borderRadius, (int) TypedValue
//						.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
//								BODER_RADIUS_DEFAULT, getResources()
//										.getDisplayMetrics()));
//		type = a.getInt(R.styleable.RoundImageView_type, TYPE_ROUND);
//
//		a.recycle();
//	}
//
//	@Override
//	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
//		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
//
//		if (type == TYPE_CIRCLE) {
//			mWidth = Math.min(getMeasuredWidth(), getMeasuredHeight());
//			mRadius = mWidth / 2;
//			setMeasuredDimension(mWidth, mWidth);
//		}
//
//	}
//
//	private void setUpShader() {
//		Drawable drawable = getDrawable();
//		if (drawable == null) {
//			return;
//		}
//
//		Bitmap bmp = drawableToBitamp(drawable);
//		mBitmapShader = new BitmapShader(bmp, TileMode.CLAMP, TileMode.CLAMP);
//		float scale = 1.0f;
//		if (type == TYPE_CIRCLE) {
//			int bSize = Math.min(bmp.getWidth(), bmp.getHeight());
//			scale = mWidth * 1.0f / bSize;
//
//		} else if (type == TYPE_ROUND) {
//			scale = Math.max(getWidth() * 1.0f / bmp.getWidth(), getHeight()
//					* 1.0f / bmp.getHeight());
//		}
//		mMatrix.setScale(scale, scale);
//		mBitmapShader.setLocalMatrix(mMatrix);
//		mBitmapPaint.setShader(mBitmapShader);
//	}
//
//	/**
//	 * drawableתbitmap
//	 * 
//	 * @param drawable
//	 * @return
//	 */
//	private Bitmap drawableToBitamp(Drawable drawable) {
//		if (drawable instanceof BitmapDrawable) {
//			BitmapDrawable bd = (BitmapDrawable) drawable;
//			return bd.getBitmap();
//		}
//		int w = drawable.getIntrinsicWidth();
//		int h = drawable.getIntrinsicHeight();
//		Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
//		Canvas canvas = new Canvas(bitmap);
//		drawable.setBounds(0, 0, w, h);
//		drawable.draw(canvas);
//		return bitmap;
//	}
//
//	@Override
//	protected void onDraw(Canvas canvas) {
//		if (getDrawable() == null) {
//			return;
//		}
//		setUpShader();
//
//		if (type == TYPE_ROUND) {
//			canvas.drawRoundRect(mRoundRect, mBorderRadius, mBorderRadius,
//					mBitmapPaint);
//		} else {
//			canvas.drawCircle(mRadius, mRadius, mRadius, mBitmapPaint);
//			// drawSomeThing(canvas);
//		}
//	}
//
//	@Override
//	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
//		super.onSizeChanged(w, h, oldw, oldh);
//		if (type == TYPE_ROUND)
//			mRoundRect = new RectF(0, 0, getWidth(), getHeight());
//	}
//
//	public void setBorderRadius(int borderRadius) {
//		int pxVal = dp2px(borderRadius);
//		if (this.mBorderRadius != pxVal) {
//			this.mBorderRadius = pxVal;
//			invalidate();
//		}
//	}
//
//	public void setType(int type) {
//		if (this.type != type) {
//			this.type = type;
//			if (this.type != TYPE_ROUND && this.type != TYPE_CIRCLE) {
//				this.type = TYPE_CIRCLE;
//			}
//			requestLayout();
//		}
//	}
//
//	public int dp2px(int dpVal) {
//		return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
//				dpVal, getResources().getDisplayMetrics());
//	}
	
	public RoundImageView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

	public RoundImageView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public RoundImageView(Context context) {
		super(context);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		try {
			super.onDraw(canvas);
		} catch (Exception e) {
			Log.i("ImageViewCatch", "---------------e:"+e.toString());
		}
	}
	public void setBorderRadius(int radius){
		
	}
}

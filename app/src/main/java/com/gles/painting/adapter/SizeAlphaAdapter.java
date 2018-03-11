package com.gles.painting.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.BaseAdapter;
import android.widget.ImageView;

import com.gles.painting.R;
import com.gles.painting.util.PGUtil;

public class SizeAlphaAdapter extends BaseAdapter {

	private Context c;
	private int[][] resid;
	private LayoutInflater inflater;
	private int selectPos = -1;

	public int getSelectPos() {
		return selectPos;
	}

	public void setSelectPos(int selectPos) {
		this.selectPos = selectPos;
	}

	public SizeAlphaAdapter(Context c, int[][] resid){
		this.c = c;
		this.resid = resid;
		inflater = LayoutInflater.from(c);
	}

	@Override
	public int getCount() {
		return resid.length;
	}

	@Override
	public Object getItem(int arg0) {
		return 0;
	}

	@Override
	public long getItemId(int arg0) {
		return arg0;
	}

	@Override
	public View getView(int arg0, View arg1, ViewGroup arg2) {
		View view = inflater.inflate(R.layout.size_alpha_item, null);
		ImageView iv = (ImageView)view.findViewById(R.id.image_item);
		ImageView selectIv = (ImageView)view.findViewById(R.id.image_select);
		iv.getLayoutParams().width = LayoutParams.WRAP_CONTENT;
		iv.getLayoutParams().height = PGUtil.dip2px(c,55);
		iv.setImageResource(resid[arg0][0]);
		if(selectPos >= 0 && selectPos == arg0){
			selectIv.setVisibility(View.VISIBLE);
		}else{
			selectIv.setVisibility(View.INVISIBLE);
		}

		return view;
	}

}

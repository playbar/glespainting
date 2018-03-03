package com.haowan.openglnew.adapter;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.haowan.opengl_new.R;

/**
 * 所有上面为图片下面为文字的列表适配器
 * @author xiongCuiFan
 *
 */
public class MyBaseAdapter extends BaseAdapter {

	private Context c;
	private LayoutInflater inflater;
	/** 适配器所需资源数组[2][n],0-图片资源,1-字符串资源 */
	private int[][] itemArray;
	/** 所在View的高度(首页功能按钮组专用属性) */
	private int height;
	
//	public MyBaseAdapter(Context context,int[][] array,int height){
//		c = context;
//		inflater = LayoutInflater.from(c);
//		itemArray = array;
//		this.height = height;
//	}
	
	public MyBaseAdapter(Context context, int[][] array){
		c = context;
		inflater = LayoutInflater.from(c);
		itemArray = array;
	}

	public int[][] getItemArray() {
		return itemArray;
	}

	public void setItemArray(int[][] itemArray) {
		this.itemArray = itemArray;
	}

	@Override
	public int getCount() {
		if(itemArray == null){
			return 0;
		}else{
			return itemArray[0].length;
		}
		
	}

	@Override
	public Object getItem(int arg0) {
		return arg0;
	}

	@Override
	public long getItemId(int arg0) {
		return arg0;
	}

	@Override
	public View getView(int arg0, View arg1, ViewGroup arg2) {
		ViewHolder holder;
		if(arg1 == null){
			arg1 = inflater.inflate(R.layout.huaba_func_item, null);
			holder = new ViewHolder();
			holder.linear_item = (LinearLayout)arg1.findViewById(R.id.linear_item);
			holder.image_item = (ImageView)arg1.findViewById(R.id.image_item);
			holder.text_item = (TextView)arg1.findViewById(R.id.text_item);
			holder.text_item.setTextColor(Color.WHITE);
			arg1.setTag(holder);
		}else{
			holder = (ViewHolder)arg1.getTag();
		}
		int imageid = itemArray[0][arg0];
		int textid = itemArray[1][arg0];
		if(imageid != 0)
			holder.image_item.setImageResource(imageid);
		if(textid != 0)
			holder.text_item.setText(textid);
		return arg1;
	}
	
	class ViewHolder{
		LinearLayout linear_item;
		ImageView image_item;
		TextView text_item;
	}

}

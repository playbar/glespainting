package com.gles.painting.adapter;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;

import com.gles.painting.R;


public class ColorAdapter extends BaseAdapter
{
	private static final String TAG="ColorAdapter";
	
    private Context context;
    
    private boolean mBaseColor = true;
    
    private int mcolor[];
    
    private int mW;
    
    public int[] getMcolor() {
		return mcolor;
	}

	public void setMcolor(int[] mcolor) {
		this.mcolor = mcolor;
	}

	public ColorAdapter(Context context,
                        int color[], int w, boolean baseColor)
    {
        this.context = context;
        this.mcolor = color;
        this.mBaseColor = baseColor;
        this.mW=w;
    }
    
    @Override
    public int getCount()
    {
    	return mcolor.length;
        
    }
    
    @Override
    public Object getItem(int position)
    {
    	return mcolor[position];
    }
    
    @Override
    public long getItemId(int position)
    {
        return 0;
    }
    
    @Override
    public View getView(int position, View convertView, ViewGroup parent)
    {
        ImageView imageView = new ImageView(context);
//        imageView.setMinimumWidth(mW);
//        imageView.setMaxWidth(mW);
//        imageView.setMinimumHeight(mW);
//        imageView.setMaxHeight(mW);
//        imageView.setBackgroundColor(mcolor[position]);
//        imageView.setColorFilter(mcolor[position]);
        
        imageView.setImageResource(R.drawable.color_item_bg);
        imageView.setScaleType(ScaleType.CENTER_INSIDE);
        imageView.setColorFilter(mcolor[position]);
        
        return imageView;
    }
    
}

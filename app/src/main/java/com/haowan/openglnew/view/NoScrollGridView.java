/**
 * 文件名称 : NoScrollGridView.java
 * <p>
 * 作者信息 : xiong_cuifan
 * <p>
 * 创建时间 : 2013-3-12, 上午10:38:23
 * <p>
 * 版权声明 : Copyright (c) 2009-2012 CIeNET Ltd. All rights reserved
 * <p>
 * 评审记录 :
 * <p>
 */

package com.haowan.openglnew.view;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.GridView;

/**
 * 自身不带滚动的gridView
 * 
 * @see  [相关类/方法]
 * @since  [产品/模块版本]
 */
public class NoScrollGridView extends GridView {

	/** <默认构造函数>
	 */
	public NoScrollGridView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		// TODO Auto-generated constructor stub
	}

	/** <默认构造函数>
	 */
	public NoScrollGridView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	/** <默认构造函数>
	 */
	public NoScrollGridView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}
	
	@Override
    public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) { 

        int expandSpec = MeasureSpec.makeMeasureSpec(
                Integer.MAX_VALUE >> 2, MeasureSpec.AT_MOST);
        super.onMeasure(widthMeasureSpec, expandSpec); 
    } 

}

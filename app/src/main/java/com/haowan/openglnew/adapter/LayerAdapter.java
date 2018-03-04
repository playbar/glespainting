package com.haowan.openglnew.adapter;

import android.annotation.SuppressLint;
import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.haowan.opengl_new.R;
import com.haowan.openglnew.bean.DrawLayer;
import com.haowan.openglnew.dialog.LayerSettingPopWindow;

import java.util.List;

/**
 * Created by Mypc on 2018/2/22 0022.
 */
@SuppressLint("NewApi")
public class LayerAdapter extends ArrayAdapter<DrawLayer> implements View.OnClickListener {

    LayoutInflater inflater;
    LayerSettingPopWindow.LayerCallback callback;

    public LayerAdapter(Context context, List<DrawLayer> objects, LayerSettingPopWindow.LayerCallback callback){
        super(context, 0, objects);
        inflater = LayoutInflater.from(context);
        this.callback = callback;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        ViewHolder holder;
        if(convertView == null){
            convertView = inflater.inflate(R.layout.layer_item_layout,null);
            holder = new ViewHolder();
            holder.layer_id = (TextView) convertView.findViewById(R.id.layer_id);
            holder.layer_lock = (ImageView)convertView.findViewById(R.id.layer_lock);
            holder.layer_see = (ImageView)convertView.findViewById(R.id.layer_see);
            holder.layer_alpha = (TextView)convertView.findViewById(R.id.layer_alpha);
            holder.layer_mode = (TextView)convertView.findViewById(R.id.layer_mode);
            convertView.setTag(holder);
        }else{
            holder = (ViewHolder)convertView.getTag();
        }
        DrawLayer bean = getItem(position);
        holder.layer_id.setText(""+bean.getLayerId());
        holder.layer_alpha.setText(bean.getAlphaRate() + "%");
        holder.layer_lock.setImageResource(bean.getIsLocked() == 0 ? R.drawable.layer_unlock : R.drawable.layer_lock);
        holder.layer_see.setImageResource(bean.getIsVisible() == 0 ? R.drawable.layer_unsee : R.drawable.layer_see);
        holder.layer_lock.setTag(position);
        holder.layer_see.setTag(position);
        holder.layer_lock.setOnClickListener(this);
        holder.layer_see.setOnClickListener(this);

        return convertView;
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.layer_lock:
                if(callback != null){
                    callback.layerLock((int)view.getTag());
                }
                break;
            case R.id.layer_see:
                if(callback != null){
                    callback.layerSee((int)view.getTag());
                }
                break;
        }
    }

    class ViewHolder{
        TextView layer_id;
        ImageView layer_lock;
        ImageView layer_see;
        TextView layer_alpha;
        TextView layer_mode;
    }
}

package com.gles.painting.dialog;

import android.app.Dialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.gles.painting.R;
import com.gles.painting.view.NoScrollListView;

/**
 * Created by xcf on 2018/3/1.
 */

public class LayerModeCheckDialog extends Dialog {
    public static final int[][] mode = {{1, R.string.layer_mode_normal},{2,R.string.layer_mode_multiply},{3,R.string.layer_mode_screen}};
    LayerModeCallback callback;
    LayoutInflater inflater;

    public LayerModeCallback getCallback() {
        return callback;
    }

    public void setCallback(LayerModeCallback callback) {
        this.callback = callback;
    }

    public LayerModeCheckDialog(Context c,LayerModeCallback callback){
        super(c,R.style.VerifyDialog);
        inflater = LayoutInflater.from(c);
        this.callback = callback;
        initView();
    }
    private void initView(){
        View view = inflater.inflate(R.layout.layer_mode_layout, null);
        setContentView(view);
        NoScrollListView listview = (NoScrollListView)view.findViewById(R.id.layer_mode_list);
        listview.setAdapter(new LayerModeAdapter());
        listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                if(callback != null){
                    callback.setMode(mode[i]);
                    dismiss();
                }
            }
        });
    }


    class LayerModeAdapter extends BaseAdapter{

        @Override
        public int getCount() {
            return mode.length;
        }

        @Override
        public Object getItem(int i) {
            return mode[i];
        }

        @Override
        public long getItemId(int i) {
            return i;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {
            view = inflater.inflate(R.layout.layer_mode_item,null);
            TextView tv = (TextView)view.findViewById(R.id.layer_mode_text);
            tv.setText(mode[i][1]);
            return view;
        }
    }

    public interface LayerModeCallback{
        void setMode(int[] mode);
    }

}

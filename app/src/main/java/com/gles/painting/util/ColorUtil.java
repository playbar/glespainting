package com.gles.painting.util;

import android.content.Context;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;

public class ColorUtil {

	private static String TAG = "ColorUtil";

	private static String mName = "usedcolor.txt";

	public synchronized static void WriteColortoFile(Context context,
			String color) {
		Log.d(TAG, "WriteColortoSD-->" + color);

		FileInputStream fis = null;
		String s = "";
		boolean isDelete = false;
		try {
			fis = context.openFileInput(mName);
			InputStreamReader is = new InputStreamReader(fis, "GBK");
			BufferedReader br = new BufferedReader(is);
			s = br.readLine().trim();

			if (null != s && s.contains("|")) {
				if (s.endsWith("|")) {
					s = s.substring(0, s.length() - 1);
				}
				Log.d(TAG, "WriteColortoFile s==" + s);
				String[] a = s.split("\\|");
				for (int i = 0; i < a.length; i++) {
					Log.d(TAG, "WriteColortoSD i==" + i + "-->a=" + a[i]);
				}
				if (a.length >= 25) {
					String fileName = "/data/data/com.haowan.huabar/files/"
							+ mName;
					File f = new File(fileName);
					if (f.exists()) {
						f.delete();
						isDelete = true;
						s = "";
						Log.d(TAG, "---------------");
						for (int i = 1; i < 25; i++) {
							s = s + a[i] + "|";
						}
						Log.d(TAG, "usedColor==" + s);
					}
				}
			}
		} catch (FileNotFoundException e1) {
			Log.e(TAG, "WriteColortoFile FileNotFoundException");
		} catch (Exception e) {
			e.printStackTrace();
		} finally {

			try {
				if(fis != null){
					fis.close();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		try {
			FileOutputStream fos = null;
			fos = context.openFileOutput(mName, Context.MODE_APPEND);

			if (isDelete) {
				Log.e(TAG, "---------------");
				color = s + color + "|";
			} else {
				color = color + "|";
			}

			Log.e(TAG, "color------" + color);
			byte[] buffer = color.getBytes("GBK");
			fos.write(buffer);

			if (null != fos) {
				fos.flush();
				fos.close();
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	public synchronized static String readColortoFile(Context context) {
		FileInputStream fis = null;
		String s = "";
		try {
			fis = context.openFileInput(mName);
			InputStreamReader is = new InputStreamReader(fis, "GBK");
			BufferedReader br = new BufferedReader(is);
			s = br.readLine().trim();

		} catch (FileNotFoundException e1) {
			Log.e(TAG, "readColortoFile FileNotFoundException");
		} catch (Exception e) {
			e.printStackTrace();
			if(null!=fis)
			{
				try {
					fis.close();
				} catch (Exception e1) {
				    e1.printStackTrace();
				}
			}
			return null;
		}
		if (null == s || ("").equals(s))
		{
			if(null!=fis)
			{
				try {
					fis.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			return null;
		}

		if (s.endsWith("|")) {
			s = s.substring(0, s.length() - 1);
		}
		Log.i(TAG, "readColortoFile s==" + s);
		
		if(null!=fis)
		{
			try {
				fis.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return s;
	}

}

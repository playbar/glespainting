
package com.gles.painting;

// Wrapper for native library
public class JNILib {

     public static native void init(int canvasWidth, int canvasHeight, int viewportWidth, int viewportHeight);

     public static native void exit();

     public static native void setTexture(int id, int w, int h, int[] pixelArray);

     public static native void changeCanvas(int width, int height, int color);

     public static native void resize(int width, int height);

     public static native void pan(float x0, float y0, float x1, float y1);
     public static native void zoom(float scale, float cx, float cy);

     // transform screen coordinate into canvas coordinate
     // x,y -- screen coordinate
     // return an array of 2-elements:
     //   1st -- canvas's x coordinate, 2nd -- canvas's y coordinate
     public static native float[] getCanvasCoord(float x, float y);

     //usage:
     // 1.after init, call this function 
     //   to display a blank canvas with background color
     // 2.clear canvas
     public static native void drawBlankCanvas(float r, float g, float b);

     public static native void drawNormalLine(int action, float x, float y, float size, int rgba, int textureID);



//     public static native void drawBlurLine(int action, float x, float y, float size, int rgba, int textureID);
//     public static native void drawSegLine(int action, float x, float y, float size, int rgba, int textureID);
//     public static native void drawLeaf(int action, float x, float y, float size, int rgba, int textureID);
//     public static native void snap(int index);
//     public static native void undo(int baseMapIndex, int cnt, byte[] dat, int len);
//     public static native void redo(byte[] dat, int len);
//
//     public static native void playData(int cnt, byte[] dat, int len);
//     public static native void playDraw(int cnt);
//
//     public static native int pick(int x,int y);
//     public static native byte[] getCanvasContent();
}

/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.haowan.openglnew;

// Wrapper for native library

public class RenderLib {

     public static native void init(int canvasWidth, int canvasHeight, int viewportWidth, int viewportHeight);
//     public static native void init();
     public static native void resize(int width, int height);
     public static native void step();

     public static native void exit();

     public static native void setTexture(int id, int w, int h, int[] pixelArray);

     public static native void changeCanvas(int width, int height, int color);

     public static native void pan(float x0, float y0, float x1, float y1);
     public static native void zoom(float scale, float cx, float cy);

     public static native float[] getCanvasCoord(float x, float y);

     //usage:
     // 1.after init, call this function
     //   to display a blank canvas with background color
     // 2.clear canvas
     public static native void drawBlankCanvas(float r, float g, float b);

     public static native void drawNormalLine(int action, float x, float y, float size, int rgba, int textureID);




}

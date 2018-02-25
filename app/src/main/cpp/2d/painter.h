
#pragma once

struct Vec3{float x,y,z;};

void mh_init(float canvasWidth,  float canvasHeight, 
    float viewportWidth, float viewportHeight);
void mh_exit();

void mh_changeCanvas(float width, float height, int color);

// id: 101 -- normal pen
//     102 -- blur pen
// texture size(width/height) need to be 64/128/256/512/1024...
// id: same as drawNormalLine's parameter -- textureID
// 纹理格式：rgba8888
//color 纹理像素数据
void mh_setTexture(int id, int width, int height, unsigned char* color);

Vec3 mh_getCanvasCoord(int x, int y);

void mh_getCanvasContent(signed char* p);

//usage:
// 1.after init, call this function 
//   to display a blank canvas with background color
// 2.clear canvas
void mh_drawBlankCanvas(float r, float g, float b);

// textureID: same as mh_setTexture's parameter -- id
// x,y: canvas coordinate 
void mh_drawNormalLine(int action, float x, float y, int size, int rgba, int textureID);

void mh_drawBlurLine(int action, float x, float y, int size, int rgba, int textureID);

void mh_drawSegLine(int action, float x, float y, int size, int rgba, int textureID);

void mh_drawLeaf(int action, float x, float y, int size, int rgba, int textureID);

void mh_resize(int width, int height);

void mh_pan(float x0, float y0, float x1, float y1);
void mh_zoom(float scale, float cx, float cy);  //(cx,cy): zoom center

/*action: 0 表示起点,  1 表示中间点, 2 表示终点 */
void mh_erase(int action, float x, float y, float width);

/* pick color: return rgba8888 */
unsigned int mh_pick(int x, int y);

// get the content of canvas 
// buffer size: canvasWidth*canvasHeight*4
//void mh_snap(unsigned char **buffer);

//used to undo, 
//assign idx[uased as baseMapIdx of mh_nudo]
void mh_snap(int idx);
void mh_undo(int baseMapIdx, int cnt, signed char* pdat, int len);
void mh_redo(signed char* pdat, int len);

void mh_play_data(int cnt, signed char* pdat, int len);
void mh_play_draw(int cnt);  //cnt: 本次要一次性绘制的笔数


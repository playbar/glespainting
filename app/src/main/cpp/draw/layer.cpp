
#include <math.h>
#include "LayerManager.h"
#include "vec2.h"
#include "mat4.h"
#include "layer.h"

extern int g_opmode;
extern int g_pencil;
extern int g_width;
extern LayerManager* g_renderer;

LayerID hd_createLayer()
{
    return g_renderer->createLayer();
}


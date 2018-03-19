//
// Created by hou guoli on 2018/3/12.
//

#ifndef GLESPAINTING_GLSL_H
#define GLESPAINTING_GLSL_H



const char VERTEX_SHADER[] =
        "uniform mat4 mvp;\n"
        "attribute vec2 pos;\n"
        "attribute vec2 tcoord;\n"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "  gl_Position = mvp*vec4(pos, 0.0, 1.0);\n"
        "  vTexCoord = tcoord;\n"
        "}\n";

const char FRAGMENT_SHADER[] =
        "precision highp float;\n"
        "uniform int flag;\n"
        "uniform int pcl;\n"
        "uniform vec4 color;\n"
        "uniform sampler2D texture;\n"
        "uniform sampler2D texture1;\n"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "  if(flag==10){\n"
        "    gl_FragColor = texture2D(texture,vTexCoord); \n"
        "    if(pcl==0)\n"
        "      if(gl_FragColor.a<0.82){ discard; return; }\n"
        "    gl_FragColor.a *= color.a;\n"
        "    gl_FragColor.rgb = gl_FragColor.a * color.rgb;\n"
        "  }\n"
        "  else if(flag==2){\n"
        "    gl_FragColor = texture2D(texture,vTexCoord); \n"
        "  }\n"
        "}\n";

const char *vertexShaderprocessRow = R"glsl(
        uniform mat4 mvp;
        attribute vec2 pos;
        attribute vec2 tcoord;
        varying vec2 vUv;
        void main()
        {
           vUv = tcoord;
           gl_Position = mvp * vec4( pos, 0.0, 1.0 );
        }
)glsl";


const char *fragmentShaderprocessRow = R"glsl(
    precision highp float;
    uniform sampler2D texture2;
    uniform vec2 uWH;
    uniform int uPix;
    varying vec2 vUv;
    void main( void )
    {
        vec4 color = vec4( 0.0 );
        float wstep = 1.0/uWH.x;
        for(int i = -uPix; i <= uPix; ++i)
        {
            float ftemp = float(i) * wstep;
            vec4 col = texture2D( texture2, vec2( vUv.x + ftemp, vUv.y ));
            color = color + col;
        }
        float tcou = float(uPix * 2 + 1);
        gl_FragColor = color / tcou;
//        vec4 colorv1 = texture2D( texture2, vec2( vUv.x, vUv.y) );
//        gl_FragColor = colorv1;
    }
)glsl";


const char *vertexShaderprocessCol = R"glsl(
                uniform mat4 mvp;
                attribute vec2 pos;
                attribute vec2 tcoord;
                varying vec2 vUv;
                void main()
                {
                   vUv = tcoord;
                   gl_Position = mvp * vec4( pos, 0.0, 1.0 );
                }
)glsl";

const char *fragmentShaderprocessCol = R"glsl(
    uniform sampler2D texture2;
    uniform vec2 uWH;
    uniform int uPix;
    varying vec2 vUv;
    void main( void )
    {
        vec4 color = vec4( 0.0 );
        float wstep = 1.0/uWH.y;
        for(int i = -uPix; i <= uPix; ++i)
        {
            float ftemp = float(i) * wstep;
            vec4 col = texture2D( texture2, vec2( vUv.x, vUv.y + ftemp ));
            color = color + col;
        }
        float tcou = float(uPix * 2 + 1);
        gl_FragColor = color / tcou;
    }
)glsl";

#endif //GLESPAINTING_GLSL_H

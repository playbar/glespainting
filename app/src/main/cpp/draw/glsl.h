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

const char vertexShaderprocessRow[] =
        "uniform mat4 mvp;\n"
        "attribute vec2 pos;\n"
        "attribute vec2 tcoord;\n"
        "varying vec2 vUv;\n"
        "void main()\n"
        "{\n"
        "   vUv = tcoord;\n"
        "   gl_Position = mvp * vec4( pos, 0.0, 1.0 );\n"
        "}";

const char fragmentShaderprocessRow[]=
        "uniform sampler2D texture2;\n"
        "varying vec2 vUv;\n"
        "void main( void ) {\n"
        " vec4 color = vec4( 0.0 ); \n"
        " float h = 1.0/800.0;\n"
        " vec4 colorh1 = texture2D( texture2, vec2( vUv.x - 4.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh2 = texture2D( texture2, vec2( vUv.x - 3.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh3 = texture2D( texture2, vec2( vUv.x - 2.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh4 = texture2D( texture2, vec2( vUv.x - 1.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh5 = texture2D( texture2, vec2( vUv.x - 0.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh6 = texture2D( texture2, vec2( vUv.x + 1.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh7 = texture2D( texture2, vec2( vUv.x + 2.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh8 = texture2D( texture2, vec2( vUv.x + 3.0 * h, vUv.y ) ) ;\n"
        " vec4 colorh9 = texture2D( texture2, vec2( vUv.x + 4.0 * h, vUv.y ) ) ;\n"
        " color =\n"
        " +colorh1\n"
        " +colorh2\n"
        " +colorh3\n"
        " +colorh4\n"
        " +colorh5\n"
        " +colorh6\n"
        " +colorh7\n"
        " +colorh8\n"
        " +colorh9;\n"
        " gl_FragColor = color/9.0;\n"
        "}";


const char vertexShaderprocessCol[] =
        "uniform mat4 mvp;\n"
                "attribute vec2 pos;\n"
                "attribute vec2 tcoord;\n"
                "varying vec2 vUv;\n"
                "void main()\n"
                "{\n"
                "   vUv = tcoord;\n"
                "   gl_Position = mvp * vec4( pos, 0.0, 1.0 );\n"
                "}";

const char fragmentShaderprocessCol[]=
        "uniform sampler2D texture2;\n"
                "varying vec2 vUv;\n"
                "void main( void ) {\n"
                " vec4 color = vec4( 0.0 ); \n"
                " float h = 1.0/1200.0;\n"
                " vec4 colorv1 = texture2D( texture2, vec2( vUv.x, vUv.y - 4.0 * h ) );\n"
                " vec4 colorv2 = texture2D( texture2, vec2( vUv.x, vUv.y - 3.0 * h ) );\n"
                " vec4 colorv3= texture2D( texture2, vec2( vUv.x, vUv.y - 2.0 * h ) );\n"
                " vec4 colorv4= texture2D( texture2, vec2( vUv.x, vUv.y - 1.0 * h ) );\n"
                " vec4 colorv5= texture2D( texture2, vec2( vUv.x, vUv.y - 0.0 * h ) );\n"
                " vec4 colorv6= texture2D( texture2, vec2( vUv.x, vUv.y + 1.0 * h ) );\n"
                " vec4 colorv7= texture2D( texture2, vec2( vUv.x, vUv.y + 2.0 * h ) );\n"
                " vec4 colorv8= texture2D( texture2, vec2( vUv.x, vUv.y + 3.0 * h ) );\n"
                " vec4 colorv9 = texture2D( texture2, vec2( vUv.x, vUv.y + 4.0 * h ) );\n"
                " color =\n"
                " +colorv1\n"
                " +colorv2\n"
                " +colorv3\n"
                " +colorv4\n"
                " +colorv5\n"
                " +colorv6\n"
                " +colorv7\n"
                " +colorv8\n"
                " +colorv9;\n"
                " gl_FragColor = color/9.0;\n"
                "}";

#endif //GLESPAINTING_GLSL_H

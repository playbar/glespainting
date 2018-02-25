//
// Created by mac on 2018/2/25.
//


#include <GLES2/gl2.h>
#include "gles3jni.h"
#include "mylog.h"

const Vertex QUAD[4] = {
        // Square with diagonal < 2 so that it fits in a [-1 .. 1]^2 square
        // regardless of rotation.
        {{-0.7f, -0.7f}, {0x00, 0xFF, 0x00, 0xFF}},
        {{ 0.7f, -0.7f}, {0x00, 0x00, 0xFF, 0xFF}},
        {{-0.7f,  0.7f}, {0xFF, 0x00, 0x00, 0xFF}},
        {{ 0.7f,  0.7f}, {0xFF, 0xFF, 0xFF, 0xFF}},
};

const Vertex QUAD1[4] = {
        // Square with diagonal < 2 so that it fits in a [-1 .. 1]^2 square
        // regardless of rotation.
        {{-0.7f, -0.7f}, {0xff, 0xFF, 0x00}},
        {{ 0.7f, -0.7f}, {0xff, 0x00, 0xFF}},
        {{-0.7f,  0.7f}, {0xFF, 0x00, 0x00}},
        {{ 0.7f,  0.7f}, {0xFF, 0xFF, 0xFF}},
};

typedef struct
{
    GLuint programObject;
    GLint samplerLoc;
    GLuint textureId;
} UserData;

UserData gUserData;

bool checkGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                LOGE("Could not compile %s shader:\n%s\n",
                     shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                     infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        LOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                LOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}


// ----------------------------


GLuint CreateSimpleTexture2D( )
{
    // Texture object handle
    GLuint textureId =0;

    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
            {
                    255,   0,   0, // Red
                    0, 255,   0, // Green
                    0,   0, 255, // Blue
                    255, 255,   0  // Yellow
            };

    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &textureId );


    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, textureId );

    // Load the texture
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    return textureId;
}

GLuint CreateStorageTexture2D( )
{
    // Texture object handle
    GLuint textureId =0;

    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
            {
                    255,   0,   0, // Red
                    0, 255,   0, // Green
                    0,   0, 255, // Blue
                    255, 255,   0  // Yellow
            };

    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &textureId );


    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, textureId );
//    glTexStorage2D( GL_TEXTURE_2D, 0, GL_RGB, 2, 2 );

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, pixels );
    // Load the texture
//    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    return textureId;

}

void Init ( )
{
    char vShaderStr[] =
            "#version 300 es                            \n"
                    "layout(location = 0) in vec4 a_position;   \n"
                    "layout(location = 1) in vec2 a_texCoord;   \n"
                    "out vec2 v_texCoord;                       \n"
                    "void main()                                \n"
                    "{                                          \n"
                    "   gl_Position = a_position;               \n"
                    "   v_texCoord = a_texCoord;                \n"
                    "}                                          \n";

//   char fShaderStr[] =
//       "#version 300 es                                     \n"
//       "precision mediump float;                            \n"
//       "in vec2 v_texCoord;                                 \n"
//       "layout(location = 0) out vec4 outColor;             \n"
//       "uniform sampler2D s_texture;                        \n"
//       "void main()                                         \n"
//       "{                                                   \n"
//       "  outColor = texture( s_texture, v_texCoord );      \n"
//       "}                                                   \n";

    char fShaderStr[] =
            "#version 300 es                                     \n"
                    "precision mediump float;                            \n"
                    "in vec2 v_texCoord;                                 \n"
                    "layout(location = 0) out vec4 outColor;             \n"
                    "uniform sampler2D s_texture;                        \n"
                    "vec2 brownConradyDistortion(vec2 uv)                \n"
                    "{ \n"
                    "   float demoScale = 1.0; \n"
                    "   uv *= demoScale; \n"
                    "   float barrelDistortion1 = 0.08; \n"
                    "   float barrelDistortion2 = 0.0; \n"
                    "   float r2 = uv.x*uv.x + uv.y*uv.y; \n"
                    "   uv *= 1.0 + barrelDistortion1 * r2 + barrelDistortion2 * r2 * r2; \n"
                    "   return uv;\n"
                    "}\n"
                    "void main()                                         \n"
                    "{                                                   \n"
                    "   vec2 uv = v_texCoord; \n"
                    "   uv = uv * 2.0 - 1.0; \n"
                    "   uv = brownConradyDistortion(uv);\n"
                    "   uv = 0.5 * (uv * 1.0 + 1.0); \n"
                    "   vec4 color;\n"
                    "   if(uv.x>1.0||uv.y>0.93||uv.x<0.0||uv.y<0.07){\n"
                    "       color = vec4(0.0,0.0,0.0,1.0);\n"
                    "    }else{\n"
                    "        color = texture(s_texture, uv);\n"
                    "    }"
                    "   outColor = color;       \n"
                    "  //outColor = texture( s_texture, v_texCoord );      \n"
                    "}                                                   \n";

    // Load the shaders and get a linked program object
    gUserData.programObject = createProgram ( vShaderStr, fShaderStr );

    // Get the sampler location
    gUserData.samplerLoc = glGetUniformLocation ( gUserData.programObject, "s_texture" );

    // Load the texture
//    gUserData.textureId = CreateStorageTexture2D ();
    gUserData.textureId = CreateSimpleTexture2D ();

    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    return;
}


void Draw ()
{
    GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f, 0.0f,  0.0f,
                            -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,
                            1.0f,  0.0f, 0.0f, 1.0f,  1.0f,
                            1.0f,  1.0f, 0.0f,  1.0f,  0.0f
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    // Set the viewport

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( gUserData.programObject );

    // Load the vertex position
    glVertexAttribPointer ( 0, 3, GL_FLOAT,
                            GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
    // Load the texture coordinate
    glVertexAttribPointer ( 1, 2, GL_FLOAT,
                            GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );

    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );

    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, gUserData.textureId );

    // Set the sampler texture unit to 0
    glUniform1i ( gUserData.samplerLoc, 0 );

    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

//    pthread_t seft = pthread_self();
//    char chId[32] = {0};
//    sprintf( chId, "draw thread id=%u", seft );
//    LOGI( chId );

}

#include "esUtil.h"
#include "FileWrapper.h"
typedef struct
{
    GLuint programObject;
    
    GLuint sampluerLoc;
    
    GLuint textureId;
    
    int numIndices;
    GLfloat *vertices;
    GLfloat *normals;
    GLuint *indices;

} UserData;

GLuint CreateSimpleTextureCubemap()
{
    GLuint textureid;
    GLubyte cubePixels[6][3] =
    {
        255,    0,  0,
        0,    255,  0,
        0,      0, 255,
        255, 255, 0,
        255, 0, 255,
        255, 255, 255,
    };
    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[2]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[3]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[4]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[5]);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    return textureid;
    
}
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
//   fragmentShader   = LoadShader ( GL_FRAGMENT_SHADER, GetShaderFile("fragment.glsl"));
    userData->programObject = esLoadProgram(GetShaderFile("vertex.glsl"), GetShaderFile("fragment.glsl"));
    
    userData->sampluerLoc = glGetUniformLocation(userData->programObject,"s_texture");
    
    userData->textureId = CreateSimpleTextureCubemap();
    
    userData->numIndices = esGenSphere(20, 0.75f, &userData->vertices, &userData->normals, NULL, &userData->indices);
    
   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
    UserData *userData = esContext->userData;
    glViewport(0, 0, esContext->width, esContext->height);
    glClear(GL_COLOR_BUFFER_BIT);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glUseProgram(userData->programObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 , userData->vertices);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, userData->normals);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, userData->textureId);
    
    glUniform1i(userData->sampluerLoc, 0);
    glDrawElements(GL_TRIANGLES, userData->numIndices, GL_UNSIGNED_INT, userData->indices);
    
    
}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   glDeleteProgram ( userData->programObject );
}

// mian函数,入口
int triangleMain ( ESContext *esContext )
{

   esContext->userData = malloc ( sizeof ( UserData ) );
    
   esCreateWindow ( esContext, "Hello Triangle", 1334, 750, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}

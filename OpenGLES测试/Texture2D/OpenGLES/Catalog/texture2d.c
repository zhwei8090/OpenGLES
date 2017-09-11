#include "esUtil.h"
#include "FileWrapper.h"
typedef struct
{
    GLuint programObject;
    
    GLuint sampluerLoc;
    
    GLuint textureId;

} UserData;

GLuint CreateSimpleTexture2D()
{
//    GLuint textureId;
//    GLubyte pixels[4*3] =
//    {
//        255,    0,      0,
//        0,      255,    0,
//        0,      0,    255,
//        255,    0,      0
//    };
//    glPixelStorei(GL_UNPACK_ALIGNMENT,1)控制的是所读取数据的对齐方式，默认4字节对齐，即一行的图像数据字节数必须是4的整数倍，即读取数据时，读取4个字节用来渲染一行，之后读取4字节数据用来渲染第二行。对RGB 3字节像素而言，若一行10个像素，即30个字节，在4字节对齐模式下，OpenGL会读取32个字节的数据，若不加注意，会导致glTextImage中致函数的读取越界，从而全面崩溃。
    
    ImageInfo imageInfo = GetImageFile("scanner.png");
    
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
////
//    glGenTextures(1, &textureId);
////
//    glBindTexture(GL_TEXTURE_2D, textureId);
////
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 3, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
////
////    glTexParameterf(<#GLenum target#>, <#GLenum pname#>, <#GLfloat param#>)
////    // Set the filtering mode
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//    
    return imageInfo.name;
    
}
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
//   fragmentShader   = LoadShader ( GL_FRAGMENT_SHADER, GetShaderFile("fragment.glsl"));
    userData->programObject = esLoadProgram(GetShaderFile("vertex.glsl"), GetShaderFile("fragment.glsl"));
    
    userData->sampluerLoc = glGetUniformLocation(userData->programObject,"s_texture");
    
    userData->textureId = CreateSimpleTexture2D();
    
    
   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   
    GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f,  // Position 0
        0.0f,  0.0f,        // TexCoord 0
        -1.0f, -1.0f, 0.0f,  // Position 1
        0.0f,  1.0f,        // TexCoord 1
        1.0f, -1.0f, 0.0f,  // Position 2
        1.0f,  1.0f,        // TexCoord 2
        1.0f,  1.0f, 0.0f,  // Position 3
        1.0f,  0.0f         // TexCoord 3
    };
    GLushort indices[] = { 0, 1, 2, 2, 3, 0 };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), vVertices);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), &vVertices[3]);
//
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
//
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData->textureId);
    
    glUniform1f(userData->sampluerLoc, 0);
    
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    
    
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

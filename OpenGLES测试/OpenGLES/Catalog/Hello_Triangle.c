#include "esUtil.h"
#include "FileWrapper.h"
typedef struct
{
   GLuint programObject;

} UserData;

GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // 创建着色器对象
   shader = glCreateShader ( type );
   // 如果创建失败,则return
   if ( shader == 0 )
   {
      return 0;
   }

   // 加载着色器数据
    glShaderSource(shader, 1, &shaderSrc, NULL);

   // 编译着色器
    glCompileShader(shader);

   // 验证编译状态(这里的意思是验证编译之后有没有错误)
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    // 如果编译失败
   if ( !compiled )
   {
       GLint infoLen = 0;
       glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
       if (infoLen > 1)
       {
           char *infoLog = malloc(sizeof(char) * infoLen);
           glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
           esLogMessage(infoLog);
           free(infoLog);
       }
       glDeleteShader(shader);
       return 0;
   }
   return shader;
}

///
// Initialize the shader and program object
// 初始化shader和 程序
//
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
    
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

   // 加载片段和顶点着色器对象
    
    
   vertexShader     = LoadShader ( GL_VERTEX_SHADER  , GetShaderFile("vertex.glsl"));
   fragmentShader   = LoadShader ( GL_FRAGMENT_SHADER, GetShaderFile("fragment.glsl"));

   // 创建程序对象
    
   programObject = glCreateProgram ( );

   if ( programObject == 0 )
   {
      return 0;
   }
    // 程序对象连接着色器对象
   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // 一旦连接了着色器(并且着色器已经成功编译),我们就最终为链接着色器做好了准备,程序对象链接用glLinkProgram完成
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked )
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char *infoLog = malloc ( sizeof ( char ) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return FALSE;
   }

   // Store the program object
   userData->programObject = programObject;

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   
   GLfloat color[4] = {1.0f,0.0f,0.0f,1.0f};
   GLfloat vertexPos[3*3] = {  0.0f,  0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f
                         };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

    

   // Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vertexPos);
    
   glEnableVertexAttribArray ( 0 );
   glVertexAttrib4fv(1, color);
   glDrawArrays ( GL_TRIANGLES, 0, 3 );
   glDisableVertexAttribArray(0);
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

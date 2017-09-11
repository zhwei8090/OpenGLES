#include "esUtil.h"
#include "FileWrapper.h"
typedef struct
{
    GLuint programObject;
    
    GLuint vboIds[2];
    
    GLuint vaoId;
    
} UserData;

#define Vertex_pos_size 3  // x,y,z
#define Vertex_color_size 4 // r,g,b,a

#define Vertex_pos_indx 0
#define Vertex_color_indx 1

#define Vertex_stride (sizeof(GLfloat) * (Vertex_pos_size + Vertex_color_size))

GLuint vLoadShader ( GLenum type, const char *shaderSrc )
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
int vInit ( ESContext *esContext )
{
    UserData *userData = esContext->userData;
    
    GLuint programObject;
    
    GLfloat vertices[3 * 3] = {
        0.0f,   0.5f,   0.0f,  //pos
        -0.5f,  -0.5f,  0.0f,
        0.5f,   -0.5f,  0.0f,
    };
    GLfloat colors[3*4] = {
        1.0f,   0.0f,   0.0f,   1.0f,
        0.0f,   1.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   1.0f,   1.0f,
    };
    
    // 加载片段和顶点着色器对象
    programObject = esLoadProgram( GetShaderFile("VertexArrayObjectsvertex.glsl"), GetShaderFile("VertexArrayObjectsfragment.glsl"));
    
    if (programObject == 0) {
        return GL_FALSE;
    }
    // Store the program object
    userData->programObject = programObject;
    
    // 生成vbos    userData->vboIds是一个数组
    //n：要产生的VAO对象的数量。
    //arrays：存放产生的VAO对象的名称。
    glGenBuffers(2, userData->vboIds);
    // 绑定vbo 绑定一个缓冲区对象
    // target可能取值是：GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, or GL_PIXEL_UNPACK_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
    // 给vbo 分配数据
    //target可能取值为：GL_ARRAY_BUFFER（表示顶点数据）, GL_ELEMENT_ARRAY_BUFFER（表示索引数据）,GL_PIXEL_PACK_BUFFER（表示从OpenGL获取的的像素数据）, or GL_PIXEL_UNPACK_BUFFER（表示传递给OpenGL的像素数据）.
    //size：缓冲区对象字节数
    //data：指针：指向用于拷贝到缓冲区对象的数据。或者是NULL，表示暂时不分配数据。
    // 注意点: glBindBuffer和glBufferData一般都是成对出现,因为glBufferData每次只会去分配最近bind的数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*9, vertices, GL_STATIC_DRAW);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12, colors, GL_STATIC_DRAW);
    
    
    // 产生VAO
    // n：要产生的VAO对象的数量。
    // arrays：存放产生的VAO对象的名称。
    glGenVertexArrays(1, &userData->vaoId);
    // 绑定VAO
    glBindVertexArray(userData->vaoId);
    
//    void glEnableVertexAttribArray(      GLuint    index);
//    index：指定了需要启用的顶点属性数组的索引(这里指的是Shader里面的 layout(location = index)) 这个index
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
//    给对应的顶点属性数组指定数据：
//    index：要指定数据的顶点属性数组的索引。
//    size：每个顶点属性的数据个数。可能的取值是1、2、3或者4.初始值是4.
//    type：数组中每个数据的类型。可能的取值是：GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, or GL_DOUBLE。初始值是GL_FLOAT。
//    normalized：指定顶点数在被访问的时候是否需要被归一化。
//    注意：如果有个非零的缓冲对象绑定到GL_ARRAY_BUFFER，那么pointer就是对应的缓冲区对象的偏移量。
//    stride：两个连续顶点的属性之间的偏移量。
//    pointer：指向数组中的第一个顶点属性的第一个数据。
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[1]);
    
    glVertexAttribPointer(1, 4, GL_FLOAT,GL_FALSE ,0, 0);
    
//    作用:
//    (1)当传递的参数是非0且是glGenVertexArray()第一次返回的，此时会将传递的参数名字绑定到新创建的顶点数组对象。
//    (2)当传递的参数是已经被绑定过的对象名称，则此时的作用是激活顶点数组对象，后续的相关操作将作用到该顶点数组对象。
//    (3)当传递的参数是0，则此时是解除先前的绑定。
//array: 指明了顶点数组的名字。
    glBindVertexArray(0);
    
    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//


void vDraw ( ESContext *esContext )
{
    UserData *userData = esContext->userData;
    
    
    
    // Set the viewport
    glViewport ( 0, 0, esContext->width, esContext->height );
    
    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );
    
    // Use the program object
    glUseProgram ( userData->programObject );
    
    
    glBindVertexArray(userData->vaoId);
    
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, ( const void * ) 0);
    glDrawArrays(GL_TRIANGLES,0,3);
    glBindVertexArray(0);
    
}

void vShutdown ( ESContext *esContext )
{
    UserData *userData = esContext->userData;
    
    glDeleteProgram ( userData->programObject );
    glDeleteBuffers(2, userData->vboIds);
    glDeleteVertexArrays(1, &userData->vaoId);
    
}

// mian函数,入口
int vertexArrarObjectsMain ( ESContext *esContext )
{
    
    esContext->userData = malloc ( sizeof ( UserData ) );
    
    esCreateWindow ( esContext, "Hello Triangle", 1334, 750, ES_WINDOW_RGB );
    
    if ( !vInit ( esContext ) )
    {
        return GL_FALSE;
    }
    
    esRegisterShutdownFunc ( esContext, vShutdown );
    esRegisterDrawFunc ( esContext, vDraw );
    
    return GL_TRUE;
}

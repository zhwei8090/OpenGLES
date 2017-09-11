//
//  esUtil.h
//  OpenGLES
//
//  Created by zhangwei on 17/4/14.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#ifndef esUtil_h
#define esUtil_h

#include <stdio.h>
#include <stdlib.h>


#ifdef __APPLE__
    #include <OpenGLES/ES3/gl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
//宏定义
#ifdef WIN32
#define ESUTIL_API __cdec1
#define ESCALLBACK __cdec1
#else
#define ESUTIL_API
#define ESCALLBACK
#endif
    
    /// esCreateWindow flag - RGB color buffer 颜色缓冲区应该有R，G，B通道
#define ES_WINDOW_RGB               0
    /// esCreateWindow flag - ALPHA color buffer 指定颜色缓冲区应该有alpha
#define ES_WINDOW_APLHA             1
    /// esCreateWindow flag - depth buffer 深度缓冲区
#define ES_WINDOW_DEPTH             2
    /// esCreateWindow flag - stencil buffer 模板缓冲区
#define ES_WINDOW_STENCIL           3
    /// ecCreateWindow flag - multi-sample buffer   多样本缓冲区
#define ES_WINDOW_MULTISAMPLE       4
//
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct
{
    GLfloat m[4][4];
} ESMatrix;
    
typedef struct ESContext ESContext;

    struct ESContext
    {
        // 平台的数据放在这里
        void *platformData;
        
        // 用户数据
        void *userData;
        
        // 窗口宽度
        GLint width;
        
        // 窗口高度
        GLint height;
        
#ifndef __APPLE__
        
#endif
    
        void ( ESCALLBACK *drawFunc )(ESContext *);
        void ( ESCALLBACK *shutdownFunc ) (ESContext *);
        void ( ESCALLBACK *keyFunc) (ESContext *,unsigned char,int,int);
        void ( ESCALLBACK *updateFunc )(ESContext *,float deltaTime);
    };
    
    
    /**
     创建具有制定参数的窗口
     @param esContext 应用程序的上下文
     @param title 窗口标题栏的名称
     @param width 窗口宽度
     @param height 窗口高度
     @param flags 用于窗口创建标志的Bitfield
     @return 如果窗口创建成功，则//返回GL_TRUE，否则返回GL_FALSE
     */
    GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags );
    
    /**
     注册一个用于渲染每个帧的绘图回调函数
     @param esContext 应用程序的上下文
     @param drawFunc 将用于渲染场景的绘制回调函数
     */
    void ESUTIL_API esRegisterDrawFunc(ESContext *esContext,
                                       void (ESCALLBACK *drawFunc)(ESContext *));
    
    /**
     注册一个在关闭时被调用的回调函数

     @param esContext 应用程序的上下文
     @param shutdownFunc 关闭的回调函数
     */
    void ESUTIL_API esRegisterShutdownFunc(ESContext *esContext,
                                           void (ESCALLBACK *shutdownFunc )(ESContext *));
    
    
    /**
     注册一个更新回调函数,用于在每个时间步上进行更新

     @param esContext 应用程序的上下文
     @param updateFunc 将用于渲染场景的更新回调函数
     */
    void ESUTIL_API esRegisterUpdateFunc(ESContext *esContext,
                                         void (ESCALLBACK *updateFunc )(ESContext *,float));
    
    /**
     注册一个键盘输入处理回调函数

     @param esContext  应用程序的上下文
     @param drawFunc 用于键盘输入的应用程序处理的键回调函数
     */
    void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext,
                                       void ( ESCALLBACK *drawFunc )( ESContext *, unsigned char, int, int ) );
    
    /**
     将消息记录到平台的调试输出

     @param formatStr 格式用于错误日志的字符串
     @param ... <#... description#>
     */
    void ESUTIL_API esLogMessage ( const char *formatStr, ... );
    
    /**
     加载着色器，检查编译错误，打印错误消息以输出日志

     @param type 着色器类型（GL_VERTEX_SHADER 或 GL_FRAGMENT_SHADER）
     @param shaderSrc shaderSrc着色器源字符串
     @return 成功时的一个新的着色器对象，失败时为0
     */
    GLuint ESUTIL_API esLoadShader ( GLenum type, const char *shaderSrc );
    
    /**
     加载顶点和片段着色器，创建程序对象，链接程序。

     @param vertShaderSrc 顶点着色器源代码
     @param fragShaderSrc 着色器源代码
     @return 顶点/片段着色器对链接的新程序对象，失败时为0
     */
    GLuint ESUTIL_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );
    
    
    /**
     为球体生成几何。 为顶点数据和存储分配内存
     注:数组中的结果。 生成TRIANGLE_STRIP的索引列表

     @param numSlices 球体中的切片数
     @param radius 半径
     @param vertices 如果不是NULL，将包含float3法线的数组
     @param normals 如果不是NULL，将包含float2 texCoords的数组
     @param texCoords 如果不是NULL，将包含float2 texCoords的数组
     @param indices 如果不为NULL，将包含三角形条的索引数组
     @return 渲染缓冲区所需的索引数（存储在indices数组中的索引数）
     ///如果不是NULL）作为GL_TRIANGLE_STRIP
     */
    int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals,
                                GLfloat **texCoords, GLuint **indices );
    
    
    /**
     为多维数据集生成几何。 为顶点数据和存储分配内存
           在阵列中的结果。 为三角形生成索引列表
           scale 多维数据集的大小，单位多维数据集使用1.0。
           vertices 如果不为NULL，将包含float3位置的数组
           normals 如果不为NULL，将包含float3法线的数组
           texCoords 如果不为NULL，将包含float2 texCoords的数组
           indices 如果不为NULL，将包含三角形条索引的数组
           return 渲染缓冲区所需的索引数（存储在indices数组中的索引数）
     
     Generates geometry for a cube.  Allocates memory for the vertex data and stores
     the results in the arrays.  Generate index list for a TRIANGLES
     @param scale The size of the cube, use 1.0 for a unit cube.
     @param vertices If not NULL, will contain array of float3 positions
     @param normals If not NULL, will contain array of float3 normals
     @param texCoords If not NULL, will contain array of float2 texCoords
     @param indices If not NULL, will contain the array of indices for the triangle strip
     @return The number of indices required for rendering the buffers (the number of indices stored in the indices array
     */
    int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,
                              GLfloat **texCoords, GLuint **indices );
    
    
    /**
     生成由三角形组成的方形网格。 为顶点数据和存储分配内存
           在阵列中的结果。 生成索引列表为三角形。
            size创建一个按大小的网格（三角形数=（size-1）*（size-1）* 2）
            vertices 如果不为NULL，将包含float3位置的数组
            indices 如果不为NULL，将包含三角形条索引的数组
           渲染缓冲区所需的索引数（存储在indices数组中的索引数）
              如果不是NULL）作为GL_TRIANGLES
     
     Generates a square grid consisting of triangles.  Allocates memory for the vertex data and stores
     the results in the arrays.  Generate index list as TRIANGLES.
     @param size create a grid of size by size (number of triangles = (size-1)*(size-1)*2)
     @param vertices If not NULL, will contain array of float3 positions
     @param indices If not NULL, will contain the array of indices for the triangle strip
     @return The number of indices required for rendering the buffers (the number of indices stored in the indices array
        if it is not NULL ) as a GL_TRIANGLES
     */
    int ESUTIL_API esGenSquareGrid ( int size, GLfloat **vertices, GLuint **indices );
    

    /**
     
     
     @从文件中加载8位，24位或32位TGA图像
     @ ioContext与平台上的IO设施相关的上下文
     @ fileName磁盘上文件的名称
     @ width加载图像的宽度，以像素为单位
     @ height加载图像的高度（以像素为单位）
     @return指向加载图像的指针。 失败时为NULL。
     
    @brief Loads a 8-bit, 24-bit or 32-bit TGA image from a file
    @param ioContext Context related to IO facility on the platform
    @param fileName Name of the file on disk
    @param width Width of loaded image in pixels
    @param height Height of loaded image in pixels
    @return Pointer to loaded image.  NULL on failure.
     */
    char *ESUTIL_API esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height );
    
    
    /**
     由比例矩阵指定的乘法矩阵，并返回结果中的新矩阵
          @ result 指定输入矩阵。 结果返回缩放矩阵。
          @ sx sy，sz分别沿x，y和z轴的比例因子
     
    @brief multiply matrix specified by result with a scaling matrix and return new matrix in result
    @param result Specifies the input matrix.  Scaled matrix is returned in result.
    @param sx sy, sz Scale factors along the x, y and z axes respectively
    */
    void ESUTIL_API esScale ( ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz );
    
    /**
     乘以由结果与转换矩阵指定的矩阵，并返回结果中的新矩阵
          @ result 指定输入矩阵。 翻译矩阵在结果中返回。
          @ tx ty，tz分别沿着x，y和z轴的比例因子
    multiply matrix specified by result with a translation matrix and return new matrix in result
    @param result Specifies the input matrix.  Translated matrix is returned in result.
    @param tx ty, tz Scale factors along the x, y and z axes respectively
    */
    void ESUTIL_API esTranslate ( ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz );
    
    /**
     乘以由结果与旋转矩阵指定的矩阵，并返回结果中的新矩阵
          @ result 指定输入矩阵。 结果返回旋转矩阵。
          @ angle 指定旋转角度，以度为单位。
          @ x y，z分别指定向量的x，y和z坐标
    multiply matrix specified by result with a rotation matrix and return new matrix in result
    @param result Specifies the input matrix.  Rotated matrix is returned in result.
    @param angle Specifies the angle of rotation, in degrees.
    @param x y, z Specify the x, y and z coordinates of a vector, respectively
    */
    void ESUTIL_API esRotate ( ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
    
    /**
     乘以矩阵指定的矩阵，并返回结果中的新矩阵
          @ result指定输入矩阵。 结果返回新的矩阵。
          @左侧左侧和右侧垂直剪切平面的坐标
          @ bottom top底部和顶部水平裁剪平面的坐标
          @ nearZ farZ距离近和深度剪切平面的距离。 两个距离必须为正。
     
    multiply matrix specified by result with a perspective matrix and return new matrix in result
    @param result Specifies the input matrix.  new matrix is returned in result.
    @param left right Coordinates for the left and right vertical clipping planes
    @param bottom top Coordinates for the bottom and top horizontal clipping planes
    @param nearZ farZ Distances to the near and far depth clipping planes.  Both distances must be positive.
    */
    void ESUTIL_API esFrustum ( ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ );
    
    /**
     乘以矩阵指定的矩阵，并返回结果中的新矩阵
          @ result 指定输入矩阵。 结果返回新的矩阵。
          @ fovy 视角y角度（以度为单位）
          @ aspect 屏幕纵横比
          @ nearZ 近平面距离
          @ farZ 远平面距离
     
    multiply matrix specified by result with a perspective matrix and return new matrix in result
    @param result Specifies the input matrix.  new matrix is returned in result.
    @param fovy Field of view y angle in degrees
    @param aspect Aspect ratio of screen
    @param nearZ Near plane distance
    @param farZ Far plane distance
    */
    void ESUTIL_API esPerspective ( ESMatrix *result, float fovy, float aspect, float nearZ, float farZ );
    
    /**
     乘以矩阵指定的矩阵，并返回结果中的新矩阵
          @ result指定输入矩阵。 结果返回新的矩阵。
          @ 左侧左侧和右侧垂直剪切平面的坐标
          @ bottom top底部和顶部水平裁剪平面的坐标
          @ nearZ farZ距离近和深度剪切平面的距离。 如果平面在观察者后面，则这些值为负值
     
    multiply matrix specified by result with a perspective matrix and return new matrix in result
    @param result Specifies the input matrix.  new matrix is returned in result.
    @param left right Coordinates for the left and right vertical clipping planes
    @param bottom top Coordinates for the bottom and top horizontal clipping planes
    @param nearZ farZ Distances to the near and far depth clipping planes.  These values are negative if plane is behind the viewer
    */
    void ESUTIL_API esOrtho ( ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ );
    
    /**
     执行以下操作 - result matrix = srcA matrix * srcB matrix
          @ result返回乘法矩阵
          @ srcA srcB要乘以的输入矩阵
     perform the following operation - result matrix = srcA matrix * srcB matrix
    @param result Returns multiplied matrix
    @param srcA srcB Input matrices to be multiplied
    */
    void ESUTIL_API esMatrixMultiply ( ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB );
    
    /**
     返回一个单位矩阵
     
     return an indentity matrix
    @param result returns identity matrix
    */
    void ESUTIL_API esMatrixLoadIdentity ( ESMatrix *result );
    
    /**
    Generate a transformation matrix from eye position, look at and up vectors
    @param result Returns transformation matrix
    @param posX posY, posZ           eye position
    @param lookAtX lookAtY, lookAtZ  look at vector
    @param upX upY, upZ              up vector
    */
    void ESUTIL_API
    esMatrixLookAt ( ESMatrix *result,
                    float posX,    float posY,    float posZ,
                    float lookAtX, float lookAtY, float lookAtZ,
                    float upX,     float upY,     float upZ );
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* esUtil_h */

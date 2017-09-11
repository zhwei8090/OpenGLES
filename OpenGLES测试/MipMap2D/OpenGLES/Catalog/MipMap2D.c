#include "esUtil.h"
#include "FileWrapper.h"


typedef struct
{
    GLuint programObject;
    
    GLuint samplerLoc;
    
    GLuint offsetLoc;
    
    GLuint textureid;

} UserData;
GLubyte *GenCheckImage(int width,int height,int checkSize)
{
    int x,y;
    GLubyte *pixels = malloc(width * height * 3);
    if (pixels == NULL) {
        return NULL;
    }
    for (  y = 0; y < height ; y++)
    {
        for ( x = 0; x < width; x++)
        {
            GLubyte rColor = 0;
            GLubyte bColor = 0;
            if (( x / checkSize ) % 2 == 0)
            {
                rColor = 255 * (( y / checkSize ) % 2);
                bColor = 255 * ( 1 - ( (y/checkSize) % 2));
            }
            else
            {
                bColor = 255 * (( y / checkSize ) % 2);
                rColor = 255 * ( 1 - ( (y/checkSize) % 2));
            }
            pixels[(y * width + x) * 3] = rColor;
            pixels[(y * width + x) * 3 + 1] = 0;
            pixels[(y * width + x) * 3 + 2] = bColor;
        }
    }
    return pixels;
}
GLboolean GenMipMap2D(GLubyte *src,GLubyte **dst,int srcWidth,int srcHeight,int *dstWidth,int *dstHeight)
{
    int x,y;
    int texelSize = 3;
    *dstWidth = srcWidth / 2;
    if (*dstWidth <= 0) {
        *dstWidth = 1;
    }
    *dstHeight = srcHeight / 2;
    if (*dstHeight <= 0) {
        *dstHeight = 1;
    }
    *dst = malloc(sizeof(GLubyte) * texelSize * (*dstWidth) * (*dstHeight));
    if(*dst == NULL ){
        return GL_FALSE;
    }
    for ( y = 0; y < *dstHeight ; y ++) {
        for ( x = 0; x < *dstWidth ; x++)
        {
            int srcIndex[4];
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            int sample;
            srcIndex[0] = (((y * 2) * srcWidth) + (x * 2)) * texelSize;
            srcIndex[1] = (((y * 2) * srcWidth) + (x * 2 + 1)) * texelSize;
            srcIndex[2] = ((((y * 2) + 1 )* srcWidth) + (x * 2)) * texelSize;
            srcIndex[3] = ((((y * 2) + 1 )* srcWidth) + (x * 2 + 1)) * texelSize;
            
            for(sample = 0 ; sample < 4; sample ++)
            {
                r += src[srcIndex[sample]];
                g += src[srcIndex[sample] + 1];
                b += src[srcIndex[sample] + 2];
            }
            
            r /= 4.0;
            g /= 4.0;
            b /= 4.0;
            
            (*dst)[( y * (*dstWidth) + x) * texelSize] = (GLubyte)(r);
            (*dst)[( y * (*dstWidth) + x) * texelSize + 1] = (GLubyte)(g);
            (*dst)[( y * (*dstWidth) + x) * texelSize + 2] = (GLubyte)(b);
        }
    }
    return GL_TRUE;
}
GLuint CreateMipMappedTexture2D()
{
    GLuint textureId;
    int width = 256, height = 256;
    int level;
    GLubyte *pixels;
    GLubyte *prevImage;
    GLubyte *newImage = NULL;
    
    pixels = GenCheckImage(width,height, 50);
    if(pixels == NULL){
        return 0;
    }
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
//    level = 1;
//    prevImage = &pixels[0];
//    while (width > 1 && height > 1)
//    {
//        int newWidth,newHeight;
//        GenMipMap2D(prevImage, &newImage, width, height, &newWidth, &newHeight);
//        glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, newImage);
//        free(prevImage);
//        prevImage = newImage;
//        level ++ ;
//        width = newWidth;
//        height = newHeight;
//    }
//    free( newImage );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return textureId;
    
}

int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

    userData->programObject = esLoadProgram(GetShaderFile("vertex.glsl"), GetShaderFile("fragment.glsl"));
    userData->samplerLoc = glGetUniformLocation(userData->programObject, "s_texture");
    userData->offsetLoc = glGetUniformLocation(userData->programObject, "u_offset");
    
    userData->textureid = CreateMipMappedTexture2D();
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   
    GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f, 1.5f,  // Position 0
        0.0f,  0.0f,              // TexCoord 0
        -0.5f, -0.5f, 0.0f, 0.75f, // Position 1
        0.0f,  1.0f,              // TexCoord 1
        0.5f, -0.5f, 0.0f, 0.75f, // Position 2
        1.0f,  1.0f,              // TexCoord 2
        0.5f,  0.5f, 0.0f, 1.5f,  // Position 3
        1.0f,  0.0f               // TexCoord 3
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
//    GLushort indices[] = { 0, 1, 2, 2, 3, 0 };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( userData->programObject );
    
    // Load the vertex position
    glVertexAttribPointer ( 0, 4, GL_FLOAT,
                           GL_FALSE, 6 * sizeof ( GLfloat ), vVertices );
    // Load the texture coordinate
    glVertexAttribPointer ( 1, 2, GL_FLOAT,
                           GL_FALSE, 6 * sizeof ( GLfloat ), &vVertices[4] );
    
    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );
    
    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, userData->textureid );
    
    // Set the sampler texture unit to 0
    glUniform1i ( userData->samplerLoc, 0 );
    
    // Draw quad with nearest sampling
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glUniform1f ( userData->offsetLoc, -0.6f );
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
    
    // Draw quad with trilinear filtering
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glUniform1f ( userData->offsetLoc, 0.6f );
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
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

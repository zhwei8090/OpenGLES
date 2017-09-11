
//@private
//GLuint                      name;
//GLenum                      target;
//GLuint                      width;
//GLuint                      height;
//GLuint                      depth;
//GLKTextureInfoAlphaState    alphaState;
//GLKTextureInfoOrigin        textureOrigin;
//BOOL                        containsMipmaps;
//GLuint                      mimapLevelCount;
//GLuint                      arrayLength;
//}

#include <OpenGLES/ES3/gl.h>

//#import <GLKit/GLKit.h>


typedef struct
{
    GLuint name;
    GLuint target;
    GLuint width;
    GLuint height;
    GLuint depth;
    GLuint arrayLength;
    
} ImageInfo;

const char *GetBundleFileName ( const char *fileName );

const char *GetShaderFile(const char *filePath);

const ImageInfo GetImageFile(const char *fileName);

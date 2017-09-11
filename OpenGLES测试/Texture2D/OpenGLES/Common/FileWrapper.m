
#import "FileWrapper.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

const char *GetBundleFileName( const char *fileName )
{
    NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
    NSString* baseName = [fileNameNS stringByDeletingPathExtension];
    NSString* extension = [fileNameNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
    
    
    fileName = [path UTF8String];
    return fileName;
}
const char *GetShaderFile(const char *fileName)
{
    NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
    NSString* baseName = [fileNameNS stringByDeletingPathExtension];
    NSString* extension = [fileNameNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
    
    
    NSString *shader = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    
    return [shader UTF8String];
}
const ImageInfo GetImageFile(const char *fileName)
{
    NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
    GLKTextureInfo *diffuseMap = [GLKTextureLoader textureWithCGImage:[UIImage imageNamed:fileNameNS].CGImage options:nil error:nil];
    ImageInfo imgInfo;
    imgInfo.name    =   diffuseMap.name;
    imgInfo.width   =   diffuseMap.width;
    imgInfo.height  =   diffuseMap.height;
    imgInfo.target  =   diffuseMap.target;
    imgInfo.depth   =   diffuseMap.depth;
    imgInfo.arrayLength = diffuseMap.arrayLength;
    
    return imgInfo;
}

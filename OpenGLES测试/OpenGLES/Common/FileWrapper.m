
#import "FileWrapper.h"
#import <Foundation/Foundation.h>


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

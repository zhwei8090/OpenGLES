//
//  CameraView.m
//  camera Draw
//
//  Created by zhangwei on 17/9/8.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import "CameraView.h"
#import "ZWContext.h"

#define ZWWidth [UIScreen mainScreen].bounds.size.width
#define ZWHeight  [UIScreen mainScreen].bounds.size.height
@interface CameraView() <GLKViewDelegate>
{
    EAGLContext *_eaglContext;
    GLuint _texHandler;
    GLuint vbo,_vbohandle;
    int _luminanceTextureAtt;
    int _width,_height;
    unsigned char *_pixels;

}
@property (nonatomic,strong)CADisplayLink   *displayLink;
@property (nonatomic,strong)ZWContext       *zwContext;
@property (nonatomic,assign) BOOL  isFirstn;
@end

@implementation CameraView


#pragma mark -GLKViewDelegate
- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // 创建context,并设置当前context
        _eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        [EAGLContext setCurrentContext:_eaglContext];
        self.context = _eaglContext;
        self.drawableMultisample = GLKViewDrawableMultisample4X;
        self.delegate = self;
        self.layer.backgroundColor = [UIColor clearColor].CGColor;
        self.layer.opaque = NO;
        NSString *verteShader = [[NSBundle mainBundle] pathForResource:@"vertex" ofType:@"glsl"];
        NSString *fragmentShader = [[NSBundle mainBundle] pathForResource:@"fragment" ofType:@"glsl"];
        
        NSString *vertexShaderContent = [NSString stringWithContentsOfFile:verteShader encoding:NSUTF8StringEncoding error:nil];
        NSString *fragmentShaderContent = [NSString stringWithContentsOfFile:fragmentShader encoding:NSUTF8StringEncoding error:nil];
        
        self.zwContext = [[ZWContext alloc] initWithVertexShader:vertexShaderContent fragmentShader:fragmentShaderContent];
        [self initTexture];
    }
    return self;
}
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if(_width == 0)return;
    [EAGLContext setCurrentContext:_eaglContext];
    glClearColor(0.0, 0.0, 0.0, 0.0);
    if (_isFirstn) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width , _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
        _isFirstn = NO;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_BGRA, GL_UNSIGNED_BYTE, _pixels);
    float camRad = (float)_height/_width, scrRad = (float)ZWWidth/ZWHeight;
    float xfactor=1,yfactor=1;
    if(camRad > scrRad)
    {
        xfactor = camRad / scrRad;
    }
    else
    {
        yfactor = scrRad / camRad;
    }
    
    float x1 = -xfactor,y1 = -yfactor,x2 = xfactor,y2 = yfactor;
    
    float vertices[] =
    {
        x1, y2,   0, 1,
        x1, y1,   0, 0,
        x2, y2,   1, 1,
        x2, y1,   1, 0
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 4, vertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    GLint oldTexture,oldProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgram);
    glUseProgram(self.zwContext.program);
    _luminanceTextureAtt = glGetUniformLocation(self.zwContext.program, "luminanceTexture");
    glActiveTexture(GL_TEXTURE0);
    
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldTexture);
    glBindTexture(GL_TEXTURE_2D, _texHandler);
    glUniform1i(_luminanceTextureAtt, 0);
    
    glBindVertexArrayOES(_vbohandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArrayOES(0);

    
}
- (void)updateBuffer:(unsigned char *)pixels width:(int)width height:(int)heigth
{
    _pixels = pixels;
    _width = width;
    _height = heigth;
    [self display];
}
- (void)initTexture
{
    glGenTextures(1, &_texHandler);
    glBindTexture(GL_TEXTURE_2D, _texHandler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
    
    
    float vertuces[] =
    {
        -1.0f,  1.0f,   0,  1,
        -1.0f,  -1.0f,  0,  0,
        1.0f,   1.0f,   1,  1,
        1.0f,   -1.0f,  1,  0
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 4, vertuces ,GL_STATIC_DRAW);
    

    
    
    glGenVertexArraysOES(1, &_vbohandle);
    glBindVertexArrayOES(_vbohandle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    GLint posLoc ,texcoordLoc;
    posLoc = glGetAttribLocation(self.zwContext.program, "position");
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(posLoc);
    texcoordLoc = glGetAttribLocation(self.zwContext.program, "inputTextureCoordinate");
    glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(texcoordLoc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    self.isFirstn = YES;
}
@end

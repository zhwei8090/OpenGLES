//
//  ZWBaseController.m
//  OpenGLES
//
//  Created by zhangwei on 17/4/14.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import "ZWBaseController.h"
#import "esUtil.h"

extern void esMain(ESContext *exContext);

@interface ZWBaseController ()
{
    ESContext _esContext;
}
@property (strong,nonatomic) EAGLContext *context; // GL上下文
@property (strong,nonatomic) GLKBaseEffect *effect;

// 设置GL
- (void)setupGL;
// 销毁GL
- (void)tearDownGL;
@end

@implementation ZWBaseController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!_context)
    {
        NSLog(@"Failed to create ES Context");
    }
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}
- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    memset(&_esContext, 0, sizeof(_esContext));
    
    esMain(&_esContext);
}
- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    if (_esContext.shutdownFunc)
    {
        _esContext.shutdownFunc( &_esContext);
    }
}
- (void)update
{
    if(_esContext.updateFunc){
        _esContext.updateFunc(&_esContext,self.timeSinceLastUpdate);
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
     _esContext.width  = (int)view.drawableWidth;
     _esContext.height = (int)view.drawableHeight;
    if (_esContext.drawFunc)
    {
        _esContext.drawFunc(&_esContext);
    }
}
@end

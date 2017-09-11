//
//  ZWBaseController.m
//  OpenGLES
//
//  Created by zhangwei on 17/4/14.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import "ZWBaseController.h"
#import "esUtil.h"

extern void triangleMain(ESContext *exContext);
extern void vertexArrarObjectsMain(ESContext *exContext);
@interface ZWBaseController ()
{
    ESContext _esContext;
    NSInteger _index;
}
@property (strong,nonatomic) EAGLContext *context; // GL上下文


@end

@implementation ZWBaseController
- (instancetype)initWithIndex:(NSInteger)index
{
    if(self = [super init])
    {
        _index = index;
    }
    return self;
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.navigationController setNavigationBarHidden:YES animated:YES];
}
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
    
    triangleMain(&_esContext);
    
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

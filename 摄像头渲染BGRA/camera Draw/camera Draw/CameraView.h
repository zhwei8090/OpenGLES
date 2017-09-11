//
//  CameraView.h
//  camera Draw
//
//  Created by zhangwei on 17/9/8.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <OpenGLES/ES2/gl.h>
@interface CameraView : GLKView

- (void)updateBuffer:(unsigned char *)pixels width:(int)width height:(int)heigth;
@end

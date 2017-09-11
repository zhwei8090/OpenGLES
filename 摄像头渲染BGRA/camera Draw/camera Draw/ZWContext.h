//
//  ZWContext.h
//  camera Draw
//
//  Created by zhangwei on 17/9/8.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <OpenGLES/ES2/glext.h>

@interface ZWContext : NSObject

@property (nonatomic,assign) GLuint program;

- (instancetype)initWithVertexShader:(NSString *)vertexShader fragmentShader:(NSString *)fragmentShader;


@end

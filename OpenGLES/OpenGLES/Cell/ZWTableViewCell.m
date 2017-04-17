//
//  ZWTableViewCell.m
//  OpenGLES
//
//  Created by zhangwei on 17/4/13.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import "ZWTableViewCell.h"


@implementation ZWTableViewCell


- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
    self.titleLabel.text = @"你好";
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end

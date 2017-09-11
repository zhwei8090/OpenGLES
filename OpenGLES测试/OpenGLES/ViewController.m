//
//  ViewController.m
//  OpenGLES
//
//  Created by zhangwei on 17/4/13.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import "ViewController.h"
#import "ZWTableViewCell.h"
#import "ZWBaseController.h"
@interface ViewController ()<UITableViewDelegate,UITableViewDataSource>

@property (nonatomic,strong) UITableView *tableView;
@property (nonatomic,strong) NSArray *datas;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"OpenGLES";
    [self.view addSubview:self.tableView];
    
    
}

#pragma mark -TableViewDataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.datas.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ZWTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"CellID" forIndexPath:indexPath];
    ;
    cell.titleLabel.text = [NSString stringWithFormat:@"第%ld节",indexPath.row + 1];
    cell.contentLabel.text = self.datas[indexPath.row];
    
    return cell;
}

#pragma mark -TableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    ZWBaseController *base = [[ZWBaseController alloc] initWithIndex:indexPath.row];
    [self.navigationController pushViewController:base animated:YES];
    
//    switch (indexPath.row) {
//        case 0:
//        {
//            
//        }
//        break;
//            
//        default:
//            break;
//    }
}

#pragma mark -GetSet

- (UITableView *)tableView
{
    if (!_tableView) {
        _tableView = [[UITableView alloc] initWithFrame:self.view.frame style:UITableViewStylePlain];
        [_tableView registerNib:[UINib nibWithNibName:@"ZWTableViewCell" bundle:nil] forCellReuseIdentifier:@"CellID"];
        [_tableView setSeparatorColor:[UIColor clearColor]];
        _tableView.delegate = self;
        _tableView.dataSource = self;
        
    }
    return _tableView;
}
- (NSArray *)datas
{
    if (!_datas) {
        _datas = [NSArray arrayWithObjects:@"画一个三角形",@"顶点缓冲区对象",nil];
    }
    return _datas;
}
@end

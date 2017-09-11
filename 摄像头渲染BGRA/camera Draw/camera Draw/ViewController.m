//
//  ViewController.m
//  camera Draw
//
//  Created by zhangwei on 17/9/8.
//  Copyright © 2017年 zhangwei. All rights reserved.
//

#import "ViewController.h"
#import <AVFoundation/AVFoundation.h>
#import "CameraView.h"
@interface ViewController ()<AVCaptureVideoDataOutputSampleBufferDelegate>
{
    AVCaptureSession *_session;
    CameraView *_cameraView;
    void *_cameraBuffer;
}
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self initCamera];
}


- (void)initCamera
{
    [self setupCaptureSession];
   _cameraView =  [[CameraView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:_cameraView];
}
- (void)setupCaptureSession
{
    
    _session = [[AVCaptureSession alloc] init];
    _session.sessionPreset = AVCaptureSessionPreset1280x720;
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    AVCaptureDevice *td = nil;
    for (AVCaptureDevice *device in devices) {
        if (device.position == AVCaptureDevicePositionBack) {
            td = device;
        }
    }
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:td error:nil];
    if (!input) {
        NSLog(@"error");
    }
    [_session addInput:input];
    
    AVCaptureVideoDataOutput *output = [[AVCaptureVideoDataOutput alloc] init];
    
    [_session addOutput:output];
//    dispatch_queue_t queue = dispatch_queue_create("com.zw.nb", NULL);
//    dispatch_queue_t queues = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    [output setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    
    output.videoSettings = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey];

    [_session startRunning];
    
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    NSLog(@"%@",[NSThread currentThread]);
    CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    size_t width  =  CVPixelBufferGetWidth(pixelBuffer);
    size_t height = CVPixelBufferGetHeight(pixelBuffer);
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    //    if (self.glesView) [self.glesView render:pixelBuffer];
    
    OSType format = CVPixelBufferGetPixelFormatType(pixelBuffer);
    if (format == kCVPixelFormatType_32BGRA) {
        void *buffer = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
        if(_cameraBuffer == nil){
            _cameraBuffer = malloc(width * height * 4);
        }
        memcpy(_cameraBuffer, buffer, width * height * 4);
        [_cameraView updateBuffer:(uint8_t*)_cameraBuffer width:(int)width height:(int)height];
    }
    CVPixelBufferUnlockBaseAddress(pixelBuffer,0);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end

//
//  VideoRenderLayer.h
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <GLKit/GLKit.h>
#import <QuartzCore/QuartzCore.h>

@interface VideoRenderLayer : CAEAGLLayer
- (id)initWithFrame:(CGRect)frame;
- (void)setRenderFrame:(CGRect)frame;
- (void)initOpenGLES;
- (void)resetRenderBuffer;
- (void)drawRGBBuffer:(uint8_t*)rgbBuffer;
- (void)setDrawRotation:(CGFloat)rotation;
- (void)drawPixelBuffer:(CVPixelBufferRef)pixelBuffer width:(CGFloat)width height:(CGFloat)height;

@end

//
//  VideoWindow.h
//  VideoPlayer
//
//  Created by 朱爱俊 on 2017/9/7.
//  Copyright © 2017年 朱爱俊. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <QuartzCore/QuartzCore.h>

typedef enum
{
    VideoRotation_None      = 0,
    VideoRotation_Left_90   = 1,
    VideoRotation_Left_180  = 2,
    VideoRotation_Left_270  = 3,
    VideoRotation_Right_90  = 4,
    VideoRotation_Right_180 = 5,
    VideoRotation_Right_270 = 6,
} VideoRotation;

@interface VideoWindow : UIView
- (id)initWithFrame:(CGRect)frame;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (void)clearWindow;
- (void)setRotation:(VideoRotation)rotation;
- (void)drawPixelBuffer:(CVPixelBufferRef)pixelBuffer width:(CGFloat)width height:(CGFloat)height;
- (void)drawYUV:(void *)YBuf U:(void *)UBuf V:(void *)VBuf strideY:(CGFloat)strideY strideUV:(CGFloat)strideUV frameWidth:(CGFloat)width frameHeight:(CGFloat)height;
- (void)setRenderParam:(CGFloat)videoWidth videoHeight:(CGFloat)videoHeight
       userWindowWidth:(CGFloat)userWndWidth userWindowHeight:(CGFloat)userWndHeight;
@end

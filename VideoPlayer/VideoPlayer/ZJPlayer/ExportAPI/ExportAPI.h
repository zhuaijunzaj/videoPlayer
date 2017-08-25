//
//  ExportAPI.h
//  VideoPlayer
//
//  Created by Kattern on 2017/8/24.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@interface MediaInfo : NSObject
@property (nonatomic, assign) BOOL seekable;
@property (nonatomic, assign) BOOL hasAudio;
@property (nonatomic, assign) BOOL hasVideo;
@property (nonatomic, assign) NSUInteger duration;
@property (nonatomic, assign) NSUInteger videoWidth;
@property (nonatomic, assign) NSUInteger videoHeight;
@property (nonatomic, assign) NSInteger videoRotation;
@property (nonatomic, assign) NSUInteger audioChannels;
@property (nonatomic, assign) NSUInteger audioSampleRate;
@end

@interface VideoWindow :UIView
- (id)initWithFrame:(CGRect)frame;
- (id)initWithCoder:(NSCoder*)aDecoder;
@end
@interface ExportAPI : NSObject
+(instancetype)sharedInstance;
-(void)openMedia:(NSString*)path;
@end

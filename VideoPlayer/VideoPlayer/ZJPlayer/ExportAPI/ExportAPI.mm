//
//  ExportAPI.m
//  VideoPlayer
//
//  Created by Kattern on 2017/8/24.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#import "ExportAPI.h"
#import "Player.hpp"



@interface ExportAPI ()
{
    Player *player;
}
@end
@implementation ExportAPI

+(instancetype)sharedInstance
{
    static ExportAPI *api = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        api = [[ExportAPI alloc] init];
    });
    return api;
}
-(id)init
{
    self = [super init];
    if (self){
        player = Player::sharedInstance();
    }
    return self;
}
-(void)openMedia:(NSString*)path width:(NSInteger)width height:(NSInteger)height window:(void*)window
{
    player->openMedia(window,[path UTF8String],(int)width,(int)height);
}

-(void)play
{
    player->play();
}
@end

//
//  ZJType.h
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef ZJType_h
#define ZJType_h

#ifdef __cplusplus
extern "C"{
#endif

#ifndef NULL
#define NULL         0
#endif
    
    typedef wchar_t                 ZJ_WCHAR;
    
    typedef unsigned char           ZJ_U8;
    typedef unsigned short          ZJ_U16;
    typedef unsigned int            ZJ_U32;
    typedef unsigned long long      ZJ_U64;
    
    
#define ZJ_U8_MAX                    0xff
#define ZJ_U16_MAX                   0xffff
#define ZJ_U32_MAX                   0xffffffff
#define ZJ_U64_MAX                   0xfffffffffffffff
    
#ifdef __cplusplus
}
#endif
#endif /* ZJType_h */

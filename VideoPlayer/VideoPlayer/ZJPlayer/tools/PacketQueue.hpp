//
//  PacketQueue.hpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#ifndef PacketQueue_hpp
#define PacketQueue_hpp

#include <stdio.h>
#include "ZJType.h"

#define MAX_QUEUE_SIZE 512

template <typename T>
class PacketQueue {
public:
    PacketQueue(ZJ_U32 size = MAX_QUEUE_SIZE);
    ~PacketQueue();
    
    int Push(T data);
    int Pop(T* data);
    
    int Front(T* data) const;
    int Tail(T* data) const;
    
    bool isFull() const;
    bool isEmpty() const;
    
    int Count() const;
    int size()  const;
    void clean();
    
private:
    ZJ_U32 nSize;
    ZJ_U32 nHead;
    ZJ_U32 nTail;
    ZJ_U32 nItemsCount;
    T *    nData;
};

#endif /* PacketQueue_hpp */

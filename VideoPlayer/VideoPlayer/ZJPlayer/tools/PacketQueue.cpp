//
//  PacketQueue.cpp
//  VideoPlayer
//
//  Created by Kattern on 2017/8/23.
//  Copyright © 2017年 Kattern. All rights reserved.
//

#include "PacketQueue.hpp"

template<typename T>
PacketQueue<T>::PacketQueue(ZJ_U32 size):nSize(0),nHead(0),nTail(0),nItemsCount(0),nData(NULL)
{
    nData = new T[size];
}

template<typename T>
PacketQueue<T>::~PacketQueue()
{
    if (nData) delete[] nData;
}

template<typename T>
int PacketQueue<T>::Push(T data)
{
    if (nItemsCount >= nSize || nData == NULL)
        return -1;
    
    int pos = (nTail % nSize);
    nData[pos] = data;
    nTail++;
    nItemsCount++;
    return 0;
}

template<typename T>
int PacketQueue<T>::Pop(T *data)
{
    if (nItemsCount == 0 || nData == NULL )
        return -1;
    if (data == NULL)
        return -1;
    int pos = (nHead % nSize);
    *data = data[pos];
    nHead++;
    nItemsCount--;
    return 0;
}

template<typename T>
int PacketQueue<T>::Front(T *data) const
{
    if (nItemsCount == 0 || nData == NULL )
        return -1;
    if (data == NULL)
        return -1;
    int pos = (nHead % nSize);
    *data = data[pos];
    return 0;
}

template<typename T>
int PacketQueue<T>::Tail(T *data) const
{
    if (nItemsCount == 0 || nData == NULL )
        return -1;
    if (data == NULL)
        return -1;
    int pos = ((nTail-1) % nSize);
    *data = data[pos];
    return 0;
}

template<typename T>
bool PacketQueue<T>::isFull() const
{
    return nItemsCount >= nSize;
}

template<typename T>
bool PacketQueue<T>::isEmpty() const
{
    return 0 == nItemsCount;
}

template<typename T>
int PacketQueue<T>::Count() const
{
    return nItemsCount;
}

template<typename T>
int PacketQueue<T>::size() const
{
    return nSize;
}

template<typename T>
void PacketQueue<T>::clean()
{
    nHead = 0;
    nTail = 0;
    nItemsCount = 0;
}
































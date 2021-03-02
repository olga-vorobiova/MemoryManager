//
//  memoryManager.cpp
//
//  Created by Olga Vorobiova on 26.02.2021.
//  Copyright © 2021 Olga Vorobiova. All rights reserved.
//

#include <iostream>
#include "memoryManager.hpp"
#include "memoryManagerException.hpp"

constexpr static byte byteMask[] = {1, 2, 4, 8, 16, 32, 64, 128};

MemoryManager::MemoryManager(void* ptr, size_t memorySize, size_t blockSize) :
m_blockSize(blockSize)
{
    if((m_blockSize > memorySize) || (blockSize == 0))
        throw MemoryManagerException("MemoryManager: invalid block size");
    
    m_memoryPtr = static_cast<byte*>(ptr);
    m_infoSize = 0;
    
    if(m_blockSize == memorySize)
    {
        m_blocksCount = 1;
        m_freeBlocksCount = 1;
    }
    else 
    {
        m_blocksCount = memorySize / m_blockSize;
        
        while(true)
        {
            m_infoSize = ((memorySize - m_infoSize) / m_blockSize) / 8;
            m_infoSize += (((memorySize - m_infoSize) / m_blockSize) % 8 == 0) ? 0 : 1;
            size_t tempBlocksCount = (memorySize - m_infoSize) / m_blockSize;

            if((m_blocksCount - tempBlocksCount) < 8 || (tempBlocksCount - m_blocksCount) < 8)
            {
                m_blocksCount = tempBlocksCount;
                break;
            }
            m_blocksCount = tempBlocksCount;
        }

        m_freeBlocksCount = m_blocksCount;
        
        for(size_t index = 0; index < m_infoSize; ++index)
        {
            byte &infoByte = m_memoryPtr[index];
            infoByte = 0;
        }
    }
}

void* MemoryManager::allocate()
{
    size_t shift = m_infoSize + findFreeBlock() * m_blockSize;
    void* p = m_memoryPtr + shift;
    return p;
}

void MemoryManager::free(void* p)
{
    byte* ptr = static_cast<byte*>(p);
    if ((m_memoryPtr > ptr) ||
        ((m_memoryPtr + m_infoSize + m_blocksCount * m_blockSize) <= ptr))
        throw MemoryManagerException("MemoryManager::free: invalid pointer");
        
    size_t blockIndex = static_cast<byte*>(p) - m_memoryPtr;
    blockIndex = (blockIndex - m_infoSize) / m_blockSize;
    static const std::string multipleFreeExceptionMessage = "MemoryManager::free: call free multiple times for the same pointer";
    
    if(m_infoSize == 0)
    {
        if(m_freeBlocksCount == 0)
            ++m_freeBlocksCount;
        else
            throw MemoryManagerException(multipleFreeExceptionMessage);
    }
    else
    {
        size_t byteIndex = blockIndex / 8;
        size_t bitIndex = blockIndex % 8;
        byte& infoByte = m_memoryPtr[byteIndex];
        byte mask = byteMask[bitIndex];

        if(infoByte & mask)
        {
            infoByte ^= mask;
            ++m_freeBlocksCount;
        }
        else
            throw MemoryManagerException(multipleFreeExceptionMessage);
    }
}

size_t MemoryManager::findFreeBlock()
{
    if(!m_freeBlocksCount)
        throw MemoryManagerException("MemoryManager::allocate: no free memory");
    
    for(size_t byteIndex = 0; byteIndex < m_infoSize; ++byteIndex)
    {
        byte& infoByte = m_memoryPtr[byteIndex];
        if(infoByte == 0xFF)
            continue;

        size_t bitIndex = 0;
        byte mask = 1;
        
        for(; bitIndex < 8; ++bitIndex)
        {
            if((~infoByte & mask) == mask)
            {
                infoByte |= mask;
                --m_freeBlocksCount;
                return bitIndex + byteIndex * 8;
            }
            mask <<= 1;
        }
    }

    --m_freeBlocksCount;
    return 0;
}


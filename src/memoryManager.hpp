//
//  memoryManager.hpp
//
//  Created by Olga Vorobiova on 26.02.2021.
//  Copyright © 2021 Olga Vorobiova. All rights reserved.
//

#ifndef memoryManager_hpp
#define memoryManager_hpp

#include <cstdint>

using byte = uint8_t;

class MemoryManager
{
public:
    MemoryManager(void* ptr, size_t memorySize, size_t blockSize);
    
    void* allocate();
    
    void free(void* p);
    
    size_t getMaxBlocks() const {return m_blocksCount;}
    
    bool isFreeBlockAvailable() const {return m_freeBlocksCount;}
    
    MemoryManager(const MemoryManager&) = delete;
    
    MemoryManager& operator= (const MemoryManager &str) = delete;
    
private:
    size_t findFreeBlock();
    
    size_t m_blocksCount;
    size_t m_freeBlocksCount;
    size_t m_infoSize;
    
    byte* m_memoryPtr;
    size_t m_blockSize;
    
};

#endif /* memoryManager_hpp */

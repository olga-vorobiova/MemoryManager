//
//  testMemoryManager.hpp
//
//  Created by Olga Vorobiova on 27.02.2021.
//  Copyright © 2021 Olga Vorobiova. All rights reserved.
//

#ifndef testMemoryManager_hpp
#define testMemoryManager_hpp

#include <stdio.h>

bool testConstruction(size_t memorySize, size_t blockSize, bool expectedResult);
bool testConstruction();
bool testBlockCount(size_t memorySize, size_t blockSize, size_t expectedResult);
bool testBlockCount();
bool testAllocateException();
bool testFreeException();
bool testAllocateAndFree();
bool testMultipleFreeForSamePointer(size_t memorySize, size_t blockSize);
bool testMultipleFreeForSamePointer();
void testMemoryManager();

#endif /* testMemoryManager_hpp */

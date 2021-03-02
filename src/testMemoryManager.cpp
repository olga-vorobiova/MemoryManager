//
//  testMemoryManager.cpp
//
//  Created by Olga Vorobiova on 27.02.2021.
//  Copyright © 2021 Olga Vorobiova. All rights reserved.
//

#include <iostream>
#include <memory>
#include "testMemoryManager.hpp"
#include "memoryManager.hpp"

using std::cout;
using std::endl;

bool testConstruction(size_t memorySize, size_t blockSize, bool expectedResult)
{
    bool realResult = true;
    try
    {
        auto ptr = std::make_unique<byte[]>(memorySize);
        MemoryManager memoryManager(ptr.get(), sizeof(byte) * memorySize, blockSize);
        
    }
    catch (const std::exception& exception)
    {
        realResult = false;
    }
    
    return  realResult == expectedResult;
}

bool testConstruction()
{
    bool testResult = true;
    
    testResult = testConstruction(120, 3, true);
    testResult &= testConstruction(50, 0, false);
    testResult &= testConstruction(50, 50, true);
    testResult &= testConstruction(5, 20, false);
    
    return testResult;
}

bool testBlockCount(size_t memorySize, size_t blockSize, size_t expectedResult)
{
    try
    {
        auto ptr = std::make_unique<byte[]>(memorySize);
        MemoryManager memoryManager(ptr.get(), sizeof(byte) * memorySize, blockSize);
        
        return memoryManager.getMaxBlocks() == expectedResult;
    }
    catch (const std::exception& exception)
    {
        return false;
    }
}

bool testBlockCount()
{
    bool testResult = true;
    
    // 301 blocks * 2 bytes + 38 bytes of info = 640 bytes
    // 38 bytes of info * 8 bits = 304 bits (used 301 bits of 304)
    testResult = testBlockCount(640, 2, 301);
    
    // 1 block * 10 bytes + 0 bytes of info = 10 bytes
    testResult &= testBlockCount(10, 10, 1);
    
    // 1 block * 20 bytes + 1 byte of info = 21 bytes
    // 1 byte of info * 8 bits = 8 bits (used 1 bit of 8)
    testResult &= testBlockCount(40, 20, 1);
    
    // 8 blocks * 2 bytes + 2 bytes of info = 18 bytes
    // 2 bytes of info * 8 = 16 bits (used 8 from 16)
    testResult &= testBlockCount(19, 2, 8);
    
    // 10 blocks * 4 bytes + 2 bytes of info = 42 bytes
    // 2 bytes of info * 8 bits = 16 bits (used 10 of 16)
    testResult &= testBlockCount(45, 4, 10);
    
    return testResult;
}

bool testAllocateException()
{
    bool testResult = false;
    try
    {
        auto ptr = std::make_unique<byte[]>(10);
        MemoryManager memoryManager(ptr.get(), sizeof(byte) * 10, sizeof(int));
        
        for(size_t index = 0; index < memoryManager.getMaxBlocks(); ++index)
        {
             memoryManager.allocate();
        }
        
        memoryManager.allocate();
    }
    catch (const std::exception& exception)
    {
        testResult = true;
    }
    
    return testResult;
}

bool testFreeException()
{
    bool testResult = false;
    try
    {
        auto ptr = std::make_unique<byte[]>(16);
        MemoryManager memoryManager(ptr.get(), sizeof(byte) * 16, sizeof(short));
        short* blockPtr = nullptr;
        
        for(size_t index = 0; index < memoryManager.getMaxBlocks(); ++index)
        {
            blockPtr = static_cast<short*>(memoryManager.allocate());
        }
        
        blockPtr++;
        memoryManager.free(blockPtr);
    }
    catch (const std::exception& exception)
    {
        testResult = true;
    }
    
    return testResult;
}

bool testAllocateAndFree()
{
    bool testResult = true;
    try
    {
        auto ptr = std::make_unique<byte[]>(30);
        MemoryManager memoryManager(ptr.get(), sizeof(byte) * 30, sizeof(short));
        short* blockPtr = nullptr;
        
        for(size_t index = 0; index < memoryManager.getMaxBlocks(); ++index)
        {
            blockPtr = static_cast<short*>(memoryManager.allocate());
        }
        
        memoryManager.free(blockPtr);
        short* newBlockPtr = static_cast<short*>(memoryManager.allocate());
        
        return blockPtr == newBlockPtr;
    }
    catch (const std::exception& exception)
    {
        testResult = false;
    }
    return testResult;
}

bool testMultipleFreeForSamePointer(size_t memorySize, size_t blockSize)
{
    bool testResult = false;
    try
    {
        auto ptr = std::make_unique<byte[]>(memorySize);
        MemoryManager memoryManager(ptr.get(), sizeof(byte) * memorySize, blockSize);
        void* blockPtr = nullptr;
        
        for(size_t index = 0; index < memoryManager.getMaxBlocks(); ++index)
        {
            blockPtr = static_cast<short*>(memoryManager.allocate());
        }
        
        memoryManager.free(blockPtr);
        memoryManager.free(blockPtr);
    }
    catch (const std::exception& exception)
    {
        testResult = true;
    }
    return testResult;
}

bool testMultipleFreeForSamePointer()
{
    bool testResult = true;
    
    testResult = testMultipleFreeForSamePointer(10, 10);
    testResult &= testMultipleFreeForSamePointer(20, 2);
    
    return testResult;
}

void printTestResult(bool testResult, std::string functionName)
{
    if(testResult)
        cout << "test \"" << functionName << "\" passed" << endl;
    else
        cout << "test \"" << functionName << "\" failed" << endl;
}

void testMemoryManager()
{
    printTestResult(testConstruction(), "construction");
    printTestResult(testBlockCount(), "block count");
    printTestResult(testAllocateException(), "allocate exception");
    printTestResult(testFreeException(), "free exception");
    printTestResult(testAllocateAndFree(), "allocate and free");
    printTestResult(testMultipleFreeForSamePointer(), "call free multiple times for the same pointer");
}

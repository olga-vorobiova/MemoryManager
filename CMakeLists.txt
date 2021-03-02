cmake_minimum_required(VERSION 3.1) 

project(memoryManager)			

set (CMAKE_CXX_STANDARD 14)
set (SOURCES src/main.cpp src/memoryManager.cpp src/testMemoryManager.cpp)		

add_executable(testMemoryManager ${SOURCES})
		
									
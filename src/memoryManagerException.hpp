//
//  memoryManagerException.hpp
//
//  Created by Olga Vorobiova on 27.02.2021.
//  Copyright © 2021 Olga Vorobiova. All rights reserved.
//

#ifndef memoryManagerException_hpp
#define memoryManagerException_hpp

#include <stdio.h>
#include <exception>
#include <string>

class MemoryManagerException: public std::bad_alloc
{
public:
    MemoryManagerException(): std::bad_alloc(){}
    MemoryManagerException(std::string msg): std::bad_alloc(), m_errorMessage(msg){}
    const char* what() const noexcept override
    {
        if(m_errorMessage.size())
            return m_errorMessage.c_str();
        
        return std::bad_alloc::what();
    }
    
private:
    std::string m_errorMessage;
};

#endif /* memoryManagerException_hpp */

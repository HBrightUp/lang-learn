#include <iostream>
#include "./types.h"
#include"./interview/include/interfaces.h"
#include"./cppprevious/include/interfaces.h"
#include"./cpp20/include/interfaces.h"
#include"./cpp23/include/interfaces.h"

int main() {
    
    cxx_version version = cxx_version::interview;
    
    switch (version)
    {
        case cxx_version::interview:
            interview_call();
            break;
        case cxx_version::cppprevious:
            cppprevious_call();
            break;
        case cxx_version::cpp20:
            cpp20_call();
            break;
        case cxx_version::cpp23:
            cpp23_call();
            break;
        
        default:
            break;
    }
    
  
    return 0;
}



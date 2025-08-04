#include<iostream>

#include"../include/source_location.h"


void log(const std::string& message, std::source_location loc ) {
    std::cout << loc.file_name() << ":" << loc.line() << " - " << message << "\n";
}
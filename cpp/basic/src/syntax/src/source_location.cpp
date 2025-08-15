#include"../include/source_location.h"



#include <iostream>
#include <string_view>
#include <source_location>
 
void log(const std::string_view message,
         const std::source_location location = 
               std::source_location::current())
{
    std::cout << "file: "
              << location.file_name() << "("
              << location.line() << ":"
              << location.column() << ") `"
              << location.function_name() << "`: "
              << message << '\n';
}
 
template <typename T> void fun(T x)
{
    log(x); //  file: /home/hml/project/lang-learn/cpp/basic/src/syntax/src/source_location.cpp(23:8) `void fun(T) [with T = const char*]`: Hello C++20!
}
 


void call_source_location() {
    log("Hello world!");    //  file: /home/hml/project/lang-learn/cpp/basic/src/syntax/src/source_location.cpp(29:8) `void call_source_location()`: Hello world!
    fun("Hello C++20!");
}
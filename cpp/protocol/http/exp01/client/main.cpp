#include <iostream>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "./httplib.h"


int main() {

    // HTTP
    httplib::Client cli("http://localhost:8080");

    // HTTPS
    //httplib::Client cli("https://yhirose.github.io");

    auto res = cli.Get("/hi");
    std::cout << "status: " << res->status << std::endl;    //  status: 200
    std::cout << "body: " << res->body << std::endl;    //  body: Hello World!
    

    return 0;
}

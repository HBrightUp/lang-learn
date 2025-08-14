#include<iostream>
#include "./httplib.h"

int main(void)
    {
    using namespace httplib;

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    // Match the request path against a regular expression
    // and extract its captures
    svr.Get(R"(/numbers/(\d+))", [&](const Request& req, Response& res) {
        auto numbers = req.matches[1];
        res.set_content(numbers, "text/plain");
    });

    // Capture the second segment of the request path as "id" path param
    svr.Get("/users/:id", [&](const Request& req, Response& res) {
        auto user_id = req.path_params.at("id");
        res.set_content(user_id, "text/plain");
    });

    // Extract values from HTTP headers and URL query params
    svr.Get("/body-header-param", [](const Request& req, Response& res) {
        if (req.has_header("Content-Length")) {
            auto val = req.get_header_value("Content-Length");
        }
        if (req.has_param("key")) {
            auto val = req.get_param_value("key");
        }
        
        res.set_content(req.body, "text/plain");
    });

    // If the handler takes time to finish, you can also poll the connection state
    svr.Get("/task", [&](const Request& req, Response& res) {
        const char * result = nullptr;
        //process.run(); // for example, starting an external process
        while (result == nullptr) {
        sleep(1);
        if (req.is_connection_closed()) {
            //process.kill(); // kill the process
            return;
        }
        //result = process.stdout(); // != nullptr if the process finishes
        result = "task finished!";
        }
        res.set_content(result, "text/plain");
    });

    svr.set_pre_compression_logger([](const httplib::Request& req, const httplib::Response& res) {
        // Log before compression - res.body contains uncompressed content
        // Content-Encoding header is not yet set
        //your_pre_compression_logger(req, res);
        std::cout << "set_pre_compression_logger" << std::endl;
    });

    svr.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        std::cout << req.method << " " << req.path << " -> " << res.status << std::endl;
    });

    svr.set_error_handler([](const auto& req, auto& res) {
        auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        char buf[BUFSIZ];
        snprintf(buf, sizeof(buf), fmt, res.status);
        res.set_content(buf, "text/html");  //  Error Status: 404
    });

    svr.Get("/stop", [&](const Request& req, Response& res) {
        svr.stop();
    });

   

  svr.listen("localhost", 1234);
}
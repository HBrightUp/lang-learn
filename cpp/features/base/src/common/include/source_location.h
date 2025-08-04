#pragma once
#include <source_location>

void log(const std::string& message, std::source_location loc = std::source_location::current());
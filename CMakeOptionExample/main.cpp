#include <iostream>

#ifdef ENABLE_LOGGING
void log(const std::string& message) {
    std::cout << "LOG: " << message << std::endl;
}
#else
void log(const std::string&) {}
#endif

int main() {
    log("This is a log message.");
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#include <iostream>

int main() {
    std::cout << "*** Current build information ***" << std::endl;
    std::cout << "Build Type: " << BUILD_TYPE << std::endl;
    std::cout << "CMake Generator: " << CMAKE_GENERATOR << std::endl;
    std::cout << "Architecture: " << ARCHITECTURE << std::endl;
    std::cout << "*** End of build information ***" << std::endl;
    return 0;
}

#include <iostream>
#include <vector>

int main() {

    try {

        return(0u);

    }
    catch (const std::exception& err) {
        std::cerr << "Exception: " << err.what() << '\n';
    }
    catch (const char* err) {
        std::cerr << "Exception: " << err << '\n';
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }

}

#include <iostream>
#include <unistd.h> // for sleep

int main() {
    
    int count = 5;
    for (size_t i = 0; i < count; i++){
        std::cout << "a" << std::endl;
    }
    return 0;
}

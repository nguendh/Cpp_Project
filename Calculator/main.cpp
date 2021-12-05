#include <iostream>
#include "BigInt.h"

int main() {
    bigint a, b;
    std::cin >> a >> b;
    std::cout << a * b << std::endl;
    std::cout << a + b << std::endl;
    std::cout << a - b << std::endl;
}

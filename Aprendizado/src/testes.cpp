#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
    std::vector<int> age = {18, 20, 22, 25, 30};
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "David", "Eve"};
    for (size_t i = 0; i < age.size(); ++i) {
        std::cout << names[i] << " is " << age[i] << " years old." << std::endl;
        std::cin >> age[i];
        std::cout << "Updated age for " << names[i] << ": " << age[i] << std::endl;
    }
    // Example of a simple C++ program
    return 0;
}
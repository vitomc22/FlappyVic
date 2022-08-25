#include "flappy.hpp"
#include <cstdlib>
#include <ctime>
#include <memory>

int main() {
    //semeador randomico
    std::srand(std::time(0));
    
    auto flappy = std::make_shared<FlappyBird>();
    flappy->run();
    return 0;
}
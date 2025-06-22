/**
 * @file main.cpp
 * @brief Ponto de entrada principal da aplicação.
 */
#include "core/Game.hpp"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Uma exceção ocorreu: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Uma exceção desconhecida ocorreu." << std::endl;
        return 1;
    }

    return 0;
}
/**
 * @file main.cpp
 * @brief Ponto de entrada principal da aplicação.
 */

#include "core/Game.hpp"
#include <iostream>

/**
 * @brief Função principal da aplicação.
 * 
 * Cria uma instância do jogo e executa o loop principal. 
 * Utiliza blocos try-catch para capturar exceções durante a execução.
 * 
 * @return int Código de retorno da aplicação (0 para sucesso, 1 para erro).
 */
int main() {
    try {
        /// Instancia e inicia o jogo.
        Game game;
        game.run();
    }
    catch (const std::exception& e) {
        /// Captura exceções padrão e imprime a mensagem de erro.
        std::cerr << "Uma exceção ocorreu: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        /// Captura qualquer outra exceção que não derive de std::exception.
        std::cerr << "Uma exceção desconhecida ocorreu." << std::endl;
        return 1;
    }

    return 0;
}

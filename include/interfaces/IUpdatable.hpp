/**
 * @file IUpdatable.hpp
 * @brief Define a interface para todos os objetos que precisam ser atualizados a cada frame.
 */
#pragma once

/**
 * @class IUpdatable
 * @brief Interface que define um "contrato" para objetos com lógica de atualização.
 *
 * Classes que herdam de IUpdatable devem implementar o método update(), que
 * será chamado a cada ciclo do loop principal do jogo para atualizar o estado do objeto.
 */
class IUpdatable {
public:
    /**
     * @brief Destrutor virtual padrão.
     */
    virtual ~IUpdatable() = default;

    /**
     * @brief Função virtual pura para atualizar a lógica do objeto.
     * @param deltaTime O tempo, em segundos, decorrido desde o último frame. Essencial para física e movimentos consistentes.
     */
    virtual void update(float deltaTime) = 0;
};
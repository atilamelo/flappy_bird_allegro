/**
 * @file PlayerData.hpp
 * @brief Declaração da classe PlayerData para armazenamento global de informações do jogador.
 *
 * Esta classe funciona como um singleton que armazena e fornece acesso global ao nome
 * e à pontuação do jogador durante a execução do jogo.
 */

#pragma once
#include <iostream>
#include <string>

/**
 * @brief Armazena informações globais do jogador, como nome e pontuação.
 *
 * Utiliza membros estáticos para garantir que as informações sejam compartilhadas globalmente.
 * Proíbe cópia e atribuição para garantir a unicidade da instância.
 */
class PlayerData {
public:
    /**
     * @brief Obtém a instância singleton da classe PlayerData.
     * @return Referência para a instância única.
     */
    static PlayerData& getInstance();

    /**
     * @brief Define o nome do jogador.
     * @param name Nome do jogador.
     */
    static void setName(const std::string& name);

    /**
     * @brief Recupera o nome do jogador.
     * @return Nome do jogador.
     */
    static std::string getName();

    /**
     * @brief Define a pontuação do jogador.
     * @param score Pontuação do jogador.
     */
    static void setScore(int score);

    /**
     * @brief Recupera a pontuação do jogador.
     * @return Pontuação atual do jogador.
     */
    static int getScore();

private:
    static std::string playerName; ///< Nome global do jogador.
    static int playerScore;        ///< Pontuação global do jogador.

    PlayerData() = default;

    // Impede cópia e atribuição para garantir unicidade.
    PlayerData(const PlayerData&) = delete;
    PlayerData& operator=(const PlayerData&) = delete;
};

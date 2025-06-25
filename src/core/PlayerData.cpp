/**
 * @file PlayerData.cpp
 * @brief Implementação da classe PlayerData para manipulação global de dados do jogador.
 */

#include <iostream>
#include "core/PlayerData.hpp"

// Definição dos membros estáticos globais
std::string PlayerData::playerName = "";
int PlayerData::playerScore = 0;
int PlayerData::gamesPlayed = 0;

/**
 * @brief Define o nome do jogador.
 * @param name Nome a ser atribuído.
 */
void PlayerData::setName(const std::string& name){
    playerName = name;
}

/**
 * @brief Obtém o nome do jogador atual.
 * @return Nome do jogador.
 */
std::string PlayerData::getName(){
    return playerName;
}

/**
 * @brief Define a pontuação atual do jogador.
 * @param score Valor da pontuação.
 */
void PlayerData::setScore(int score){
    playerScore = score;
}

/**
 * @brief Obtém a pontuação atual do jogador.
 * @return Pontuação.
 */
int PlayerData::getScore(){
    return playerScore;
}

/**
 * @brief Define número de jogos atual do jogador.
 * @param games Valor do número de jogos.
 */
void PlayerData::setGames(int games){
    gamesPlayed = games;
}

/**
 * @brief Obtém o número de jogos atual do jogador.
 * @return Número de jogos.
 */
int PlayerData::getGames(){
    return gamesPlayed;
}

/**
 * @brief Obtém a instância única de PlayerData.
 * @return Referência para a instância.
 */
PlayerData& PlayerData::getInstance() {
    static PlayerData instance;
    return instance;
}
#include <iostream>
#include "core/PlayerData.hpp"

// Definição dos membros estáticos
std::string PlayerData::playerName = "";
int PlayerData::playerScore = 0;

void PlayerData::setName(const std::string& name){
    playerName = name;
}

std::string PlayerData::getName(){
    return playerName;
}

void PlayerData::setScore(int score){
    playerScore = score;
}

int PlayerData::getScore(){
    return playerScore;
}
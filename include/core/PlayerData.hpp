#include <iostream>

// Classe de estado global para armazenar as informaçoes do jogador
class PlayerData {
public:
    static PlayerData& getInstance() {
        static PlayerData instance; // Instância única
        return instance;
    }
    
    static void setName(const std::string& name);
    static std::string getName();
    static void setScore(int score);
    static int getScore();
private:
    static std::string playerName;
    static int playerScore;
    PlayerData() = default;

    PlayerData(const PlayerData&) = delete;            // Impede cópia
    PlayerData& operator=(const PlayerData&) = delete; // Impede atribuição
};
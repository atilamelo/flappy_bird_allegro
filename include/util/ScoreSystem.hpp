#pragma once

#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>
#include <filesystem>  // C++17 filesystem

// Exceção para erros de nome
class NameException : public std::invalid_argument {
public:
    explicit NameException(const std::string& message) 
        : std::invalid_argument(message) {}
};

// Exceção para erros de pontuação
class ScoreException : public std::out_of_range {
public:
    explicit ScoreException(const std::string& message) 
        : std::out_of_range(message) {}
};

class ScoreSystem {
public:    
    // Declaração da constante MAX_SCORE
    static const int MAX_SCORE = 1000000000; // 1 bilhão
    
    ScoreSystem() : dataFile("Scores.csv") {
        loadData();
    }
    explicit ScoreSystem(const std::string& file) : dataFile(file) {
        loadData();
    }
    
    // Valida o formato do nome
    void validatePlayerName(const std::string& name) const;
    
    // Registra ou atualiza pontuação
    void registerOrUpdateScore(const std::string& name, int score);
    
    // Obtém as melhores pontuações
    std::vector<std::pair<std::string, int>> getTopScores(int count = 5) const;

private:
    // Mapa para armazenar nome->pontuação
    std::map<std::string, int> scoreMap;
    std::string dataFile;

    // Função para remover espaços do início e fim da string
    static std::string trim(const std::string& str);
    
    // Verifica caracteres válidos no nome
    bool validateNameChars(const std::string& name) const;
    
    // Carrega dados do arquivo
    void loadData();
    
    // Salva dados no arquivo
    void saveData() const;
};
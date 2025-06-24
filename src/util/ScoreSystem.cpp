/**
 * @file ScoreSystem.cpp
 * @brief Implementação da classe ScoreSystem.
 */
#include "util/ScoreSystem.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

ScoreSystem& ScoreSystem::getInstance() {
    static ScoreSystem instance;
    return instance;
}

ScoreSystem::ScoreSystem() : dataFile("Scores.csv") {
    loadData();
}

// --- Funções Utilitárias ---

std::string ScoreSystem::trim(const std::string& str) {
    const char* whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

std::string ScoreSystem::toUpper(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return upperStr;
}

bool ScoreSystem::validateNameChars(const std::string& name) const {
    for (char c : name) {
        if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ')) {
            return false;
        }
    }
    return true;
}

// --- Funções de Validação e Lógica Principal ---

void ScoreSystem::validatePlayerName(const std::string& name) const {
    if (name.empty()) {
        throw NameException("O nome não pode ser vazio.");
    }
    if (name.length() < 3 || name.length() > 12) {
        throw NameException("O nome deve ter entre 3 e 12 caracteres.");
    }
    if (!validateNameChars(toUpper(name))) { // Valida a versão em maiúsculas
        throw NameException("O nome contém caracteres inválidos.");
    }
}

void ScoreSystem::registerOrUpdateScore(const std::string& name, int score) {
    std::string normalizedName = toUpper(trim(name));
    
    validatePlayerName(normalizedName);
    
    if (score < 0 || score > MAX_SCORE) {
        throw ScoreException("Pontuação inválida (deve ser entre 0 e " + std::to_string(MAX_SCORE) + ")");
    }

    auto it = scoreMap.find(normalizedName);
    if (it != scoreMap.end()) {
        if (score > it->second) {
            it->second = score;
        }
    } else {
        scoreMap[normalizedName] = score;
    }
    
    saveData();
}

std::vector<std::pair<std::string, int>> ScoreSystem::getTopScores(int count) const {
    std::vector<std::pair<std::string, int>> scores;
    scores.reserve(scoreMap.size());
    for (const auto& entry : scoreMap) {
        scores.push_back(entry);
    }

    // Ordena o vetor em ordem decrescente de pontuação.
    std::sort(scores.begin(), scores.end(), 
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        });

    // Limita ao número de resultados desejado.
    if (count > 0 && static_cast<size_t>(count) < scores.size()) {
        scores.resize(count);
    }
    return scores;
}


int ScoreSystem::getPlayerScore(const std::string& name) const {
    // 1. Normaliza o nome de entrada da mesma forma que fazemos ao salvar,
    // para garantir que a busca seja consistente.
    std::string normalizedName = toUpper(trim(name));

    // 2. Busca o nome normalizado no mapa de pontuações.
    // O método find() de std::map é eficiente para essa busca.
    auto it = scoreMap.find(normalizedName);

    // 3. Verifica se o jogador foi encontrado.
    if (it != scoreMap.end()) {
        // Se o iterador for diferente do final, o jogador existe.
        // Retornamos sua pontuação (o segundo elemento do par).
        return it->second;
    } else {
        return 0;
    }
}

// --- Funções de Acesso a Arquivo ---

void ScoreSystem::loadData() {
    if (!fs::exists(dataFile)) {
        std::ofstream createFile(dataFile);
        if (createFile.is_open()) {
            createFile.close();
            std::cout << "Arquivo de pontuações criado: " << dataFile << std::endl;
        } else {
            throw std::runtime_error("Falha ao criar arquivo de pontuações: " + dataFile);
        }
        return;
    }

    std::ifstream file(dataFile);
    if (!file.is_open()) {
        throw std::runtime_error("Falha ao abrir arquivo: " + dataFile);
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream ss(line);
        std::string name;
        std::string scoreStr;
        if (getline(ss, name, ';') && getline(ss, scoreStr)) {
            try {
                int score = std::stoi(scoreStr);
                scoreMap[name] = score; // Assume que os nomes no arquivo já estão normalizados
            } catch (...) {
                std::cerr << "Aviso: Linha inválida no arquivo de scores ignorada: " << line << std::endl;
            }
        }
    }
}

void ScoreSystem::saveData() const {
    std::ofstream file(dataFile);
    if (!file.is_open()) {
        throw std::runtime_error("Falha ao salvar dados no arquivo: " + dataFile);
    }

    for (const auto& entry : scoreMap) {
        file << entry.first << ";" << entry.second << "\n";
    }
}
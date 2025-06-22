#include "util/ScoreSystem.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

// Função para remover espaços do início e fim da string
string ScoreSystem::trim(const string& str) {
    const char* whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    size_t end = str.find_last_not_of(whitespace);
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Valida caracteres do nome
bool ScoreSystem::validateNameChars(const string& name) const {
    for (char c : name) {
        // Permite: letras maiúsculas (A-Z), números (0-9) e espaços
        if (!(c >= 'A' && c <= 'Z') && 
            !(c >= '0' && c <= '9') && 
            c != ' ') {
            return false;
        }
    }
    return true;
}

// Valida o nome completo
void ScoreSystem::validatePlayerName(const string& name) const {
    if (name.empty()) {
        throw NameException("Nome vazio");
    }
    if (name.length() > 20) {
        throw NameException("Nome muito longo (>20 chars)");
    }
    if (!validateNameChars(name)) {
        throw NameException("Caracteres inválidos no nome (apenas alfanuméricos)");
    }
}

// Carrega dados do arquivo
void ScoreSystem::loadData() {
    // Verifica se o arquivo existe usando C++17 filesystem
    if (!fs::exists(dataFile)) {
        // Cria um arquivo vazio se não existir
        ofstream createFile(dataFile);
        if (createFile.is_open()) {
            createFile.close();
            cout << "Arquivo de banco de dados criado: " << dataFile << endl;
        } else {
            throw runtime_error("Falha ao criar arquivo de banco de dados: " + dataFile);
        }
        return; // Retorna pois não há dados para carregar
    }

    ifstream file(dataFile);
    if (!file.is_open()) {
        throw runtime_error("Falha ao abrir arquivo: " + dataFile);
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string name;
        string scoreStr;
        if (getline(ss, name, ';') && getline(ss, scoreStr)) {
            try {
                int score = stoi(scoreStr);
                scoreMap[name] = score;
            } catch (...) {
                // Ignora linhas inválidas
            }
        }
    }
}

// Salva dados no arquivo
void ScoreSystem::saveData() const {
    ofstream file(dataFile);
    if (!file.is_open()) {
        throw runtime_error("Falha ao salvar dados no arquivo: " + dataFile);
    }

    for (const auto& entry : scoreMap) {
        file << entry.first << ";" << entry.second << "\n";
    }
}

// Registra/atualiza pontuação
void ScoreSystem::registerOrUpdateScore(const string& name, int score) {
    string trimmedName = trim(name);
    validatePlayerName(trimmedName);
    
    if (score < 0 || score > MAX_SCORE) {
        throw ScoreException("Pontuação inválida (0 a " + to_string(MAX_SCORE) + ")");
    }

    // Atualiza se existir, insere se não existir
    auto it = scoreMap.find(trimmedName);
    if (it != scoreMap.end()) {
        if (score > it->second) {
            it->second = score;
        }
    } else {
        scoreMap[trimmedName] = score;
    }
    
    saveData();
}

// Obtém top scores
vector<pair<string, int>> ScoreSystem::getTopScores(int count) const {
    // Copia o mapa para um vetor de pares
    vector<pair<string, int>> scores;
    for (const auto& entry : scoreMap) {
        scores.push_back(entry);
    }

    // Ordena em ordem decrescente por pontuação
    sort(scores.begin(), scores.end(), 
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });

    // Limita ao número de resultados
    if (count > 0 && static_cast<size_t>(count) < scores.size()) {
        scores.resize(count);
    }
    return scores;
}
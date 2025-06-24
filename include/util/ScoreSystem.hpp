/**
 * @file ScoreSystem.hpp
 * @brief Definição da classe ScoreSystem para gerenciamento de pontuações, implementada como um Singleton.
 * @version 2.1
 * @date 2025-06-24
 */
#pragma once

#include <vector>
#include <string>
#include <map>
#include <stdexcept>

/**
 * @class NameException
 * @brief Exceção customizada para erros de validação de nome de jogador.
 */
class NameException : public std::invalid_argument {
public:
    explicit NameException(const std::string& message) : std::invalid_argument(message) {}
};

/**
 * @class ScoreException
 * @brief Exceção customizada for erros de validação de pontuação.
 */
class ScoreException : public std::out_of_range {
public:
    explicit ScoreException(const std::string& message) : std::out_of_range(message) {}
};

/**
 * @class ScoreSystem
 * @brief Gerencia o placar, incluindo carregamento, salvamento e validação.
 *
 * Esta classe é implementada como um Singleton para garantir um único ponto de
 * acesso global aos dados de pontuação. Acesso é feito através de
 * ScoreSystem::getInstance().
 */
class ScoreSystem {
public:
    static const int MAX_SCORE = 1000000000; ///< Pontuação máxima permitida.

    /**
     * @brief Obtém a única instância da classe ScoreSystem.
     * @return Referência para a instância do Singleton.
     */
    static ScoreSystem& getInstance();

    // --- Impede Cópia e Atribuição ---
    ScoreSystem(const ScoreSystem&) = delete;
    ScoreSystem& operator=(const ScoreSystem&) = delete;
    
    // --- Métodos Públicos de Interface ---

    /**
     * @brief Valida um nome de jogador de acordo com as regras definidas.
     * @param name O nome a ser validado.
     * @throw NameException se o nome for inválido.
     */
    void validatePlayerName(const std::string& name) const;

    /**
     * @brief Registra uma nova pontuação ou atualiza uma existente se a nova for maior.
     * @param name O nome do jogador. O nome será normalizado (sem espaços extras e em maiúsculas).
     * @param score A pontuação a ser registrada.
     * @throw NameException se o nome for inválido.
     * @throw ScoreException se a pontuação estiver fora do intervalo permitido.
     */
    void registerOrUpdateScore(const std::string& name, int score);
    
    /**
     * @brief Obtém uma lista das melhores pontuações, ordenadas da maior para a menor.
     * @param count O número de melhores pontuações a serem retornadas.
     * @return Um vetor de pares contendo nome e pontuação.
     */
    std::vector<std::pair<std::string, int>> getTopScores(int count = 5) const;

    /**
     * @brief Obtém a melhor pontuação registrada para um jogador específico.
     *
     * Este método é case-insensitive (não diferencia maiúsculas de minúsculas) e
     * ignora espaços no início e fim do nome.
     *
     * @param name O nome do jogador a ser consultado.
     * @return A melhor pontuação do jogador, ou 0 se o jogador não for encontrado.
     */
    int getPlayerScore(const std::string& name) const;

private:
    /**
     * @brief Construtor privado para implementar o padrão Singleton.
     */
    ScoreSystem();

    // --- Membros ---
    std::map<std::string, int> scoreMap;
    const std::string dataFile;

    // --- Funções Utilitárias Privadas ---
    static std::string trim(const std::string& str);
    static std::string toUpper(const std::string& str);
    bool validateNameChars(const std::string& name) const;
    void loadData();
    void saveData() const;
};
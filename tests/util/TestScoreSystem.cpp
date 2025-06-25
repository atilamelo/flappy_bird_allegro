#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "util/ScoreSystem.hpp" 
#include <fstream>
#include <filesystem>
#include <stdexcept> 

namespace fs = std::filesystem;

// Constante para o nome do arquivo de teste, garantindo que os testes não interfiram no arquivo real
const std::string TEST_DATA_FILE = "TestScores.csv";

// Helper para limpar o arquivo de teste antes de cada caso de teste
void cleanUpTestFile() {
    if (fs::exists(TEST_DATA_FILE)) {
        fs::remove(TEST_DATA_FILE);
    }
}

// Classes de exceção personalizadas para "NameException" e "ScoreException".
// Elas devem ser definidas em "util/ScoreSystem.hpp" ou em um arquivo de exceções separado
// que seja incluído pelo "ScoreSystem.hpp". Se já existirem, este trecho pode ser removido.
class NameException : public std::runtime_error {
public:
    explicit NameException(const std::string& message) : std::runtime_error(message) {}
};

class ScoreException : public std::runtime_error {
public:
    explicit ScoreException(const std::string& message) : std::runtime_error(message) {}
};

// Modificação da classe ScoreSystem para usar um arquivo de teste específico.
// Isso permite que os testes manipulem um arquivo temporário sem afetar os dados reais do jogo.
class TestableScoreSystem : public ScoreSystem {
public:
    // Construtor que inicializa o ScoreSystem com o nome do arquivo de teste.
    // Limpa o mapa de pontuações e recarrega os dados do arquivo de teste (que pode estar vazio).
    TestableScoreSystem(const std::string& filename = TEST_DATA_FILE) : ScoreSystem() {
        dataFile = filename; // Sobrescreve o nome do arquivo padrão com o nome do arquivo de teste
        scoreMap.clear();    // Limpa os dados que o construtor base pode ter carregado do arquivo padrão
        loadData();          // Carrega dados do arquivo de teste (que será recém-criado ou limpo)
    }
};

// --- Testes para o Sistema de Pontuação (ScoreSystem) ---
TEST_SUITE("Testes do ScoreSystem") {

    // Configuração a ser executada antes de cada caso de teste neste "TEST_SUITE"
    DOCTEST_BEFORE_EACH() {
        cleanUpTestFile(); // Garante que o arquivo de teste esteja limpo antes de cada teste
    }

    // Limpeza a ser executada após cada caso de teste neste "TEST_SUITE"
    DOCTEST_AFTER_EACH() {
        cleanUpTestFile(); // Remove o arquivo de teste após cada teste para manter o ambiente limpo
    }

    // Testa se a instância do Singleton é sempre a mesma
    TEST_CASE("A instância do Singleton é sempre a mesma") {
        TestableScoreSystem& instance1 = TestableScoreSystem::getInstance();
        TestableScoreSystem& instance2 = TestableScoreSystem::getInstance();
        CHECK(&instance1 == &instance2); // Verifica se os endereços de memória das instâncias são iguais
    }

    // --- Testes das Funções Utilitárias ---

    // Testa a função utilitária "trim"
    TEST_CASE("A função utilitária trim funciona corretamente") {
        TestableScoreSystem system; // Instância para acessar o método trim
        CHECK(system.trim("  hello world  ") == "hello world");
        CHECK(system.trim("hello") == "hello");
        CHECK(system.trim("   ") == "");
        CHECK(system.trim("") == "");
    }

    // Testa a função utilitária "toUpper"
    TEST_CASE("A função utilitária toUpper funciona corretamente") {
        TestableScoreSystem system; // Instância para acessar o método toUpper
        CHECK(system.toUpper("HelloWorld123") == "HELLOWORLD123");
        CHECK(system.toUpper("test") == "TEST");
        CHECK(system.toUpper("") == "");
    }

    // Testa se "validateNameChars" permite caracteres válidos
    TEST_CASE("validateNameChars permite caracteres válidos") {
        TestableScoreSystem system;
        CHECK(system.validateNameChars("ABCDEF"));
        CHECK(system.validateNameChars("12345"));
        CHECK(system.validateNameChars("ABC 123"));
        CHECK(system.validateNameChars("A B C D E F"));
        CHECK(system.validateNameChars("1 2 3 4 5"));
    }

    // Testa se "validateNameChars" rejeita caracteres inválidos
    TEST_CASE("validateNameChars rejeita caracteres inválidos") {
        TestableScoreSystem system;
        CHECK_FALSE(system.validateNameChars("ABC!"));
        CHECK_FALSE(system.validateNameChars("XYZ@"));
        CHECK_FALSE(system.validateNameChars("user#1"));
    }

    // --- Testes das Funções de Validação e Lógica Principal ---

    // Testa se "validatePlayerName" lança exceção para nome vazio
    TEST_CASE("validatePlayerName lança exceção para nome vazio") {
        TestableScoreSystem system;
        CHECK_THROWS_AS(system.validatePlayerName(""), NameException);
    }

    // Testa se "validatePlayerName" lança exceção para nome muito curto
    TEST_CASE("validatePlayerName lança exceção para nome muito curto") {
        TestableScoreSystem system;
        CHECK_THROWS_AS(system.validatePlayerName("AB"), NameException);
    }

    // Testa se "validatePlayerName" lança exceção para nome muito longo
    TEST_CASE("validatePlayerName lança exceção para nome muito longo") {
        TestableScoreSystem system;
        CHECK_THROWS_AS(system.validatePlayerName("ABCDEFGHIJKLMNOP"), NameException);
    }

    // Testa se "validatePlayerName" lança exceção para caracteres inválidos
    TEST_CASE("validatePlayerName lança exceção para caracteres inválidos") {
        TestableScoreSystem system;
        CHECK_THROWS_AS(system.validatePlayerName("John!"), NameException);
        CHECK_THROWS_AS(system.validatePlayerName("Mary@"), NameException);
    }

    // Testa se "registerOrUpdateScore" registra um novo jogador
    TEST_CASE("registerOrUpdateScore registra um novo jogador") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("PlayerOne", 100);
        CHECK(system.getPlayerScore("PlayerOne") == 100);
    }

    // Testa se "registerOrUpdateScore" atualiza um jogador existente com pontuação maior
    TEST_CASE("registerOrUpdateScore atualiza um jogador existente com pontuação maior") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("PlayerTwo", 50);
        CHECK(system.getPlayerScore("PlayerTwo") == 50);
        system.registerOrUpdateScore("PlayerTwo", 150);
        CHECK(system.getPlayerScore("PlayerTwo") == 150);
    }

    // Testa se "registerOrUpdateScore" não atualiza com pontuação menor
    TEST_CASE("registerOrUpdateScore não atualiza com pontuação menor") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("PlayerThree", 200);
        CHECK(system.getPlayerScore("PlayerThree") == 200);
        system.registerOrUpdateScore("PlayerThree", 100);
        CHECK(system.getPlayerScore("PlayerThree") == 200);
    }

    // Testa se "registerOrUpdateScore" normaliza o nome do jogador ("trim" e "toUpper")
    TEST_CASE("registerOrUpdateScore normaliza o nome do jogador (trim & toUpper)") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("  player_four  ", 75);
        CHECK(system.getPlayerScore("player_four") == 75); // Busca com nome original (mas armazenado normalizado)
        CHECK(system.getPlayerScore("PLAYER_FOUR") == 75); // Busca com nome já normalizado
        CHECK(system.getPlayerScore("  Player_Four  ") == 75); // Busca com nome misto e espaços
    }

    // Testa se "registerOrUpdateScore" lança exceção para pontuação inválida (muito baixa)
    TEST_CASE("registerOrUpdateScore lança exceção para pontuação inválida (muito baixa)") {
        TestableScoreSystem system;
        CHECK_THROWS_AS(system.registerOrUpdateScore("BadScore", -1), ScoreException);
    }

    // Testa se "registerOrUpdateScore" lança exceção para pontuação inválida (muito alta)
    // "MAX_SCORE" deve ser definido em "ScoreSystem.hpp"
    TEST_CASE("registerOrUpdateScore lança exceção para pontuação inválida (muito alta)") {
        TestableScoreSystem system;
        // Assume-se que MAX_SCORE é definido no seu ScoreSystem.hpp
        CHECK_THROWS_AS(system.registerOrUpdateScore("AnotherBadScore", MAX_SCORE + 1), ScoreException);
    }

    // Testa se "getPlayerScore" retorna 0 para jogador inexistente
    TEST_CASE("getPlayerScore retorna 0 para jogador inexistente") {
        TestableScoreSystem system;
        CHECK(system.getPlayerScore("NonExistentPlayer") == 0);
    }

    // Testa se "getPlayerScore" retorna a pontuação correta para jogador existente
    TEST_CASE("getPlayerScore retorna pontuação correta para jogador existente") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("ExistingPlayer", 300);
        CHECK(system.getPlayerScore("ExistingPlayer") == 300);
    }

    // Testa se "getPlayerScore" lida com a normalização do nome
    TEST_CASE("getPlayerScore lida com a normalização do nome") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("  test_player  ", 450);
        CHECK(system.getPlayerScore("test_player") == 450);
        CHECK(system.getPlayerScore("TEST_PLAYER") == 450);
        CHECK(system.getPlayerScore("  TeSt_PlAyEr  ") == 450);
    }

    // Testa se "getTopScores" retorna as pontuações em ordem decrescente
    TEST_CASE("getTopScores retorna as pontuações em ordem decrescente") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("PlayerA", 100);
        system.registerOrUpdateScore("PlayerB", 300);
        system.registerOrUpdateScore("PlayerC", 50);
        system.registerOrUpdateScore("PlayerD", 200);

        auto topScores = system.getTopScores(0); // Pega todos os scores
        REQUIRE(topScores.size() == 4);
        CHECK(topScores[0].first == "PLAYERB");
        CHECK(topScores[0].second == 300);
        CHECK(topScores[1].first == "PLAYERD");
        CHECK(topScores[1].second == 200);
        CHECK(topScores[2].first == "PLAYERA");
        CHECK(topScores[2].second == 100);
        CHECK(topScores[3].first == "PLAYERC");
        CHECK(topScores[3].second == 50);
    }

    // Testa se "getTopScores" retorna o número especificado de pontuações
    TEST_CASE("getTopScores retorna o número especificado de pontuações") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("PlayerA", 100);
        system.registerOrUpdateScore("PlayerB", 300);
        system.registerOrUpdateScore("PlayerC", 50);
        system.registerOrUpdateScore("PlayerD", 200);

        auto top3Scores = system.getTopScores(3);
        REQUIRE(top3Scores.size() == 3);
        CHECK(top3Scores[0].first == "PLAYERB");
        CHECK(top3Scores[0].second == 300);
        CHECK(top3Scores[1].first == "PLAYERD");
        CHECK(top3Scores[1].second == 200);
        CHECK(top3Scores[2].first == "PLAYERA");
        CHECK(top3Scores[2].second == 100);
    }

    // Testa se "getTopScores" retorna todas as pontuações se o "count" for maior que o total
    TEST_CASE("getTopScores retorna todas as pontuações se o count for maior que o total") {
        TestableScoreSystem system;
        system.registerOrUpdateScore("PlayerX", 10);
        system.registerOrUpdateScore("PlayerY", 20);
        
        auto topScores = system.getTopScores(10);
        REQUIRE(topScores.size() == 2);
        CHECK(topScores[0].first == "PLAYERY");
        CHECK(topScores[0].second == 20);
        CHECK(topScores[1].first == "PLAYERX");
        CHECK(topScores[1].second == 10);
    }

    // --- Testes das Funções de Acesso a Arquivo (Persistência de Dados) ---

    // Testa a persistência de dados: pontuações são salvas e carregadas corretamente
    TEST_CASE("Persistência de dados: pontuações são salvas e carregadas corretamente") {
        // Primeiro, registre algumas pontuações com uma instância do sistema
        {
            TestableScoreSystem system1;
            system1.registerOrUpdateScore("PersistPlayer1", 100);
            system1.registerOrUpdateScore("PersistPlayer2", 200);
            system1.registerOrUpdateScore("PersistPlayer3", 50);
        } // "system1" é destruído aqui, o que deve acionar o "saveData()"

        // Em seguida, crie uma nova instância e verifique se os dados foram carregados do arquivo
        {
            TestableScoreSystem system2; // Esta nova instância carregará os dados do arquivo
            CHECK(system2.getPlayerScore("PersistPlayer1") == 100);
            CHECK(system2.getPlayerScore("PersistPlayer2") == 200);
            CHECK(system2.getPlayerScore("PersistPlayer3") == 50);
            
            // Verifique também a ordem dos top scores na nova instância
            auto topScores = system2.getTopScores(0);
            REQUIRE(topScores.size() == 3);
            CHECK(topScores[0].first == "PERSISTPLAYER2");
            CHECK(topScores[0].second == 200);
        }
    }

    // Testa se "loadData" cria o arquivo se ele não existir
    TEST_CASE("loadData cria o arquivo se ele não existir") {
        cleanUpTestFile(); // Garante que o arquivo não exista antes do teste
        TestableScoreSystem system; // O construtor chamará "loadData()"
        CHECK(fs::exists(TEST_DATA_FILE)); // Verifica se o arquivo foi criado
    }

    // Testa se "loadData" lida com linhas corrompidas de forma graciosa (ignorando-as)
    TEST_CASE("loadData lida com linhas corrompidas de forma graciosa") {
        cleanUpTestFile();
        // Crie um arquivo com algumas linhas válidas e uma linha corrompida
        std::ofstream file(TEST_DATA_FILE);
        file << "PLAYER1;100\n";
        file << "LINHA_CORROMPIDA_SEM_PONTUACAO;\n"; // Linha corrompida
        file << "PLAYER2;200\n";
        file.close();

        // Crie uma nova instância do sistema e verifique se apenas os dados válidos foram carregados
        TestableScoreSystem system;
        CHECK(system.getPlayerScore("PLAYER1") == 100);
        CHECK(system.getPlayerScore("PLAYER2") == 200);
        // A linha corrompida deve ser ignorada, e não deve ter inserido um jogador com pontuação 0
        CHECK(system.getPlayerScore("LINHA_CORROMPIDA_SEM_PONTUACAO") == 0);
    }
}
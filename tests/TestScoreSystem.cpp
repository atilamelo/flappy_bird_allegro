#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "../include/ScoreSystem.hpp"
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Teste da função de validação de nomes de jogadores
TEST_CASE("Teste de validação de nomes") {
    ScoreSystem system;
    
    SUBCASE("Nomes válidos") {
        // Deve aceitar letras maiúsculas, espaços e números
        CHECK_NOTHROW(system.validatePlayerName("PLAYER1"));
        CHECK_NOTHROW(system.validatePlayerName("PLAYER 2"));
        CHECK_NOTHROW(system.validatePlayerName("A B C 123"));
        CHECK_NOTHROW(system.validatePlayerName("  SPACES "));  // Verifica tratamento de espaços nas extremidades
        CHECK_NOTHROW(system.validatePlayerName("PLAYER ONE"));
        CHECK_NOTHROW(system.validatePlayerName("123 ABC"));
        CHECK_NOTHROW(system.validatePlayerName("A")); // Nome mínimo válido
    }
    
    SUBCASE("Nomes inválidos") {
        // Deve lançar exceções para nomes inválidos
        CHECK_THROWS_AS(system.validatePlayerName("player"), NameException); // minúsculas não permitidas
        CHECK_THROWS_AS(system.validatePlayerName(""), NameException); // nome vazio
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER@"), NameException); // caractere especial não permitido
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER!"), NameException);
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER-"), NameException); // hífen não permitido
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER_NAME"), NameException); // underscore não permitido
        CHECK_THROWS_AS(system.validatePlayerName("JOGADOR Ç"), NameException); // caractere não ASCII
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER COM NOME MUITO LONGO ACIMA DE VINTE"), NameException); // nome excede o limite de tamanho
    }
}

// Teste do registro e atualização de pontuações no arquivo
TEST_CASE("Teste de registro e atualização de pontuações") {
    const string testFile = "test_scores.csv";
    {
        // Cria arquivo inicial com pontuação existente
        ofstream file(testFile);
        file << "EXISTING;100\n";
        file.close();
    }
    
    ScoreSystem system(testFile);
    
    SUBCASE("Novo jogador") {
        // Deve registrar nova pontuação se jogador for novo
        system.registerOrUpdateScore("NEWPLAYER", 200);
        auto top = system.getTopScores();
        bool found = false;
        for (const auto& player : top) {
            if (player.first == "NEWPLAYER" && player.second == 200) {
                found = true;
                break;
            }
        }
        CHECK(found);
    }
    
    SUBCASE("Atualização com pontuação maior") {
        // Deve atualizar a pontuação se a nova for maior
        system.registerOrUpdateScore("EXISTING", 150);
        auto top = system.getTopScores();
        CHECK(top[0].second == 150);
    }
    
    SUBCASE("Atualização com pontuação menor") {
        // Não deve substituir se a nova pontuação for menor
        system.registerOrUpdateScore("EXISTING", 50);
        auto top = system.getTopScores();
        CHECK(top[0].second == 100);
    }
    
    SUBCASE("Nome com espaços") {
        // Testa registro de nome com espaços internos
        system.registerOrUpdateScore("PLAYER WITH SPACES", 300);
        auto top = system.getTopScores();
        bool found = false;
        for (const auto& player : top) {
            if (player.first == "PLAYER WITH SPACES" && player.second == 300) {
                found = true;
                break;
            }
        }
        CHECK(found);
    }
    
    remove(testFile.c_str());
}

// Verifica ordenação correta por pontuação (decrescente)
TEST_CASE("Teste de ordenação do ranking") {
    const string testFile = "sort_test.csv";
    ScoreSystem system(testFile);
    
    system.registerOrUpdateScore("PLAYER1", 100);
    system.registerOrUpdateScore("PLAYER2", 300);
    system.registerOrUpdateScore("PLAYER3", 200);
    
    auto top = system.getTopScores(3);
    CHECK(top[0].second == 300);
    CHECK(top[0].first == "PLAYER2");
    CHECK(top[1].second == 200);
    CHECK(top[1].first == "PLAYER3");
    CHECK(top[2].second == 100);
    CHECK(top[2].first == "PLAYER1");
    
    remove(testFile.c_str());
}

// Garante que os dados são persistidos corretamente no arquivo
TEST_CASE("Teste de persistência") {
    const string testFile = "persistence_test.csv";
    
    {
        ScoreSystem system(testFile);
        system.registerOrUpdateScore("PERSISTENCE", 500);
    }
    
    // Deve carregar os dados persistidos ao reiniciar o sistema
    ScoreSystem system2(testFile);
    auto top = system2.getTopScores();
    bool found = false;
    for (const auto& player : top) {
        if (player.first == "PERSISTENCE" && player.second == 500) {
            found = true;
            break;
        }
    }
    CHECK(found);
    
    remove(testFile.c_str());
}

// Testa limites de valores permitidos para pontuação
TEST_CASE("Teste de limites de pontuação") {
    const string testFile = "limits_test.csv";
    ScoreSystem system(testFile);
    
    SUBCASE("Pontuação mínima") {
        // Zero deve ser aceito
        CHECK_NOTHROW(system.registerOrUpdateScore("MIN", 0));
    }
    
    SUBCASE("Pontuação máxima") {
        // Deve aceitar pontuação igual ao limite máximo definido
        CHECK_NOTHROW(system.registerOrUpdateScore("MAX", ScoreSystem::MAX_SCORE));
    }
    
    SUBCASE("Pontuação negativa") {
        // Pontuação negativa não é permitida
        CHECK_THROWS_AS(system.registerOrUpdateScore("NEG", -1), ScoreException);
    }
    
    SUBCASE("Pontuação acima do limite") {
        // Acima do máximo definido não é permitido
        CHECK_THROWS_AS(system.registerOrUpdateScore("HIGH", ScoreSystem::MAX_SCORE + 1), ScoreException);
    }
    
    remove(testFile.c_str());
}

// Verifica se o sistema lida bem com menos de 10 registros
TEST_CASE("Teste de top scores com menos de 10 registros") {
    const string testFile = "less_than_10.csv";
    ScoreSystem system(testFile);
    
    system.registerOrUpdateScore("ONE", 100);
    system.registerOrUpdateScore("TWO", 200);
    
    auto top = system.getTopScores();
    CHECK(top.size() == 2); // Deve retornar apenas os registros existentes
    CHECK(top[0].second == 200);
    CHECK(top[1].second == 100);
    
    remove(testFile.c_str());
}

// Testa remoção de espaços em nomes antes da validação
TEST_CASE("Teste de trim nos nomes") {
    const string testFile = "test_trim.csv";
    ScoreSystem system(testFile);
    
    SUBCASE("Nome com espaços no início e fim") {
        // Deve considerar "  KAKA  " e "KAKA" como o mesmo jogador
        system.registerOrUpdateScore("  KAKA  ", 800);
        system.registerOrUpdateScore("KAKA", 900); // Deve atualizar
        
        auto top = system.getTopScores();
        int count = 0;
        for (const auto& player : top) {
            if (player.first == "KAKA") {
                count++;
                CHECK(player.second == 900);
            }
        }
        CHECK(count == 1); // Apenas uma entrada deve existir
    }
    
    SUBCASE("Nome com espaços internos") {
        // Espaços internos são preservados
        system.registerOrUpdateScore("  PLAYER ONE  ", 1000);
        auto top = system.getTopScores();
        CHECK(top[0].first == "PLAYER ONE");
        CHECK(top[0].second == 1000);
    }
    
    remove(testFile.c_str());
}

// Testa comportamento com um arquivo inicialmente vazio
TEST_CASE("Teste com arquivo vazio") {
    const string testFile = "empty_file_test.csv";
    ofstream file(testFile); // Cria arquivo vazio
    file.close();
    
    ScoreSystem system(testFile);
    auto top = system.getTopScores();
    CHECK(top.empty()); // Não deve haver dados inicialmente
    
    // Após registrar, deve conter um único jogador
    system.registerOrUpdateScore("PLAYER1", 100);
    top = system.getTopScores();
    CHECK(top.size() == 1);
    CHECK(top[0].first == "PLAYER1");
    CHECK(top[0].second == 100);
    
    remove(testFile.c_str());
}

// Verifica robustez com dados malformados no arquivo
TEST_CASE("Teste com arquivo corrompido") {
    const string testFile = "corrupted_file_test.csv";
    {
        ofstream file(testFile);
        file << "PLAYER1;100\n";        // válido
        file << "INVALID_LINE\n";       // inválido (faltando separador)
        file << "PLAYER2;TEXT\n";       // inválido (pontuação não numérica)
        file << "PLAYER3;200\n";        // válido
        file.close();
    }
    
    ScoreSystem system(testFile);
    auto top = system.getTopScores();
    CHECK(top.size() == 2); // Apenas linhas válidas devem ser carregadas
    
    bool foundPlayer1 = false;
    bool foundPlayer3 = false;
    for (const auto& player : top) {
        if (player.first == "PLAYER1" && player.second == 100) {
            foundPlayer1 = true;
        } else if (player.first == "PLAYER3" && player.second == 200) {
            foundPlayer3 = true;
        }
    }
    CHECK(foundPlayer1);
    CHECK(foundPlayer3);
    
    remove(testFile.c_str());
}

// Teste de performance e comportamento com grande volume de dados
TEST_CASE("Teste de desempenho com muitos registros") {
    const string testFile = "performance_test.csv";
    ScoreSystem system(testFile);
    
    const int NUM_PLAYERS = 1000;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        system.registerOrUpdateScore("PLAYER" + to_string(i), i); // i = pontuação crescente
    }
    
    auto top = system.getTopScores(10);
    CHECK(top.size() == 10); // Deve retornar os 10 melhores
    for (int i = 0; i < 10; i++) {
        CHECK(top[i].second == NUM_PLAYERS - 1 - i); // Verifica ordem decrescente
    }
    
    remove(testFile.c_str());
}

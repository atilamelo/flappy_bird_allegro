#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "../include/score.hpp"
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

TEST_CASE("Teste de validação de nomes") {
    ScoreSystem system;
    
    SUBCASE("Nomes válidos") {
        CHECK_NOTHROW(system.validatePlayerName("PLAYER1"));
        CHECK_NOTHROW(system.validatePlayerName("PLAYER 2"));
        CHECK_NOTHROW(system.validatePlayerName("A B C 123"));
        CHECK_NOTHROW(system.validatePlayerName("  SPACES  "));
        CHECK_NOTHROW(system.validatePlayerName("PLAYER ONE"));
        CHECK_NOTHROW(system.validatePlayerName("123 ABC"));
        CHECK_NOTHROW(system.validatePlayerName("A"));
    }
    
    SUBCASE("Nomes inválidos") {
        CHECK_THROWS_AS(system.validatePlayerName("player"), NameException); // minúsculas
        CHECK_THROWS_AS(system.validatePlayerName(""), NameException); // vazio
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER@"), NameException); // caractere inválido
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER!"), NameException); // caractere inválido
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER-"), NameException); // hífen não permitido
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER_NAME"), NameException); // underscore não permitido
        CHECK_THROWS_AS(system.validatePlayerName("JOGADOR Ç"), NameException); // caractere especial
        CHECK_THROWS_AS(system.validatePlayerName("PLAYER COM NOME MUITO LONGO ACIMA DE VINTE"), NameException); // muito longo
    }
}

TEST_CASE("Teste de registro e atualização de pontuações") {
    const string testFile = "test_scores.csv";
    {
        ofstream file(testFile);
        file << "EXISTING;100\n";
        file.close();
    }
    
    ScoreSystem system(testFile);
    
    SUBCASE("Novo jogador") {
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
        system.registerOrUpdateScore("EXISTING", 150);
        auto top = system.getTopScores();
        CHECK(top[0].second == 150);
    }
    
    SUBCASE("Atualização com pontuação menor") {
        system.registerOrUpdateScore("EXISTING", 50);
        auto top = system.getTopScores();
        CHECK(top[0].second == 100);
    }
    
    SUBCASE("Nome com espaços") {
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

TEST_CASE("Teste de persistência") {
    const string testFile = "persistence_test.csv";
    
    {
        ScoreSystem system(testFile);
        system.registerOrUpdateScore("PERSISTENCE", 500);
    }
    
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

TEST_CASE("Teste de limites de pontuação") {
    const string testFile = "limits_test.csv";
    ScoreSystem system(testFile);
    
    SUBCASE("Pontuação mínima") {
        CHECK_NOTHROW(system.registerOrUpdateScore("MIN", 0));
    }
    
    SUBCASE("Pontuação máxima") {
        // Usar ScoreSystem::MAX_SCORE
        CHECK_NOTHROW(system.registerOrUpdateScore("MAX", ScoreSystem::MAX_SCORE));
    }
    
    SUBCASE("Pontuação negativa") {
        CHECK_THROWS_AS(system.registerOrUpdateScore("NEG", -1), ScoreException);
    }
    
    SUBCASE("Pontuação acima do limite") {
        // Usar ScoreSystem::MAX_SCORE + 1
        CHECK_THROWS_AS(system.registerOrUpdateScore("HIGH", ScoreSystem::MAX_SCORE + 1), ScoreException);
    }
    
    remove(testFile.c_str());
}

TEST_CASE("Teste de top scores com menos de 10 registros") {
    const string testFile = "less_than_10.csv";
    ScoreSystem system(testFile);
    
    system.registerOrUpdateScore("ONE", 100);
    system.registerOrUpdateScore("TWO", 200);
    
    auto top = system.getTopScores();
    CHECK(top.size() == 2);
    CHECK(top[0].second == 200);
    CHECK(top[1].second == 100);
    
    remove(testFile.c_str());
}
TEST_CASE("Teste de trim nos nomes") {
    const string testFile = "test_trim.csv";
    ScoreSystem system(testFile);
    
    SUBCASE("Nome com espaços no início e fim") {
        system.registerOrUpdateScore("  KAKA  ", 800);
        system.registerOrUpdateScore("KAKA", 900); // Deve atualizar ao invés de criar novo
        
        auto top = system.getTopScores();
        int count = 0;
        for (const auto& player : top) {
            if (player.first == "KAKA") {
                count++;
                CHECK(player.second == 900); // Deve manter a maior pontuação
            }
        }
        CHECK(count == 1); // Deve ter apenas uma entrada
    }
    
    SUBCASE("Nome com espaços internos") {
        system.registerOrUpdateScore("  PLAYER ONE  ", 1000);
        auto top = system.getTopScores();
        CHECK(top[0].first == "PLAYER ONE");
        CHECK(top[0].second == 1000);
    }
    
    remove(testFile.c_str());
}
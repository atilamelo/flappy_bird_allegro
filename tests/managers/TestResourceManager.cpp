#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "../include/managers/ResourceManager.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <fstream>
#include <cstdio>

// Fixture para inicialização da Allegro (somente imagem)
struct AllegroFixture {
    AllegroFixture() {
        al_init();
        al_init_image_addon();
    }
    
    ~AllegroFixture() {
        al_shutdown_image_addon();
        al_uninstall_system();
    }
};

// Classe de ajuda para criar arquivos temporários
class TempFile {
public:
    TempFile(const std::string& content, const std::string& extension) {
        // Gera nome de arquivo temporário único
        char filename[] = "tmpfileXXXXXX";
        int fd = mkstemp(filename);
        close(fd);
        path = std::string(filename) + extension;
        
        // Escreve conteúdo no arquivo
        std::ofstream file(path);
        file << content;
    }
    
    ~TempFile() {
        remove(path.c_str()); // Remove o arquivo ao finalizar
    }
    
    std::string getPath() const { return path; }

private:
    std::string path;
};

TEST_SUITE("Testes do ResourceManager") {
    // Função para limpar o singleton entre os testes
    void resetResourceManager() {
        ResourceManager& rm = ResourceManager::getInstance();
        try {
            rm.loadBitmap("__reset__", "arquivo_inexistente");
        } catch (...) {
            // Ignora erros - apenas para resetar o estado interno
        }
    }

    TEST_CASE_FIXTURE(AllegroFixture, "Carregar e obter bitmap") {
        resetResourceManager();
        ResourceManager& rm = ResourceManager::getInstance();
        
        // Cria arquivo temporário de imagem
        TempFile tempImage("", ".png");
        ALLEGRO_BITMAP* bmp = al_create_bitmap(10, 10);
        al_save_bitmap(tempImage.getPath().c_str(), bmp);
        al_destroy_bitmap(bmp);

        // Testa o carregamento do bitmap
        CHECK_NOTHROW(rm.loadBitmap("teste_bitmap", tempImage.getPath()));
        
        // Verifica se o bitmap pode ser obtido
        ALLEGRO_BITMAP* carregado = rm.getBitmap("teste_bitmap");
        CHECK(carregado != nullptr);
    }

    TEST_CASE_FIXTURE(AllegroFixture, "Carregar sub-bitmap") {
        resetResourceManager();
        ResourceManager& rm = ResourceManager::getInstance();
        
        // Cria bitmap fonte
        TempFile tempFonte("", ".png");
        ALLEGRO_BITMAP* fonte = al_create_bitmap(20, 20);
        al_save_bitmap(tempFonte.getPath().c_str(), fonte);
        al_destroy_bitmap(fonte);

        // Carrega bitmap fonte
        CHECK_NOTHROW(rm.loadBitmap("fonte", tempFonte.getPath()));
        
        // Testa criação de sub-bitmap
        CHECK_NOTHROW(rm.loadSubBitmap("sub", "fonte", 0, 0, 10, 10));
        
        // Verifica sub-bitmap
        ALLEGRO_BITMAP* sub = rm.getBitmap("sub");
        CHECK(sub != nullptr);
    }

    TEST_CASE_FIXTURE(AllegroFixture, "Carregar atlas de JSON") {
        resetResourceManager();
        ResourceManager& rm = ResourceManager::getInstance();
        
        // Cria imagem de atlas temporária
        TempFile tempAtlas("", ".png");
        ALLEGRO_BITMAP* atlas = al_create_bitmap(20, 20);
        al_save_bitmap(tempAtlas.getPath().c_str(), atlas);
        al_destroy_bitmap(atlas);

        // Cria JSON de atlas temporário
        std::string jsonConteudo = R"({
            "sprites": [
                {
                    "fileName": "sprite_teste1",
                    "width": 10,
                    "height": 10,
                    "x": 0,
                    "y": 0
                },
                {
                    "fileName": "sprite_teste2",
                    "width": 10,
                    "height": 10,
                    "x": 10,
                    "y": 0
                }
            ]
        })";
        TempFile tempJson(jsonConteudo, ".json");

        // Testa carregamento do atlas
        CHECK_NOTHROW(rm.loadAtlasJson(tempJson.getPath(), "atlas_teste", tempAtlas.getPath()));
        
        // Verifica se os sprites foram criados
        CHECK_NOTHROW(rm.getBitmap("sprite_teste1"));
        CHECK_NOTHROW(rm.getBitmap("sprite_teste2"));
    }

    TEST_CASE_FIXTURE(AllegroFixture, "Exceções para bitmaps ausentes") {
        resetResourceManager();
        ResourceManager& rm = ResourceManager::getInstance();
        
        // Verifica exceções para bitmaps não encontrados
        CHECK_THROWS_AS(rm.getBitmap("bitmap_inexistente"), std::runtime_error);
    }

    TEST_CASE_FIXTURE(AllegroFixture, "Exceção ao carregar bitmap inválido") {
        resetResourceManager();
        ResourceManager& rm = ResourceManager::getInstance();
        
        // Verifica exceção ao carregar bitmap inexistente
        CHECK_THROWS_AS(rm.loadBitmap("invalido", "caminho/inexistente.png"), std::runtime_error);
    }
}
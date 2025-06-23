/**
 * @file ScoreBoard.hpp
 * @brief Definição da classe ScoreBoard.
 */
#pragma once

#include "core/GameObject.hpp" // Certifique-se de que GameObject está incluído
#include "managers/ScoreManager.hpp" // Inclui o ScoreManager
#include "interfaces/IDrawable.hpp"
#include "interfaces/IUpdatable.hpp"
#include <allegro5/allegro5.h> // Para ALLEGRO_BITMAP

// Forward declaration para evitar dependência cíclica, se necessário
// class ScoreManager;

class ScoreBoard : public GameObject, public IDrawable, public IUpdatable {
private:
    ALLEGRO_BITMAP* boardTexture;
    ALLEGRO_BITMAP* currentMedal;
    const ScoreManager& scoreManager; // Referência para o ScoreManager

    float startY;
    float endY;
    float animationDuration;
    float elapsedTime;
    bool active;

    int finalScore;
    int bestScore;

    float currentScale; // Novo membro para controlar a escala

    void determineMedal();

public:
    /**
     * @brief Construtor da classe ScoreBoard.
     * @param finalY A posição Y final para onde o ScoreBoard deslizará.
     * @param duration A duração da animação de deslizamento.
     * @param scManager Referência para o ScoreManager para desenhar os números.
     * @param initialScale O fator de escala inicial para o ScoreBoard e seus elementos.
     */
    ScoreBoard(float finalY, float duration, const ScoreManager& scManager, float initialScale = 1.0f);

    /**
     * @brief Exibe o ScoreBoard com a pontuação final e a melhor pontuação.
     * @param score A pontuação obtida na rodada atual.
     * @param best A melhor pontuação registrada.
     */
    void show(int score, int best);

    /**
     * @brief Reinicia o estado do ScoreBoard, escondendo-o.
     */
    void reset();

    /**
     * @brief Atualiza a lógica do ScoreBoard, como a animação de deslizamento.
     * @param deltaTime O tempo decorrido desde a última atualização.
     */
    void update(float deltaTime) override;

    /**
     * @brief Desenha o ScoreBoard na tela.
     */
    void draw() const override;

    /**
     * @brief Define o fator de escala para o ScoreBoard.
     * @param newScale O novo fator de escala.
     */
    void setScale(float newScale);


    bool isActive() { return this->active; };
};


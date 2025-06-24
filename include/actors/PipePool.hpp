/**
 * @file PipePool.hpp
 * @brief Declaração da classe PipePool para gerenciamento de um pool de objetos PipePair.
 *
 * Esta classe implementa um pool de objetos PipePair, reutilizando-os para evitar
 * alocação/desalocação frequente e melhorar o desempenho do jogo.
 */

#pragma once
#include <vector>
#include <memory>
#include "actors/PipePair.hpp"

/**
 * @brief Gerencia um pool de objetos PipePair reutilizáveis para otimizar a performance.
 *
 * O pool mantém instâncias de PipePair e fornece um PipePair inativo quando necessário.
 * Se não houver PipePair inativo disponível, um novo será criado.
 */
class PipePool {
public:
    /**
     * @brief Construtor que inicializa o pool com um número inicial de PipePairs.
     * @param initialSize Tamanho inicial do pool.
     */
    PipePool(size_t initialSize);

    /**
     * @brief Retorna um PipePair inativo do pool, ou cria um novo se necessário.
     * @return Ponteiro para PipePair disponível.
     */
    PipePair* getPipe();

    /**
     * @brief Atualiza todos os PipePairs do pool.
     * @param deltaTime Tempo decorrido desde a última atualização.
     */
    void update(float deltaTime);

    /**
     * @brief Desenha todos os PipePairs ativos do pool.
     */
    void draw() const;

    /**
     * @brief Reseta todos os PipePairs para o estado inicial.
     */
    void reset();

private:
    std::vector<std::unique_ptr<PipePair>> pool; ///< Vetor de ponteiros únicos para PipePair.
};
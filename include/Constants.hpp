#pragma once

/**
 * @file Config.hpp
 * @brief Define todas as constantes globais e configurações do jogo.
 * @details Este arquivo centraliza os "números mágicos" do jogo em um só lugar,
 * facilitando o ajuste fino da jogabilidade (game tuning).
 */

/**
 * @defgroup ScreenConfig Configurações da Tela e Renderização
 * @brief Constantes relacionadas ao tamanho da tela e taxa de atualização.
 * @{
 */
constexpr float FPS = 30.0;         ///< Taxa de quadros por segundo (frames per second) alvo.
constexpr int BUFFER_W = 288;       ///< Largura do buffer de renderização (resolução interna horizontal).
constexpr int BUFFER_H = 512;       ///< Altura do buffer de renderização (resolução interna vertical).
/** @} */


/**
 * @defgroup BirdConfig Configurações do Pássaro
 * @brief Constantes que definem a física e as dimensões do pássaro.
 * @{
 */
constexpr float GRAVITY = 1000.0f;              ///< Aceleração da gravidade aplicada ao pássaro (pixels/s²).
constexpr float JUMP_IMPULSE_VELOCITY = -430.0f;///< Velocidade vertical instantânea aplicada ao pular (pixels/s).
constexpr float MAX_UP_ANGLE = 60.0f;           ///< Ângulo máximo de rotação para cima ao pular (em graus).
constexpr float TERMINAL_VELOCITY = 600.0f;     ///< Velocidade máxima de queda do pássaro (pixels/s).
constexpr float MAX_DOWN_ANGLE = -60.0f;        ///< Ângulo máximo de rotação para baixo ao cair (em graus).

constexpr int BIRD_WIDTH = 34;                  ///< Largura da hitbox do pássaro (em pixels).
constexpr int BIRD_HEIGHT = 24;                 ///< Altura da hitbox do pássaro (em pixels).

constexpr int FLOOR_HEIGHT = 56;                ///< Altura da área do chão (em pixels).
constexpr int PLAYABLE_AREA_HEIGHT = BUFFER_H - FLOOR_HEIGHT; ///< Altura útil da área de jogo.

constexpr int BIRD_START_X = BUFFER_W / 4;      ///< Posição X inicial do pássaro.
constexpr int BIRD_START_Y = BUFFER_H / 2;      ///< Posição Y inicial do pássaro.
/** @} */


/**
 * @defgroup PipeConfig Configurações dos Canos
 * @brief Constantes para o comportamento, aparência e geração dos canos.
 * @{
 */
const int PIPE_POOL_SIZE = 4;                   ///< Número de pares de canos gerenciados no pool de objetos.
const float PIPE_WIDTH = 52.0f;                 ///< Largura visual e da hitbox dos canos (pixels).
const float PIPE_SPEED = 170.0f;                ///< Velocidade de movimento horizontal dos canos (pixels/s).
const float PIPE_GAP = 150.0f;                  ///< Espaço vertical entre o cano superior e inferior (pixels).
const float PIPE_INTERVAL = 1.25f;              ///< Tempo entre o surgimento de novos pares de canos (segundos).

const float PIPE_MIN_HEIGHT = 20.0f;            ///< Altura mínima (posição Y) para a base do cano superior.
const float PIPE_MAX_HEIGHT = BUFFER_H - PIPE_GAP; ///< Altura máxima (posição Y) para a base do cano superior.
/** @} */


/**
 * @defgroup CollisionConfig Configurações de Colisão
 * @brief Parâmetros para ajustar a detecção de colisão.
 * @{
 */
constexpr float COLLISION_TOLERANCE = 0.0f;     ///< Fator de tolerância para a hitbox. 0 significa colisão exata.
/** @} */


/**
 * @defgroup ScoreConfig Configurações de Pontuação
 * @brief Constantes relacionadas ao sistema de pontuação do jogador.
 * @{
 */
constexpr int SCORE_INCREASE_AMOUNT = 1;        ///< Quantidade de pontos ganhos ao passar por um cano.
constexpr float SCORE_PASS_THRESHOLD_X = BIRD_START_X - (BIRD_WIDTH / 2); ///< Coordenada X que o cano deve cruzar para o jogador pontuar.
/** @} */


/**
 * @defgroup SceneConfig Configurações de Cena e Efeitos
 * @brief Constantes para elementos visuais como background e transições.
 * @{
 */
constexpr float BACKGROUND_SCROLL_SPEED = 30.0f;///< Velocidade de rolagem do plano de fundo para o efeito parallax (pixels/s).
constexpr float GAME_OVER_DELAY = 3.0f;         ///< Tempo de espera (em segundos) após a morte do jogador antes de transicionar de cena.
/** @} */
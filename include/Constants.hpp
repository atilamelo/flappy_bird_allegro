#pragma once

// --- Configurações da tela ---
constexpr float FPS = 30.0;
constexpr int BUFFER_W = 288;
constexpr int BUFFER_H = 512;

// --- Configurações do pássaro ---
constexpr float GRAVITY = 1000.0f;          // (pixels/s^2)
constexpr float JUMP_IMPULSE_VELOCITY = -430.0f;        // (pixels/s)
constexpr float MAX_UP_ANGLE = 60.0f; // Ângulo máximo para cima (graus)
constexpr float TERMINAL_VELOCITY = 600.0f; // Velocidade máxima de queda (pixels/s)
constexpr float MAX_DOWN_ANGLE = -60.0f; // Ângulo máximo para baixo (graus)

constexpr int BIRD_WIDTH = 34;
constexpr int BIRD_HEIGHT = 24;

constexpr int FLOOR_HEIGHT = 56;
constexpr int PLAYABLE_AREA_HEIGHT = BUFFER_H - FLOOR_HEIGHT;

constexpr int BIRD_START_X = BUFFER_W / 4;
constexpr int BIRD_START_Y = BUFFER_H / 2;

// --- Configurações dos canos ---
const int PIPE_POOL_SIZE = 4;
const float PIPE_WIDTH = 52.0f;             // Largura dos canos
const float PIPE_SPEED = 170.0f;            // Velocidade horizontal dos canos (pixels/s)
const float PIPE_GAP = 150.0f;              // Espaço vertical entre o cano superior e inferior (pixels)
const float PIPE_INTERVAL = 1.25f;          // Tempo entre o surgimento de novos pares de canos (segundos)

const float PIPE_MIN_HEIGHT = 20.0f;         // Margem do topo da tela para a base do cano superior
const float PIPE_MAX_HEIGHT = BUFFER_H - PIPE_GAP; // Margem do chão da tela para o topo do cano inferior

// --- Configurações de Colisão ---
constexpr float COLLISION_TOLERANCE = 0.0f; 

// --- Configurações de pontuação ---
constexpr int SCORE_INCREASE_AMOUNT = 1;
constexpr float SCORE_PASS_THRESHOLD_X = BIRD_START_X - (BIRD_WIDTH / 2);

// --- Background e Parallax ---
constexpr float BACKGROUND_SCROLL_SPEED = 30.0f; // Velocidade de rolagem do plano de fundo (pixels/s)

constexpr float GAME_OVER_DELAY = 3.0f;
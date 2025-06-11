#pragma once

// --- Configurações da Tela ---
constexpr float FPS = 60.0;
constexpr int BUFFER_W = 9 * 50;
constexpr int BUFFER_H = 16 * 50;

// --- Configurações do Jogo ---
constexpr float GRAVITY = 500.0f;
constexpr float JUMP_FORCE = 250.0f;
constexpr float TERMINAL_VELOCITY = 300.0f;

constexpr int BIRD_WIDTH = 20;
constexpr int BIRD_HEIGHT = 20; 

// --- Configurações dos Canos ---
const float PIPE_WIDTH = 60.0f;
const float PIPE_SPEED = 40.0f;
const float PIPE_GAP = 100.0f;
const float PIPE_INTERVAL = 2.0f;
const float PIPE_MIN_HEIGHT = 50.0f;
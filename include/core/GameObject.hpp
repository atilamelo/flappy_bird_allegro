/**
 * @file GameObject.hpp
 * @brief Definição da classe base GameObject, que armazena dados comuns a objetos de jogo.
 */
#pragma once

/**
 * @class GameObject
 * @brief Contém propriedades fundamentais para um objeto no mundo do jogo, como posição e tamanho.
 *
 * Esta classe serve como um contêiner de dados. Ela não define comportamentos
 * como update() ou draw(), que são gerenciados através de interfaces separadas
 * (ex: IDrawable, IUpdatable) para maior flexibilidade.
 */
class GameObject
{
protected:
    float x, y;         ///< Posição do objeto no eixo X e Y.
    float width, height;///< Dimensões do objeto.

public:
    /**
     * @brief Construtor da GameObject.
     * @param x Posição inicial no eixo X.
     * @param y Posição inicial no eixo Y.
     * @param w Largura inicial.
     * @param h Altura inicial.
     */
    GameObject(float x, float y, float w, float h);

    /**
     * @brief Destrutor virtual padrão.
     */
    virtual ~GameObject() = default;

    // --- Getters ---
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    // --- Setters ---
    void setX(float newX) { this->x = newX; }
    void setY(float newY) { this->y = newY; }
    void setWidth(float newWidth) { this->width = newWidth; }
    void setHeight(float newHeight) { this->height = newHeight; }
};
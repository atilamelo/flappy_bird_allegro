/**
 * @file IDrawable.hpp
 * @brief Define a interface para todos os objetos que podem ser desenhados na tela.
 */
#pragma once

/**
 * @class IDrawable
 * @brief Interface que define um contrato para objetos desenháveis.
 *
 * Qualquer classe que herdar publicamente de IDrawable se compromete a implementar
 * o método draw(). Isso permite que o motor de renderização trate diferentes
 * tipos de objetos de forma uniforme.
 */
class IDrawable {
public:
    /**
     * @brief Destrutor virtual padrão para garantir a segurança em hierarquias de classes.
     */
    virtual ~IDrawable() = default;

    /**
     * @brief Função virtual pura que deve ser implementada pela classe derivada para desenhar o objeto.
     */
    virtual void draw() const = 0;
};
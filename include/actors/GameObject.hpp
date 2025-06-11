#pragma once

class GameObject
{
protected:
    float x, y;
    float width, height;

public:
    GameObject(float x, float y, float w, float h);
    virtual ~GameObject();

    virtual void draw(float deltaTime) = 0;
    virtual void update(float deltaTime) = 0;

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    void setX(float x);
    void setY(float y);
};
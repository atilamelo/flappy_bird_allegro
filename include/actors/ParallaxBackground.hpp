#include "GameObject.hpp"
#include <allegro5/allegro.h>

class ParallaxBackground : public GameObject {
private:
    ALLEGRO_BITMAP* backgroundImage;
    float speed;
public:
    ParallaxBackground(float x, float y, float w, float h, ALLEGRO_BITMAP* image, float speed)
        : GameObject(x, y, w, h), backgroundImage(image), speed(speed) {}

    void draw() override;
    void update(float deltaTime) override;

    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }
};
#include "GameObject.hpp"
#include <allegro5/allegro.h>
#include "Constants.hpp"

class ParallaxBackground : public GameObject {
private:
    ALLEGRO_BITMAP* backgroundImage;
    float speed;
public:
    ParallaxBackground(ALLEGRO_BITMAP* image)
        : GameObject(0, 0, BUFFER_W, BUFFER_H), backgroundImage(image), speed(BACKGROUND_SCROLL_SPEED) {}

    void draw() override;
    void update(float deltaTime) override;

    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }
};
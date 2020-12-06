#ifndef PPGSO_MENU_SCENE_H
#define PPGSO_MENU_SCENE_H

#include <src/scene/scene.h>

class MenuScene : public Scene {
public:

    MenuScene(): Scene() {}

    void update(float time) override;

    void render() override;

};

#endif //PPGSO_MENU_SCENE_H

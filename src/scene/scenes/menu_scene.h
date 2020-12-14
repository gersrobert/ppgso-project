#ifndef PPGSO_MENU_SCENE_H
#define PPGSO_MENU_SCENE_H

#include <src/scene/scene.h>

class MenuScene : public Scene {
public:

    enum Mode {
        START, PAUSE, END
    };

    explicit MenuScene(SceneWindow &window, Mode mode): Scene(window), mode(mode) {}

    void update(float time) override;

    void render() override;

private:
    Mode mode;
};

#endif //PPGSO_MENU_SCENE_H

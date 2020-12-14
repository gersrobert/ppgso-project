#ifndef PPGSO_SCENE_WINDOW_H
#define PPGSO_SCENE_WINDOW_H

#include <iostream>
#include <map>

#include <ppgso/ppgso.h>
#include <src/objects/world/water.h>
#include <src/objects/world/chunk.h>
#include <src/objects/boat/boat.h>
#include <src/gui/compass.h>
#include <src/objects/sky/skydome.h>
#include <src/gui/text.h>
#include <src/scene/scenes/game_scene.h>
#include <src/scene/scene.h>
#include <src/scene/scenes/menu_scene.h>
#include <src/objects/world/lighthouse.h>
#include <src/gui/gui_boat.h>
#include "scene_window.h"

const uint32_t WIDTH = 2560;
const uint32_t HEIGHT = 1440;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    std::vector<std::shared_ptr<Scene>> scenes;
    size_t activeScene = 0;

    bool animate = true;

    void createGameScene();
    void createMenuScene(MenuScene::Mode mode);

public:
    /*!
     * Construct custom game window
     */
    SceneWindow(const int width, const int height);

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override;

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override;

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse bu
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override;

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override;

    void startGame();
    void pauseGame();
    void endGame();
};

#endif //PPGSO_SCENE_WINDOW_H

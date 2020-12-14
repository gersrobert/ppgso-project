#include "scene_window.h"

void SceneWindow::createGameScene() {
    auto scene = std::make_shared<GameScene>(*this);
    scenes.push_back(scene);

    scene->objects.clear();
    scene->guiObjects.clear();

    auto camera = std::make_unique<Camera>(80.0f, float(WIDTH) / float(HEIGHT), 0.1f, scene->VISIBILITY);
    scene->camera = move(camera);
    scene->guiProjection = glm::ortho(-float(WIDTH) / float(HEIGHT), float(WIDTH) / float(HEIGHT), -1.0f, 1.0f);

    auto skydome = std::make_unique<Skydome>(*scene);
    scene->objects.push_back(move(skydome));

    auto player = std::make_unique<Boat>(*scene);
    scene->objects.push_back(move(player));

    for (int32_t i = -1; i <= 1; ++i) {
        for (int32_t j = -1; j <= 1; ++j) {
            auto chunk = std::make_unique<Chunk>(*scene, glm::vec2{i, j});
            scene->objects.push_back(move(chunk));
        }
    }

    auto lightHouse = std::make_unique<LightHouse>(*scene);
    scene->lightHousePosition = {lightHouse->position.x , lightHouse->position.z};
    scene->objects.push_back(move(lightHouse));

    auto compass = std::make_unique<Compass>(*scene);
    scene->guiObjects.push_back(move(compass));
}

void SceneWindow::createMenuScene(MenuScene::Mode mode) {
    auto scene = std::make_shared<MenuScene>(*this, mode);
    scenes.push_back(scene);

    scene->objects.clear();
    scene->guiObjects.clear();

    scene->guiProjection = glm::ortho(-float(WIDTH) / float(HEIGHT), float(WIDTH) / float(HEIGHT), -1.0f, 1.0f);

    switch (mode) {
        case MenuScene::START: {
            auto boat = std::make_unique<GuiBoat>(*scene);
            scene->guiObjects.push_back(move(boat));

            auto text = std::make_unique<Text>(*scene, "Press enter to start", glm::vec4{.8, .8, .8, 1});
            text->position = {-0.8f, -0.8f, 0};
            scene->guiObjects.push_back(move(text));

            break;
        }
        case MenuScene::PAUSE: {

            auto topText = std::make_unique<Text>(*scene, "PAUSED", glm::vec4{.8, .8, .8, 1});
            topText->position = {-0.4f, 0.4f, 0};
            scene->guiObjects.push_back(move(topText));

            auto bottomText = std::make_unique<Text>(*scene, "Press P to resume", glm::vec4{.8, .8, .8, 1});
            bottomText->position = {-0.8f, -0.8f, 0};
            scene->guiObjects.push_back(move(bottomText));

            break;
        }
        case MenuScene::END: {

            auto topText = std::make_unique<Text>(*scene, "WASTED", glm::vec4{.8, .8, .8, 1});
            topText->position = {-0.4f, 0.4f, 0};
            scene->guiObjects.push_back(move(topText));

            break;
        }
    }
}

SceneWindow::SceneWindow(const int width, const int height) : Window{"playground", width, height} {
    srand(time(nullptr));

    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    createGameScene();
    createMenuScene(MenuScene::START);
    activeScene = 1;
}

void SceneWindow::onKey(int key, int scanCode, int action, int mods) {
    scenes.at(activeScene)->keyboard[key] = action;
}

void SceneWindow::onCursorPos(double cursorX, double cursorY) {
    scenes.at(activeScene)->cursor.x = cursorX;
    scenes.at(activeScene)->cursor.y = cursorY;
}


void SceneWindow::onMouseButton(int button, int action, int mods) {
}

void SceneWindow::onIdle() {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(scenes.at(activeScene)->bgColor.x, scenes.at(activeScene)->bgColor.y,
                 scenes.at(activeScene)->bgColor.z, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
    scenes.at(activeScene)->update(dt);
    scenes.at(activeScene)->render();
}

void SceneWindow::startGame() {
    scenes.erase(scenes.end());
    activeScene = 0;
}

void SceneWindow::pauseGame() {
    if (activeScene == 0) {
        createMenuScene(MenuScene::PAUSE);
        activeScene = 1;
    } else {
        scenes.erase(scenes.end());
        activeScene = 0;
    }
}

void SceneWindow::endGame() {
    createMenuScene(MenuScene::END);
    activeScene = 1;
}

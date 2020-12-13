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

const uint32_t WIDTH = 2560;
const uint32_t HEIGHT = 1440;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    std::vector<std::shared_ptr<Scene>> scenes;
    size_t activeScene = 0;

    bool animate = true;

    void createGameScene() {
        auto scene = std::make_shared<GameScene>();
        scenes.push_back(scene);

        scene->objects.clear();
        scene->guiObjects.clear();

        // Create a camera
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
        scene->objects.push_back(move(lightHouse));

        auto compass = std::make_unique<Compass>(*scene);
        scene->guiObjects.push_back(move(compass));
    }

    void createMenuScene() {
        auto scene = std::make_shared<MenuScene>();
        scenes.push_back(scene);

        scene->objects.clear();
        scene->guiObjects.clear();

        scene->guiProjection = glm::ortho(-float(WIDTH) / float(HEIGHT), float(WIDTH) / float(HEIGHT), -1.0f, 1.0f);

        auto boat = std::make_unique<GuiBoat>(*scene);
        scene->guiObjects.push_back(move(boat));

        auto text = std::make_unique<Text>(*scene, "Press P to start", glm::vec4{.8, .8, .8, 1});
        text->position = {-0.8f, -0.8f, 0};
        scene->guiObjects.push_back(move(text));
    }

    public:
    /*!
     * Construct custom game window
     */
    SceneWindow(const int width, const int height) : Window{"playground", width, height} {
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

        scenes.reserve(2);
        activeScene = 0;
        createMenuScene();
        createGameScene();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        scenes.at(activeScene)->keyboard[key] = action;

        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            std::cout << action << std::endl;
            activeScene++;
            activeScene %= scenes.size();
        }
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
        scenes.at(activeScene)->cursor.x = cursorX;
        scenes.at(activeScene)->cursor.y = cursorY;
    }

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse bu
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override {
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = animate ? (float) glfwGetTime() - time : 0;

        time = (float) glfwGetTime();

        // Set gray background
        glClearColor(scenes.at(activeScene)->bgColor.x, scenes.at(activeScene)->bgColor.y, scenes.at(activeScene)->bgColor.z, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scenes.at(activeScene)->update(dt);
        scenes.at(activeScene)->render();
    }
};

int main() {

    // Initialize our window
    SceneWindow window(WIDTH, HEIGHT);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}

// Playground
// - You can use this template project for quick C++ experiments

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "src/scene/camera.h"
#include "src/scene/scene.h"
#include "src/objects/player.h"
#include "src/objects/sea.h"
#include "src/objects/wave.h"

const uint32_t WIDTH = 2560;
const uint32_t HEIGHT = 1440;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = true;


    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f);
        camera->offset.z = camera->distance;
        camera->offset.y = -5.0f;
        scene.camera = move(camera);

        auto ocean = std::make_unique<Sea>();
        scene.objects.push_back(move(ocean));

        // Add player to the scene
        auto player = std::make_unique<Player>();
        scene.objects.push_back(move(player));
    }

public:
    /*!
     * Construct custom game window
     */
    SceneWindow(const int width, const int height) : Window{"playground", width, height} {
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
//        glEnable(GL_CULL_FACE);
//        glFrontFace(GL_CCW);
//        glCullFace(GL_BACK);

        initScene();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        scene.keyboard[key] = action;
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
        scene.cursor.x = cursorX;
        scene.cursor.y = cursorY;
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
        glClearColor(.5f, .5f, .5f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
    }
};

int main() {

    // Initialize our window
    SceneWindow window(WIDTH, HEIGHT);

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}

#include "chunk.h"
#include "island.h"
#include "water.h"

Chunk::Chunk(GameScene &scene, glm::vec2 position) : size(scene.VISIBILITY), position(position) {
    for (size_t i = 0; i < 16; ++i) {
        auto island = std::make_unique<Island>(scene, *this);
        scene.objects.push_back(move(island));
    }

    auto water = std::make_unique<Water>(scene, *this);
    scene.objects.push_back(move(water));
}

bool Chunk::update(Scene &scene, float dt) {
    auto gameScene = dynamic_cast<GameScene*>(&scene);

    if (gameScene->targetPosition.x < -size + size * (position.x - 1) * 2) {
        isActive = false;

        auto chunk = std::make_unique<Chunk>(*gameScene, glm::vec2{position.x - 3, position.y});
        scene.objects.push_back(move(chunk));
    }
    else if (gameScene->targetPosition.x > size + size * (position.x + 1) * 2) {
        isActive = false;

        auto chunk = std::make_unique<Chunk>(*gameScene, glm::vec2{position.x + 3, position.y});
        scene.objects.push_back(move(chunk));
    }
    else if (gameScene->targetPosition.z < -size + size * (position.y - 1) * 2) {
        isActive = false;

        auto chunk = std::make_unique<Chunk>(*gameScene, glm::vec2{position.x, position.y - 3});
        scene.objects.push_back(move(chunk));
    }
    else if (gameScene->targetPosition.z > size + size * (position.y + 1) * 2) {
        isActive = false;

        auto chunk = std::make_unique<Chunk>(*gameScene, glm::vec2{position.x, position.y + 3});
        scene.objects.push_back(move(chunk));
    }

    return isActive;
}

void Chunk::render(Scene &scene) {
}
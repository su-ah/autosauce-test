#include "shared/Scene.hpp"

Scene::Scene() {
    Scene::instance = this;
}

Scene::Scene(std::string &filename) {
    
}

Scene::~Scene() {

}

/**
 * This function is meant to load these Animation properties back into use
*/
void Scene::load() {
    for (auto object: this->objects) {
        object.load();
    }
}

/**
 * This function is meant to remove these Animation properties from use with the
 * intention that they will be used in the future.
*/
void Scene::unload() {
    for (auto object: this->objects) {
        object.unload();
    }
}

/**
 * Update the Animation properties <timestep> seconds into the future
*/
void Scene::update(double timestep) {
    for (auto object: this->objects) {
        object.update(timestep);
    }
}

/**
 * Getter for the light emitters in the scene
*/
std::vector<Object> Scene::getLights() {
    return this->lights;
}

/**
 * Adds a light to the list of emitters
 */
void Scene::addLight(const Object &light) {
    this->lights.push_back(light);
}

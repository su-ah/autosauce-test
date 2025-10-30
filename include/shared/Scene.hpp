#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <vector>

#include "shared/Object.hpp"
#include "modeling/Camera.hpp"

class Scene {
private:
    static Scene *instance;

    std::vector<Object> objects;
    std::shared_ptr<Camera> active_camera = nullptr;
public:

    static Scene *getInstance() {
        return Scene::instance;
    }

    Scene();
    Scene(std::string &filename);
    ~Scene();

    void load();
    void unload();
    void update(double timestep);

    std::shared_ptr<Camera> get_camera();
    void set_camera(std::shared_ptr<Camera> cam);
};

    std::vector<Object> getLights(); // change to Light class later
    void addLight(const Object &light); // change to Light class later
};
Scene *Scene::instance; // ??? i hate c++ -emmy

#endif

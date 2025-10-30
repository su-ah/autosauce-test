#ifndef MODEL_HPP
#define MODEL_HPP

#include "modeling/Mesh.hpp"
#include "modeling/Material.hpp"
#include <vector>
#include <memory>

namespace modeling {

class Model {
public:
    Model();

    Model(std::vector<std::shared_ptr<Mesh>> meshes, std::vector<std::shared_ptr<Material>> mats, std::shared_ptr<Shader> shader);

    // We need a list of meshes and a list of materials, one material per mesh
    ~Model();
    std::vector<std::shared_ptr<Mesh>> getMeshes() const;
    std::vector<std::shared_ptr<Material>> getMaterials() const;
    void addMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

    std::shared_ptr<Shader> getShader();

    // Rendering methods
    void setupForRendering();  // Prepare all meshes and bind shader

private:
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Shader> shader;


};

} // namespace modeling

#endif // MODEL_HPP
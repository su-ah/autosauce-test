#ifndef MODEL_HPP
#define MODEL_HPP

#include "modeling/Mesh.hpp"
#include "modeling/Material.hpp"
#include <vector>
#include <memory>

class Model {
public:
    Model();
    // We need a list of meshes and a list of materials, one material per mesh
    ~Model();
    std::vector<Mesh> getMeshes() const;
    std::vector<const Material*> getMaterials() const;
    void addMesh(const Mesh& mesh, const Material& material);

private:
    std::vector<Mesh> meshes;
    std::vector<const Material*> materials;


};
#endif // MODEL_HPP
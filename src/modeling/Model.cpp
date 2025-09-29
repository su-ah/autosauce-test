#include "modeling/Model.hpp"

Model::Model() {
    // Initialize empty vectors - they're already default constructed
}

Model::~Model() {
    // Vectors will automatically clean up their contents
    // No explicit cleanup needed for Mesh and Material objects
}

std::vector<Mesh> Model::getMeshes() const {
    return meshes;
}

std::vector<const Material*> Model::getMaterials() const {
    return materials;
}

void Model::addMesh(const Mesh& mesh, const Material& material) {
    meshes.push_back(mesh);
    materials.push_back(&material);
}
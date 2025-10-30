#include "modeling/ModelProperties.hpp"
#include "modeling/Model.hpp"

using namespace modeling;

ModelProperties::ModelProperties(std::string gltfFilename) 
    : gltfFilename(gltfFilename), model(nullptr) {
    // Initialize with null model and empty properties map
}
ModelProperties::~ModelProperties() {
    // Shared pointer will automatically clean up Model
    // Properties map will clean up automatically
}

void ModelProperties::load() {

}


/**
 * This function is meant to remove these 
 * Model properties from use with the
 * intention that they will be used in the future.
*/
void ModelProperties::unload() {

}

/**
 * Do the various buffer setups to prepare the model
 * for the shader program
*/
void ModelProperties::update(const animation::AnimationProperties &animProps) {
    if (model) {
        // Setup the model for rendering (bind shader and vertex data)
        model->setupForRendering();
        
        auto shader = model->getShader();

        // Set any modeling-specific uniforms
        if (shader && shader->is_bound()) {
            
        }
    }
}

bool ModelProperties::hasProperty(const std::string& tag) const {
    return properties.find(tag) != properties.end();
}

void ModelProperties::removeProperty(const std::string& tag) {
    properties.erase(tag);
}

#ifndef MODEL_PROPERTIES_HPP
#define MODEL_PROPERTIES_HPP

#include <string>

#include "animation/AnimationProperties.hpp"

namespace animation {
    class AnimationProperties;
}

namespace modeling {

/**
 * Stores all model related properties of an object
*/
class ModelProperties {
public:
    ModelProperties(std::string gltfFilename);
    ~ModelProperties();

    /**
     * This function is meant to load these 
     * Model properties back into use
    */
    void load();

    /**
     * This function is meant to remove these 
     * Model properties from use with the
     * intention that they will be used in the future.
    */
    void unload();

    /**
     * Do the various buffer setups to prepare the model
     * for the shader program
    */
    void update(const animation::AnimationProperties &animProps);
};

}

#endif
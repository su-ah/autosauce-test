#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <modeling/ModelLoader.hpp>

using namespace std;

class MeshLoadTest : public ::testing::Test {
protected:
    GLFWwindow* window = nullptr;

    void SetUp() override {
        // Initialize GLFW
        if (!glfwInit()) {
            FAIL() << "Failed to initialize GLFW";
        }

        // Set OpenGL version to 3.3 Core Profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Make window invisible for testing
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        // Create a windowed mode window and its OpenGL context
        window = glfwCreateWindow(1, 1, "Test Window", NULL, NULL);
        if (!window) {
            glfwTerminate();
            FAIL() << "Failed to create GLFW window";
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(window);
            glfwTerminate();
            FAIL() << "Failed to initialize GLAD";
        }

        // Verify OpenGL context is working
        const GLubyte* version = glGetString(GL_VERSION);
        if (!version) {
            glfwDestroyWindow(window);
            glfwTerminate();
            FAIL() << "Failed to get OpenGL version";
        }
    }

    void TearDown() override {
        // Cleanup OpenGL context
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }
};

TEST_F(MeshLoadTest, DoShit) {
	using namespace modeling;
	auto shaderp = make_shared<Shader>();
	auto models = ModelLoader::loadModels("assets/mymodel.gltf",shaderp);
}

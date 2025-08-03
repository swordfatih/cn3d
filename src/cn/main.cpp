#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stb_image.h>
#include <tiny_gltf.h>
#include <vulkan/vulkan.h>

int main()
{
    std::cout << "Program started" << std::endl
              << std::flush;

    // --------------------
    // GLFW initialization
    // --------------------
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    if(!glfwVulkanSupported())
    {
        std::cerr << "GLFW: Vulkan not supported" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "GLFW initialized and Vulkan is supported" << std::endl;

    // --------------------
    // GLM test
    // --------------------
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 5.0f, 6.0f);
    glm::vec3 c = a + b;

    std::cout << "GLM vector addition: (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;

    // --------------------
    // TinyGLTF test (loading dummy model)
    // --------------------
    tinygltf::TinyGLTF loader;
    tinygltf::Model    model;
    std::string        err;
    std::string        warn;
    bool               ret = loader.LoadASCIIFromFile(&model, &err, &warn, "dummy.gltf");

    std::cout << "TinyGLTF load result: " << ret << ", warnings: " << warn << ", errors: " << err << "" << std::endl;

    // --------------------
    // STB image test (try loading an image)
    // --------------------
    int            width, height, channels;
    unsigned char* data = stbi_load("dummy.png", &width, &height, &channels, 0);
    if(data)
    {
        std::cout << "STB loaded dummy.png with size " << width << "x" << height << ", channels: " << channels << "" << std::endl;
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "STB failed to load dummy.png" << std::endl;
    }

    glfwTerminate();
    return 0;
}

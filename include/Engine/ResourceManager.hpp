#pragma once

#include "Engine/Graphics/ShaderProgram.hpp"
//#include "Texture.hpp"

#include <map>
#include <string>

/**
 * Resource manager class
 */
class ResourceManager
{
private:

    /**
     * List of shaders identified by their corresponding string keys
     */
    std::map<std::string, ShaderProgram*> m_shaders;

    /**
     * List of textures identified by their corresponding string keys
     */
    //std::map<std::string, Texture*> m_textures;

    /**
     * @brief Constructor
     */
    ResourceManager();

public:
    /**
     * Delete copy constructor
     */
    ResourceManager(const ResourceManager&) = delete;

    /**
     * Delete assignment operator
     */
    ResourceManager& operator=(const ResourceManager&) = delete;

    /**
     * @brief Destructor
     */
    ~ResourceManager();

    /**
     * @brief Gets the singleton instance for the resource manager
     * @return Singleton instance for this class
     */
    static ResourceManager& GetInstance();

    /**
     * Initialize
     */
    static void Initialize();

    /**
     * @brief Cleanup resources
     */
    static void Cleanup();

    /**
     * @brief Creates a shader program from the provided info, and stores it 
     * as a shader resource identified by the provided string key.
     * @param[in] key Key identifier for the created shader program
     * @param[in] info Info about the creation
     * @return Reference to the created shader. Returns nullptr if the shader
     * creation failed.
     */
    static ShaderProgram* CreateShader(const std::string& key, const ShaderCreateInfo& info);

    /**
     * @brief Deletes the shader identified by the provided key from the resource manager
     * @param[in] key Key identifying the shader to delete
     */
    static void DeleteShader(const std::string& key);

    /**
     * @brief Gets the shader associated with the provided key.
     * @param[in] key Key identifying the shader we are trying to get
     * @return Reference to the shader identified by the provided key. Returns
     * nullptr if it does not exist.
     */
    static ShaderProgram* GetShader(const std::string& key);

    /**
     * @brief Creates a texture from the provided file path, and stores it 
     * as a texture resource identified by the provided string key.
     * @param[in] textureFilePath File path to the texture file
     * @param[in] key Key identifier for the created texture
     * @return Reference to the created texture. Returns nullptr if the texture
     * creation failed.
     */
    //Texture* CreateTexture(const std::string& textureFilePath, const std::string& key);

    /**
     * @brief Deletes the texture identified by the provided key from the resource manager
     * @param[in] key Key identifying the texture to delete
     */
    //void DeleteTexture(const std::string& key);

    /**
     * @brief Gets the texture associated with the provided key.
     * @param[in] key Key identifying the texture we are trying to get
     * @return Reference to the texture identified by the provided key. Returns
     * nullptr if it does not exist.
     */
    //Texture* GetTexture(const std::string& key);
};


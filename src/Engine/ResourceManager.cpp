#include "Engine/ResourceManager.hpp"

/**
 * @brief Constructor
 */
ResourceManager::ResourceManager()
    : m_shaders()
    //, m_textures()
{
}

/**
 * @brief Destructor
 */
ResourceManager::~ResourceManager()
{
}

/**
 * @brief Gets the singleton instance for the resource manager
 * @return Singleton instance for this class
 */
ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

/**
 * Initialize
 */
void ResourceManager::Initialize()
{
}

/**
 * @brief Cleanup resources
 */
void ResourceManager::Cleanup()
{
    ResourceManager& instance = GetInstance();
    for (auto it = instance.m_shaders.begin(); it != instance.m_shaders.end(); ++it)
    {
	delete it->second;
    }
    instance.m_shaders.clear();

    /*for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
    {
	delete it->second;
    }
    m_textures.clear();*/
}

/**
 * @brief Creates a shader program from the provided paths for the vertex and fragment shader files, 
 * and stores it as a shader resource identified by the provided string key.
 * @param[in] vertexShaderFilePath File path to the vertex shader
 * @param[in] fragmentShaderFilePath File path to the fragment shader
 * @param[in] key Key identifier for the created shader program
 * @return Reference to the created shader. Returns nullptr if the shader
 * creation failed.
 */
ShaderProgram* ResourceManager::CreateShader(const std::string& key, const ShaderCreateInfo& info)
{
    ResourceManager& instance = GetInstance();
    if (instance.m_shaders.find(key) != instance.m_shaders.end())
    {
	return nullptr;
    }

    ShaderProgram* shader = new ShaderProgram();
    shader->Init(info);
    instance.m_shaders[key] = shader;

    return shader;
}

/**
 * @brief Deletes the shader identified by the provided key from the resource manager
 * @param[in] key Key identifying the shader to delete
 */
void ResourceManager::DeleteShader(const std::string& key)
{
    ResourceManager& instance = GetInstance();
    if (instance.m_shaders.find(key) != instance.m_shaders.end())
    {
	delete instance.m_shaders[key];
	instance.m_shaders.erase(key);
    }
}

/**
 * @brief Gets the shader associated with the provided key.
 * @param[in] key Key identifying the shader we are trying to get
 * @return Reference to the shader identified by the provided key. Returns
 * nullptr if it does not exist.
 */
ShaderProgram* ResourceManager::GetShader(const std::string& key)
{
    ResourceManager& instance = GetInstance();
    if (instance.m_shaders.find(key) != instance.m_shaders.end())
    {
	return instance.m_shaders[key];
    }

    return nullptr;
}

/**
 * @brief Creates a texture from the provided file path, and stores it 
 * as a texture resource identified by the provided string key.
 * @param[in] textureFilePath File path to the texture file
 * @param[in] key Key identifier for the created texture
 * @return Reference to the created texture. Returns nullptr if the texture
 * creation failed.
 */
/*Texture* ResourceManager::CreateTexture(const std::string& textureFilePath, const std::string& key)
{
    if (m_textures.find(key) != m_textures.end())
    {
	return nullptr;
    }

    Texture* texture = new Texture();
    texture->CreateFromFile(textureFilePath);
    m_textures[key] = texture;

    return texture;
}*/

/**
 * @brief Deletes the texture identified by the provided key from the resource manager
 * @param[in] key Key identifying the texture to delete
 */
/*void ResourceManager::DeleteTexture(const std::string& key)
{
    if (m_textures.find(key) != m_textures.end())
    {
	delete m_textures[key];
	m_textures.erase(key);
    }
}*/

/**
 * @brief Gets the texture associated with the provided key.
 * @param[in] key Key identifying the texture we are trying to get
 * @return Reference to the texture identified by the provided key. Returns
 * nullptr if it does not exist.
 */
/*Texture* ResourceManager::GetTexture(const std::string& key)
{
    if (m_textures.find(key) != m_textures.end())
    {
	return m_textures[key];
    }

    return nullptr;
}*/


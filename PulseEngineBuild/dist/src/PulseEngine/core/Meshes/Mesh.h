#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp> // Assimp::Importer

#include "Common/common.h"
#include "Common/dllExport.h"

class Skeleton;

/**
 * @brief Structure representing a single vertex in 3D space.
 * Includes position, normal, texture coordinates, bone IDs, and weights.
 */
struct Vertex
{
    glm::vec3 Position;    ///< Vertex position in 3D space.
    glm::vec3 Normal;      ///< Vertex normal vector.
    glm::vec2 TexCoords;   ///< Texture coordinates (UV).
    glm::ivec4 BoneIDs;    ///< IDs of the bones affecting this vertex.
    glm::vec4 Weights;     ///< Weights corresponding to each bone.
};

/**
 * @brief Represents a 3D mesh including geometry, bones, and rendering data.
 * This class handles OpenGL buffer setup and rendering, and can import mesh data via Assimp.
 */
class PULSE_ENGINE_DLL_API Mesh
{
public:
    /**
     * @brief Constructor with raw vertex data (float format).
     * @param vertices Array of vertex data as floats.
     */
    Mesh(const std::vector<float>& vertices);

    /**
     * @brief Constructor with raw vertex data and index data.
     * @param vertices Array of vertex data as floats.
     * @param indices Array of index data.
     */
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    /**
     * @brief Default constructor.
     */
    Mesh();

    /**
     * @brief Destructor. Cleans up GPU buffers.
     */
    ~Mesh();

    /**
     * @brief Draws the mesh using the specified shader program.
     * @param shaderProgram OpenGL shader program ID.
     */
    void Draw(GLuint shaderProgram) const;

    /**
     * @brief Loads mesh data from an Assimp mesh object.
     * @param mesh Assimp mesh pointer.
     * @param scene Assimp scene containing the mesh.
     * @return Pointer to a new Mesh object.
     */
    static Mesh* LoadFromAssimp(const aiMesh* mesh, const aiScene* scene);

    /**
     * @brief Updates the mesh state (for animation or other time-based effects).
     * @param deltaTime Time elapsed since the last update.
     */
    void UpdateMesh(float deltaTime);

    /**
     * @brief Assigns bone influence data to a vertex.
     * @param vertex Vertex to modify.
     * @param boneID ID of the bone.
     * @param weight Influence weight of the bone.
     */
    static void AddBoneDataToVertex(Vertex& vertex, int boneID, float weight);

    /// Pointer to the skeleton associated with this mesh (for skeletal animation).
    Skeleton* skeleton = nullptr;

    /// Assimp importer used for loading mesh files.
    Assimp::Importer* importer;

    // disallow copies
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // allow move
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

private:
    /**
     * @brief Initializes the OpenGL buffers (VAO, VBO, EBO) for rendering.
     */
    void SetupMesh();

    std::vector<Vertex> vertices;         ///< Final vertex data including bone weights.
    std::vector<glm::vec3> normals;       ///< Normal vectors (used before conversion).
    std::vector<glm::vec2> texCoords;     ///< Texture coordinates (used before conversion).
    std::vector<unsigned int> indices;    ///< Index data for rendering (EBO).

    unsigned int VAO; ///< Vertex Array Object.
    unsigned int VBO; ///< Vertex Buffer Object.
    unsigned int EBO; ///< Element Buffer Object.
};

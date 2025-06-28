#include "Mesh.h"
#include "PulseEngine/core/Meshes/SkeletalMesh.h"

Mesh::Mesh(const std::vector<float>& vertices)
{
    SetupMesh();
}

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
{
    SetupMesh();
}

Mesh::Mesh()
{  
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (skeleton)
    {
        delete skeleton;
        skeleton = nullptr;
    }
    if(importer)
    {
        delete importer;
        importer = nullptr;
    }
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Envoie des données des sommets
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Envoie des indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Attribut 0 : Position (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // Attribut 1 : Normal (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Attribut 2 : BoneIDs (ivec4) → glVertexAttribIPointer !
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));

    // Attribut 3 : Weights (vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));

    // Attribut 4 : TexCoords (vec2)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
    std::cout << "Mesh setup done, VAO = " << VAO << std::endl;

}

void Mesh::Draw(GLuint shaderProgram) const
{
    std::cout << "starting to draw the mesh" << std::endl;
    glUseProgram(shaderProgram);

    // // 1. Envoie à l'uniform 'hasSkeleton'
    // std::cout << "getting hasSkeleton uniform location" << std::endl;
    // GLint hasSkeletonLoc = glGetUniformLocation(shaderProgram, "hasSkeleton");
    // std::cout << "hasSkeletonLoc = " << hasSkeletonLoc << std::endl;
    // glUniform1i(hasSkeletonLoc, skeleton != nullptr);
    // std::cout << "set hasSkeleton uniform" << std::endl;

    // // 2. Envoie les bone transforms si le mesh a un squelette
    // if (skeleton)
    // {
    //     std::cout << "mesh has skeleton, getting bone matrices" << std::endl;
    //     const auto& boneMats = skeleton->GetFinalBoneMatrices();
    //     std::cout << "boneMats.size() = " << boneMats.size() << std::endl;

    //     int boneCount = std::min((int)boneMats.size(), 100);
    //     std::cout << "boneCount = " << boneCount << std::endl;
    //     GLint boneMatrixLoc = glGetUniformLocation(shaderProgram, "boneTransforms[0]");
    //     std::cout << "boneMatrixLoc = " << boneMatrixLoc << std::endl;
    //     std::cout << "calling glUniformMatrix4fv" << std::endl;
    //     glUniformMatrix4fv(boneMatrixLoc, boneCount, GL_FALSE, glm::value_ptr(boneMats[0]));
    //     std::cout << "set bone transforms uniform" << std::endl;
    // }

    // 3. Bind VAO et dessiner
    
    std::cout << "VAO value = " << VAO << std::endl;
    std::cout << "binding VAO: " << VAO << std::endl;
    glBindVertexArray(VAO);
    

    std::cout << "indices.size() = " << indices.size() << std::endl;
    std::cout << "vertices.size() = " << vertices.size() << std::endl;

    if (!indices.empty())
    {
        std::cout << "drawing with glDrawElements" << std::endl;
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }
    else
    {
        std::cout << "drawing with glDrawArrays" << std::endl;
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    }

    glBindVertexArray(0);
    std::cout << "finished drawing the mesh" << std::endl;
}

Mesh* Mesh::LoadFromAssimp(const aiMesh* mesh, const aiScene* scene)
{
    Mesh* newMesh = new Mesh();
    std::cout << "chargement du mesh" << std::endl;

    if (mesh->HasBones())
    {
        Skeleton* skel = new Skeleton(mesh, scene);
        newMesh->skeleton = skel;
    }

    try
    {
        // Indices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                newMesh->indices.push_back(face.mIndices[j]);
            }
        }
        std::cout << "face done" << std::endl;

        // Sommets
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex = {};

            // Position
            vertex.Position= glm::vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );

            // Normales
            if (mesh->HasNormals())
            {
                vertex.Normal = glm::vec3(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                );
            }
            else
            {
                vertex.Normal = glm::vec3(0.0f);
            }

            // UVs
            if (mesh->HasTextureCoords(0))
            {
                vertex.TexCoords = glm::vec2(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                );
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f);
            }

            // Initialisation à zéro pour les bones (sera rempli plus tard si bones)
            vertex.BoneIDs = glm::ivec4(0);
            vertex.Weights = glm::vec4(0.0f);

            newMesh->vertices.push_back(vertex);
        }
        if (mesh->HasBones())
        {
            std::map<std::string, int> boneMapping;
            int boneCounter = 0;
        
            for (unsigned int i = 0; i < mesh->mNumBones; ++i)
            {
                std::string boneName(mesh->mBones[i]->mName.C_Str());
            
                int boneID = 0;
                if (boneMapping.find(boneName) == boneMapping.end())
                {
                    boneID = boneCounter;
                    boneMapping[boneName] = boneCounter;
                    boneCounter++;
                }
                else
                {
                    boneID = boneMapping[boneName];
                }
            
                const aiBone* bone = mesh->mBones[i];
                for (unsigned int j = 0; j < bone->mNumWeights; ++j)
                {
                    unsigned int vertexID = bone->mWeights[j].mVertexId;
                    float weight = bone->mWeights[j].mWeight;
                
                    if (vertexID < newMesh->vertices.size())
                    {
                        AddBoneDataToVertex(newMesh->vertices[vertexID], boneID, weight);
                    }
                }
            }
        }

        std::cout << "vertices construits ! " << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception dans le chargement des vertices: " << e.what() << std::endl;
        return nullptr;
    }

    std::cout << "chargement du mesh fini, passage au setup" << std::endl;
    newMesh->SetupMesh();
    std::cout << "setup du mesh fini" << std::endl;

    std::cout << "Nombre d'indices : " << newMesh->indices.size() << std::endl;

    unsigned int maxIndex = 0;
    for (unsigned int index : newMesh->indices)
    {
        if (index > maxIndex) maxIndex = index;
    }
    std::cout << "Indice max : " << maxIndex << ", Nombre de sommets : " << newMesh->vertices.size() << std::endl;
    std::cout << "mesh->HasFaces() ? " << mesh->HasFaces() << std::endl;
    std::cout << "mesh->mNumFaces : " << mesh->mNumFaces << std::endl;

    return newMesh;
}


void Mesh::UpdateMesh(float deltaTime)
{
    if(skeleton)
    {
        skeleton->UpdateSkeleton(deltaTime);
    }
}

void Mesh::AddBoneDataToVertex(Vertex &vertex, int boneID, float weight)
{
    for (int i = 0; i < 4; ++i)
    {
        if (vertex.Weights[i] == 0.0f)
        {
            vertex.BoneIDs[i] = boneID;
            vertex.Weights[i] = weight;
            return;
        }
    }
    // Si on a déjà 4 poids, on ignore les suivants
}
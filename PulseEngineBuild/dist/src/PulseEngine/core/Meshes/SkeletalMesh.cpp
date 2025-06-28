#include "SkeletalMesh.h"
#include <glm/gtc/type_ptr.hpp>

Skeleton::Skeleton(const aiMesh *mesh, const aiScene *scene)
{
    boneCount = 0;

    rootNode = scene->mRootNode;
    if (scene->mNumAnimations > 0)
    {
        animation = scene->mAnimations[0];
    }
    else
    {
        animation = nullptr;
        std::cerr << "[Skeleton] No animations found in scene." << std::endl;
    }
    
    for (unsigned int i = 0; i < mesh->mNumBones; ++i)
    {
        aiBone* aiBone = mesh->mBones[i];
        std::string boneName(aiBone->mName.C_Str());

        AddBone(boneName, aiBone->mOffsetMatrix);
    }
    globalInverseTransform = glm::inverse(ConvertMatrix(scene->mRootNode->mTransformation));
    finalBoneMatrices.resize(100, glm::mat4(1.0f));

}

void Skeleton::AddBone(const std::string &name, const aiMatrix4x4 &offset)
{
    if (boneMapping.find(name) == boneMapping.end()) {
        boneMapping[name] = boneCount++;
        BoneInfo bone;
        bone.offsetMatrix = ConvertMatrix(offset);
        bones.push_back(bone);
    }
}

glm::mat4 Skeleton::GetBoneOffset(const std::string &name) const
{
    auto it = boneMapping.find(name);
    if (it != boneMapping.end()) {
        return bones[it->second].offsetMatrix;
    }
    return glm::mat4(1.0f);
}

void Skeleton::ApplyAnimation(float timeInSeconds, const aiAnimation* animation, const aiNode* node, const glm::mat4& parentTransform)
{
    std::string nodeName(node->mName.C_Str());

    const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);

    glm::mat4 nodeTransform = ConvertMatrix(node->mTransformation);

    if (nodeAnim)
    {
        glm::vec3 scaling = InterpolateScaling(timeInSeconds, nodeAnim);
        glm::quat rotation = InterpolateRotation(timeInSeconds, nodeAnim);
        glm::vec3 position = InterpolatePosition(timeInSeconds, nodeAnim);

        nodeTransform = glm::translate(glm::mat4(1.0f), position)
                      * glm::mat4_cast(rotation)
                      * glm::scale(glm::mat4(1.0f), scaling);
    }

    glm::mat4 globalTransform = parentTransform * nodeTransform;

    if (boneMapping.find(nodeName) != boneMapping.end())
    {
        int index = boneMapping[nodeName];

        bones[index].finalTransform = globalInverseTransform * globalTransform * bones[index].offsetMatrix;
        glm::mat4 offsetMatrix = glm::transpose(bones[index].offsetMatrix);
        finalBoneMatrices[index] = globalTransform * offsetMatrix;

    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ApplyAnimation(timeInSeconds, animation, node->mChildren[i], globalTransform);
    }
}


const aiNodeAnim* Skeleton::FindNodeAnim(const aiAnimation* animation, const std::string& nodeName)
{
    if (!animation)
    {
        return nullptr;
    }


    for (unsigned int i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim* channel = animation->mChannels[i];
        if (!channel) continue;
    
        const aiString& nodeNameString = channel->mNodeName;
        if (nodeNameString.length == 0 || !nodeNameString.C_Str()) continue;
    
        std::string animNodeName = nodeNameString.C_Str();
    
        if (nodeName == animNodeName)
        {
            return channel;
        }
    }
    return nullptr;
}


void Skeleton::UpdateSkeleton(float deltaTime)
{
    
    if (!animation)
    {
        return;
    }

    if (!rootNode)
    {
        return;
    }

    animationTime += deltaTime;
    float durationInTicks = animation->mDuration;
    float ticksPerSecond = animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f;

    float timeInTicks = fmod(animationTime * ticksPerSecond, durationInTicks);
    ApplyAnimation(timeInTicks, animation, rootNode, glm::mat4(1.0f));
}

const std::vector<glm::mat4> &Skeleton::GetFinalBoneMatrices() const
{
    return finalBoneMatrices;
}

glm::mat4 Skeleton::ConvertMatrix(const aiMatrix4x4 &m)
{
    return glm::transpose(glm::make_mat4(&m.a1));
}

glm::vec3 Skeleton::InterpolatePosition(float time, const aiNodeAnim* channel)
{
    if (channel->mNumPositionKeys == 1)
    {
        return glm::vec3(channel->mPositionKeys[0].mValue.x,
                         channel->mPositionKeys[0].mValue.y,
                         channel->mPositionKeys[0].mValue.z);
    }

    for (unsigned int i = 0; i < channel->mNumPositionKeys - 1; ++i)
    {
        if (time < static_cast<float>(channel->mPositionKeys[i + 1].mTime))
        {
            float t1 = static_cast<float>(channel->mPositionKeys[i].mTime);
            float t2 = static_cast<float>(channel->mPositionKeys[i + 1].mTime);
            float factor = (time - t1) / (t2 - t1);

            const aiVector3D& start = channel->mPositionKeys[i].mValue;
            const aiVector3D& end = channel->mPositionKeys[i + 1].mValue;

            aiVector3D delta = end - start;
            aiVector3D result = start + factor * delta;

            return glm::vec3(result.x, result.y, result.z);
        }
    }

    // fallback (dernier keyframe)
    const aiVector3D& last = channel->mPositionKeys[channel->mNumPositionKeys - 1].mValue;
    return glm::vec3(last.x, last.y, last.z);
}


glm::quat Skeleton::InterpolateRotation(float time, const aiNodeAnim* channel)
{
    if (channel->mNumRotationKeys == 1)
    {
        const aiQuaternion& q = channel->mRotationKeys[0].mValue;
        return glm::quat(q.w, q.x, q.y, q.z);
    }

    for (unsigned int i = 0; i < channel->mNumRotationKeys - 1; ++i)
    {
        if (time < static_cast<float>(channel->mRotationKeys[i + 1].mTime))
        {
            float t1 = static_cast<float>(channel->mRotationKeys[i].mTime);
            float t2 = static_cast<float>(channel->mRotationKeys[i + 1].mTime);
            float factor = (time - t1) / (t2 - t1);

            const aiQuaternion& start = channel->mRotationKeys[i].mValue;
            const aiQuaternion& end = channel->mRotationKeys[i + 1].mValue;

            aiQuaternion result;
            aiQuaternion::Interpolate(result, start, end, factor);
            result.Normalize();

            return glm::quat(result.w, result.x, result.y, result.z);
        }
    }

    // fallback
    const aiQuaternion& q = channel->mRotationKeys[channel->mNumRotationKeys - 1].mValue;
    return glm::quat(q.w, q.x, q.y, q.z);
}

glm::vec3 Skeleton::InterpolateScaling(float time, const aiNodeAnim* channel)
{
    if (channel->mNumScalingKeys == 1)
    {
        const aiVector3D& s = channel->mScalingKeys[0].mValue;
        return glm::vec3(s.x, s.y, s.z);
    }

    for (unsigned int i = 0; i < channel->mNumScalingKeys - 1; ++i)
    {
        if (time < static_cast<float>(channel->mScalingKeys[i + 1].mTime))
        {
            float t1 = static_cast<float>(channel->mScalingKeys[i].mTime);
            float t2 = static_cast<float>(channel->mScalingKeys[i + 1].mTime);
            float factor = (time - t1) / (t2 - t1);

            const aiVector3D& start = channel->mScalingKeys[i].mValue;
            const aiVector3D& end = channel->mScalingKeys[i + 1].mValue;

            aiVector3D delta = end - start;
            aiVector3D result = start + factor * delta;

            return glm::vec3(result.x, result.y, result.z);
        }
    }

    // fallback
    const aiVector3D& last = channel->mScalingKeys[channel->mNumScalingKeys - 1].mValue;
    return glm::vec3(last.x, last.y, last.z);
}


#ifndef NDOEMENUREGISTRY_H
#define NODEMENUREGISTRY_H

#include "Common/common.h"
#include "Node.h"
#include <vector>

class NodeMenuRegistry
{
public:
    static NodeMenuRegistry& Get()
    {
        static NodeMenuRegistry instance;
        return instance;
    }

    void AddCategory(const std::string& path, const std::string& nodeName, std::function<void()> onClick)
    {
        // path like "Input/Keyboard" or "Mathematical/Advanced"
        auto parts = SplitPath(path);
        AddRecursive(rootCategories, parts, nodeName, onClick);
    }

    const std::vector<NodeCategory>& GetRootCategories() const
    {
        return rootCategories;
    }

private:
    std::vector<NodeCategory> rootCategories;

    // Helper functions (split path, recursive insertion)
    std::vector<std::string> SplitPath(const std::string& path)
    {
        std::vector<std::string> parts;
        std::stringstream ss(path);
        std::string part;
        while (std::getline(ss, part, '/'))
            parts.push_back(part);
        return parts;
    }

    void AddRecursive(std::vector<NodeCategory>& categories,
                      const std::vector<std::string>& parts,
                      const std::string& nodeName,
                      std::function<void()> onClick,
                      size_t index = 0)
    {
        if (index >= parts.size()) return;

        // Find or create category
        auto it = std::find_if(categories.begin(), categories.end(),
            [&](const NodeCategory& c){ return c.name == parts[index]; });

        if (it == categories.end())
        {
            categories.push_back(NodeCategory{parts[index]});
            it = std::prev(categories.end());
        }

        if (index == parts.size() - 1)
        {
            // Last category level — add the node
            it->nodes.push_back({nodeName, onClick});
        }
        else
        {
            // Go deeper
            AddRecursive(it->subCategories, parts, nodeName, onClick, index + 1);
        }
    }
};


#endif
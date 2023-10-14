#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <format>
#include <map>
#include <algorithm>


struct ResourceLeaf
{
    std::string name;
    int tabulation;
    std::string value{};
};

struct ResourceNode
{
    std::string name;
    int tabulation{1};
    
    std::map<std::string,ResourceNode> children{};
    std::map<std::string,ResourceLeaf> leaves{};
};

ResourceNode operator+=(ResourceNode& node, const std::string& path)
{
    size_t pos = path.find_first_of('/');
    if(pos == std::string::npos)
        node.leaves[path] = {path, node.tabulation + 1};
    else {
        std::string childName = path.substr(0, pos);
        if(node.children.find(childName) == node.children.end())
            node.children[childName] = {childName, node.tabulation + 1};
        node.children[childName] += path.substr(pos + 1);
    }

    return node;
}

std::ostream& operator<<(std::ostream& stream, const ResourceLeaf& leaf)
{
    std::string tabulation(leaf.tabulation, '\t');

    stream << tabulation << leaf.name.substr(0, leaf.name.find_last_of(".")) << " = " << std::endl;
    stream << tabulation << "\t" << leaf.value << ";" << std::endl;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const ResourceNode& node)
{
    std::string tabulation(node.tabulation, '\t');
    stream << tabulation << "namespace " << node.name << " {" << std::endl;

    for (const auto& [name, leaf] : node.leaves)
        stream << leaf;
    for (const auto& [name, child] : node.children)
        stream << child;
    stream << tabulation << "} // " << node.name << std::endl;
    return stream;
}


std::vector<std::string> getFilesInDirectory(const std::string& directory)
{
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if(entry.is_directory()){
            auto directoryFiles = getFilesInDirectory(entry.path().string());
            files.insert(files.end(), directoryFiles.begin(), directoryFiles.end());
        } else if(entry.is_regular_file() && entry.path().extension() != ".gitkeep"){
            files.push_back(entry.path().string());
        }
    }
    return files;
}

void replaceChar(std::string& str, const char& from, const char& to)
{
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos)
    {
        str.replace(pos, 1, 1, to);
        pos++;
    }
}

std::string readFile(const std::filesystem::path& path)
{
    std::string buffer;

    std::ifstream file(path, std::ios::in);

    if(!file.is_open()){
        throw std::runtime_error("Could not open file " + path.string());
    }

    while(!file.eof()){
        std::string line;
        std::getline(file, line);
        buffer += line;

        if(!file.eof()){
            buffer += "\\n";
        }
    }

    return buffer;
}

// std::vector<std::string_view> splitString(std::string_view str, const char& delimiter)
// {
//     std::vector<std::string_view> parts;

//     size_t pos = 0;
//     while((pos = str.find(delimiter)) != std::string::npos)
//     {
//         parts.push_back(str.substr(0, pos));
//         str.remove_prefix(pos + 1);
//     }

//     parts.push_back(str);

//     return parts;
// }

void fillResources(ResourceNode& node, const std::filesystem::path& input_path, std::string path = "")
{
    path += node.name + "/";
    for(auto& [name, child] : node.children)
        fillResources(child, input_path, path);

    for(auto& [name, leaf] : node.leaves){
        const std::string resourceType = path.substr(0, path.find_first_of("/"));
        if(resourceType == "text"){
            leaf.value = "\"" + readFile(input_path.string() + "/" + path + leaf.name) + "\"";
            leaf.name = std::format("{} {} {} = ", "constexpr", "std::string_view", leaf.name);
        }
        else if(resourceType == "textures"){
            leaf.value = std::format("make_texture(\"{}\")", input_path.string() + "/" + path + leaf.name);
            leaf.name = std::format("{} {} {} = ", "const", "sf::Texture", leaf.name);
        }
    }
}

void outputResourceIds(std::ostream& stream, const std::vector<std::string>& files, const std::filesystem::path& input_path)
{
    std::array<ResourceNode, 15> resources{
        ResourceNode{"animations"},
        ResourceNode{"config"},
        ResourceNode{"fonts"},
        ResourceNode{"icons"},
        ResourceNode{"levels"},
        ResourceNode{"materials"},
        ResourceNode{"models"},
        ResourceNode{"music"},
        ResourceNode{"prefabs"},
        ResourceNode{"scripts"},
        ResourceNode{"shaders"},
        ResourceNode{"sounds"},
        ResourceNode{"text"},
        ResourceNode{"textures"},
        ResourceNode{"ui"},
    };

    for (const auto& file : files)
    {
        const std::string relativePath = file.substr(input_path.string().length() + 1);
        const std::string resourceType = relativePath.substr(0, relativePath.find_first_of("/"));
        
        auto it = std::find_if(resources.begin(), resources.end(), [&](const ResourceNode& node){
            return node.name == resourceType;
        });
        if(it == resources.end())
            throw std::runtime_error("Unknown resource type " + resourceType);

        *it += relativePath.substr(resourceType.length() + 1);
    }

    for (auto& rootNode : resources)
    {
        if(rootNode.children.empty() && rootNode.leaves.empty())
            continue;

        fillResources(rootNode, input_path);
    }

    const std::string time = std::format("{:%Y-%m-%d %H:%M}", std::chrono::system_clock::now());

    stream << "// This file is generated by ResourceLoader, do not edit it manually" << std::endl;
    stream << "// Generated from " << input_path << std::endl;
    stream << "// Generated on " << time << std::endl;

    stream << std::endl;
    stream << "#pragma once" << std::endl;

    stream << "#include <stdexcept>" << std::endl;
    stream << "#include <string_view>" << std::endl;
    stream << "#include <SFML/Graphics/Texture.hpp>" << std::endl;

    stream << std::endl;
    stream << "namespace Resources {" << std::endl;
    stream << "\tsf::Texture make_texture(const std::string& path)\n"
            "\t{\n"
            "\t\tsf::Texture texture;\n"
            "\t\tif(!texture.loadFromFile(path))\n"
            "\t\tthrow std::runtime_error(\"Failed to load texture \" + path);\n"
            "\t\treturn texture;\n"
            "\t}\n";

    stream << std::endl;
    for (const auto& rootNode : resources)
    {
        if(rootNode.children.empty() && rootNode.leaves.empty())
            continue;

        stream << rootNode;
    }

    stream << "} // namespace Resources" << std::endl;
}

int main(int argc, char** argv)
{
    if(argc < 3){
        std::cerr << "Usage: ResourceLoader <resources-path> <output-path>" << std::endl;
        return 1;
    }

    auto input_path = std::filesystem::path(argv[1]);
    std::vector<std::string> files = getFilesInDirectory(input_path.string());

    auto output_path = std::filesystem::path(argv[2]);
    std::ofstream output_file = std::ofstream(output_path.append("Resources.build.hpp"), std::ios::out);

    outputResourceIds(output_file, files, input_path);
    return 0;
}

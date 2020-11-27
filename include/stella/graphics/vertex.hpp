#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>

namespace stella
{
namespace graphics
{
  struct Vertex
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;

    bool operator== (const Vertex& other) const;
  };

} // namespace graphics
} // namespace stella

namespace std
{
template<>
struct hash<stella::graphics::Vertex>
{
  size_t operator() (const stella::graphics::Vertex& vertex) const { return ((hash<glm::vec3>() (vertex.position) ^ (hash<glm::vec2>() (vertex.texcoords) << 1)) >> 1); }
};
} // namespace std

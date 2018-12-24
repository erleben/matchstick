#ifndef GL3_GLM_H
#define GL3_GLM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>          // glm::mat4
#include <glm/gtc/type_ptr.hpp>    // glm::value_ptr
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>   // glm::scale
#include <glm/gtx/string_cast.hpp> // glm::to_string

inline std::ostream& operator<<(std::ostream& out, glm::mat4 const & g)
{
  return out << glm::to_string(g);
}

inline std::ostream& operator<<(std::ostream& out, glm::vec3 const & g)
{
  return out << glm::to_string(g);
}

inline std::ostream& operator<<(std::ostream& out, glm::vec4 const & g)
{
  return out << glm::to_string(g);
}

// GL3_GLM_H
#endif

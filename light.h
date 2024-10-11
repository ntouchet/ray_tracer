#pragma once
#include <glm/glm.hpp>

// Light class
class pointSource
{
public:
    pointSource() {}

    pointSource(glm::vec3 position, glm::vec3 color) : pos(position), col(color) {}

    const glm::vec3 position() const { return pos; }
    const glm::vec3 color() const { return col; }

private:
    glm::vec3 pos;
    glm::vec3 col;
};

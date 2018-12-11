#include "impshape.h"
#include "CS123SceneData.h"

/**
 * @brief impshape::impshape
 * Parent class for implicit shapes. Contains variables m_t and m_normal for each implicit shape to update while determining point of nearest
 * intersection, as well as the intersects method, which both determines whether there is an intersection, and stores the nearest point of
 * intersection with m_t and m_normal.
 */
impshape::impshape() :
    m_t(1000.f),
    m_normal(glm::vec4(0.f))
{

}

bool impshape::intersects(glm::vec4 p, glm::vec4 d) {

}

glm::vec2 impshape::getuv(glm::vec4 intersection) {

}

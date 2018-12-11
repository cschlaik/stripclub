#ifndef IMPSHAPE_H
#define IMPSHAPE_H
#include "CS123SceneData.h"


class impshape
{
public:
    impshape();
    virtual bool intersects(glm::vec4 p, glm::vec4 d);
    virtual glm::vec2 getuv(glm::vec4 intersection);
    float m_t;
    glm::vec4 m_normal;
};

#endif // IMPSHAPE_H

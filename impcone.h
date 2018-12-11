#ifndef IMPCONE_H
#define IMPCONE_H
#include "CS123SceneData.h"
#include "impshape.h"


class impcone:
        public impshape
{
public:
    impcone();
    bool intersects(glm::vec4 p, glm::vec4 d);
    bool intersectsBody(glm::vec4 p, glm::vec4 d);
    bool intersectsCap(glm::vec4 p, glm::vec4 d);
    glm::vec2 getuv(glm::vec4 intersection);
};

#endif // IMPCONE_H

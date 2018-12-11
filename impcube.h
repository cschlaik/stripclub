#ifndef IMPCUBE_H
#define IMPCUBE_H
#include "CS123SceneData.h"
#include "impshape.h"


class impcube:
        public impshape
{
public:
    impcube();
    bool intersects(glm::vec4 p, glm::vec4 d);
    bool intersectsCap(glm::vec4 p, glm::vec4 d, int i);
    glm::vec2 getuv(glm::vec4 intersection);
};

#endif // IMPCUBE_H

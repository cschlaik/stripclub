#ifndef IMPSPHERE_H
#define IMPSPHERE_H
#include "CS123SceneData.h"
#include "impshape.h"


class impsphere
        : public impshape
{
public:
    impsphere();
    bool intersects(glm::vec4 p, glm::vec4 d);
    glm::vec2 getuv(glm::vec4 intersection);
};

#endif // IMPSPHERE_H

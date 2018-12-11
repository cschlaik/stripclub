#include "impcube.h"
#include "CS123SceneData.h"
#include "impshape.h"

impcube::impcube()
    : impshape()
{

}

bool impcube::intersects(glm::vec4 p, glm::vec4 d) {
    // Check for intersections of ray with cube sides
    if ((intersectsCap(p, d, 0))||(intersectsCap(p, d, 1))||(intersectsCap(p, d, 2))) {
        return true;
    }
    else {
        return false;
    }
}

bool impcube::intersectsCap(glm::vec4 p, glm::vec4 d, int i) {
    // Determine nearer of two intersection points with planes i = .5, i = -.5 where i stands in for x, y, or z
    float t1 = (-.5 - p[i])/d[i];
    float t2 = (.5 - p[i])/d[i];
    float t3 = glm::min(t1, t2);
    glm::vec4 ray = p + d*t3;
    // Uses modulo function to check that the intersection is within the bounds of the other two axes (I'm pretty proud of this idea!)
    if ((ray[(i+1)%3] <= .5)&&(ray[(i+2)%3] <= .5)&&(ray[(i+1)%3] >= -.5)&&(ray[(i+2)%3] >= -.5)) {
        // Checks if intersection is closest thus far for the shape
        if (t3 < m_t) {
            // Updates m_t and corresponding m_normal
            m_t = t3;
            // Initializes m_normal as 0 vector
            m_normal = glm::vec4(0.f);
            // If the plane in question is the positive one, its corresponding normal coordinate is 1; otherwise, it is -1.
            if (ray[i] > 0) {
                m_normal[i] = 1.f;
            }
            else {
                m_normal[i] = -1.f;
            }
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

glm::vec2 impcube::getuv(glm::vec4 intersection) {
    glm::vec2 uv;
    // Textures each face such that it is oriented upright and not a mirrored image.
    if (intersection.x > .5 - FLT_EPSILON) {
        uv.s = .5 - intersection.z;
        uv.t = .5 - intersection.y;
    }
    else if (intersection.x < -.5 + FLT_EPSILON) {
        uv.s = .5 + intersection.z;
        uv.t = .5 - intersection.y;
    }
    else if (intersection.y > .5 - FLT_EPSILON) {
        uv.t = .5 + intersection.z;
        uv.s = .5 + intersection.x;
    }
    else if (intersection.y <  -.5 + FLT_EPSILON) {
        uv.t = .5 - intersection.z;
        uv.s = .5 + intersection.x;
    }
    else if (intersection.z > .5 - FLT_EPSILON) {
        uv.s = .5 + intersection.x;
        uv.t = .5 - intersection.y;
    }
    else if (intersection.z < -.5 + FLT_EPSILON) {
        uv.s = .5 - intersection.x;
        uv.t = .5 - intersection.y;
    }
    return uv;
}

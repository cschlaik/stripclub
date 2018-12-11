#include "impsphere.h"
#include "CS123SceneData.h"
#include "impshape.h"

impsphere::impsphere()
    : impshape()
{
}

bool impsphere::intersects(glm::vec4 p, glm::vec4 d) {
    // Use quadratic equation to solve for intersections with body
    float a = glm::dot(d, d);
    float b = 2*(glm::dot(d, p));
    float c = glm::dot(p, p) - 1.25;
    float disc = pow(b, 2) - 4*a*c;
    // If discriminant is negative, there is no intersection
    if (disc >= 0) {
        // If the nearer intersection is negative, the shape is behind or containing the camera, and is not rendered
        if ((-b - sqrt(disc))/(2*a) >= 0) {
            // This is necessarily the nearest intersection with the sphere; no need to check
            m_t = (-b - sqrt(disc))/(2*a);
            // Normal corresponds to intersection point, but with 0 in homogenous coordinate
            glm::vec4 normal_temp = glm::vec4(p + d*m_t);
            normal_temp[3] = 0.f;
            m_normal = glm::normalize(normal_temp);
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

glm::vec2 impsphere::getuv(glm::vec4 intersection) {
    glm::vec2 uv;
    // Determine where along the unwrapped sphere the intersection would occur, and express this within a unit square.
    float theta = atan2(intersection.z, intersection.x);
    if (theta >= 0) {
        uv.s = 1 - theta/(2*3.1415);
    }
    else {
        uv.s = - theta/(2*3.1415);
    }
    uv.t = .5 - intersection.y;
    float psi = glm::asin(intersection.y/.5);
    uv.t = psi/3.1415 + .5;
    return uv;
}

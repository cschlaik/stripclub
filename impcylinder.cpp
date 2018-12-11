#include "impcylinder.h"
#include "CS123SceneData.h"
#include "impshape.h"
#include <iostream>

impcylinder::impcylinder()
    : impshape()
{

}

bool impcylinder::intersects(glm::vec4 p, glm::vec4 d) {
    // Check for intersections of ray with body or sides
    if (intersectsBody(p, d)||(intersectsCap(p,d))) {
        return true;
    }
    else {
        return false;
    }
}

bool impcylinder::intersectsBody(glm::vec4 p, glm::vec4 d) {
    // Use quadratic equation to solve for intersections with body
    float a = pow(d.x, 2) + pow(d.z, 2);
    float b = 2*p.x*d.x + 2*p.z*d.z;
    float c = pow(p.x, 2) + pow(p.z, 2) - .25;
    float disc = pow(b, 2) - 4*a*c;
    // If discriminant is negative, there is no intersection
    if (disc < 0) {
        return false;
    }
    // Determine the two rational roots
    float t1 = (-b - sqrt(disc))/(2*a);
    float t2 = (-b + sqrt(disc))/(2*a);
    // If either root is negative, part of the shape is behind the camera, thus I choose not to render it
    if ((t1 >= 0)&&(t2 >= 0)) {
        // t1 will always be the smaller t, as the discriminant is a positive number
        glm::vec4 t1int = p + d*t1;
        // Check for intersection within unit cylinder bounds
        if ((t1int.y <= .5)&&(t1int.y >= -.5)) {
            // Check if intersection is closest one thus far for the shape
            if (t1 < m_t) {
                // Update m_t and its corresponding m_normal
                m_t = t1;
                m_normal = glm::normalize(glm::vec4((p+d*m_t).x, 0.f, (p+d*m_t).z, 0.f));
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
    else {
        return false;
    }
}

bool impcylinder::intersectsCap(glm::vec4 p, glm::vec4 d) {
    // Determine intersection point with infinite planes y = -.5, y = .5
    float t3 = (-.5 - p.y)/d.y;
    float t4 = (.5 - p.y)/d.y;
    // Determine the nearer of the two intersection points (the farther cap is necessarily blocked by body)
    float t5 = glm::min(t3, t4);
    glm::vec4 ray = p + d*t5;
    // Check if the intersection point is within the bounds
    if (pow(ray.x, 2) + pow(ray.z, 2) <= pow(.5, 2)) {
        // Check if intersectino is closest one thus far for the shape
        if (t5 < m_t) {
            // Update m_t and its corresponding normal
            m_t = t5;
            if (ray.y > 0) {
                m_normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
            }
            else {
                m_normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
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

glm::vec2 impcylinder::getuv(glm::vec4 intersection) {
    glm::vec2 uv;
    // If the intersection is with a cap, texture it like the plane the cap resides on.
    if (intersection.y >  .5 - FLT_EPSILON) {
        uv.t = .5 + intersection.z;
        uv.s = .5 + intersection.x;
    }
    else if (intersection.y < - .5 + FLT_EPSILON) {
        uv.t = .5 - intersection.z;
        uv.s = .5 + intersection.x;
    }
    // Determine where along the unwrapped cylinder body the intersection would occur, and express this within a unit square.
    else {
        float theta = atan2(intersection.z, intersection.x);
        if (theta >= 0) {
            uv.s = 1 - theta/(2*3.1415);
        }
        else {
            uv.s = - theta/(2*3.1415);
        }
        uv.t = .5 - intersection.y;
    }
    return uv;
}

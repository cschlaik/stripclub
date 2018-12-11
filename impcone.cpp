#include "impcone.h"
#include "CS123SceneData.h"
#include "impshape.h"
#include <iostream>
#include <math.h>

impcone::impcone()
    : impshape()
{

}

bool impcone::intersects(glm::vec4 p, glm::vec4 d) {
    // Check for intersections of ray with body or sides
    if (intersectsBody(p, d)||(intersectsCap(p,d))) {
        return true;
    }
    else {
        return false;
    }
}

bool impcone::intersectsBody(glm::vec4 p, glm::vec4 d) {
    // Use quadratic equation to solve for intersections with body
    float a = pow(d.x, 2.f) + pow(d.z, 2.f) - .25*pow(d.y, 2.f);
    float b = 2*p.x*d.x + 2*p.z*d.z - .5*p.y*d.y + .25*d.y;
    float c = pow(p.x, 2.f) + pow(p.z, 2.f) - .25*pow(p.y, 2.f) + .25*p.y - 0.0625;
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
        // Check for intersection within unit cone bounds
        if ((t1int.y <= .5)&&(t1int.y >= -.5)) {
            // Check if intersection is closest one thus far for the shape
            if (t1 < m_t) {
                // Update m_t and its corresponding m_normal
                m_t = t1;
                glm::vec4 inter = (p + m_t*d);
                m_normal = glm::normalize(glm::vec4(inter.x, glm::sqrt(glm::pow(inter.x, 2.f)
                                          + glm::pow(inter.z, 2.f))*glm::tan(M_PI/6), inter.z, 0.f));
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

bool impcone::intersectsCap(glm::vec4 p, glm::vec4 d) {
    // Determine intersection point with infinite plane y = -.5
    float t3 = (-.5 - p.y)/d.y;
    // Check whether within bounds of the cap
    glm::vec4 ray = p + d*t3;
    if (pow(ray.x, 2) + pow(ray.z, 2) <= pow(.5, 2)) {
        // Check if intersection is closest one thus far for the shape
        if (t3 < m_t) {
            // Update m_t and its corresponding m_normal
            m_t = t3;
            m_normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
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


glm::vec2 impcone::getuv(glm::vec4 intersection) {
    glm::vec2 uv;
    // If the intersection is with a cap, texture it like the plane the cap resides on.
    if (intersection.y < -.5 + FLT_EPSILON) {
        uv.t = .5 - intersection.z;
        uv.s = .5 + intersection.x;
    }
    // Determine where along the unwrapped cone body the intersection would occur, and express this within a unit square.
    else {
        // Fix s when intersection approaches cone tip
        if (intersection.y > .5 - 100*FLT_EPSILON) {
            uv.s = .5;
        }
        else {
            uv.s = fabs(glm::atan(intersection.x/intersection.z)/2*3.1415);
        }
        uv.t = .5 + intersection.y;
    }
    return uv;
}

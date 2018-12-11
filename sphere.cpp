#include "sphere.h"
#include <memory>
#include "Settings.h"
#include <SupportCanvas3D.h>

/**
 * A subclass of Shape. Fills m_vec with the appropriate math for a tessellating cone with a call to makeVerts() in the constructor.
 * Then uses Shape's setUpVBO method to build the VBO/VAO appropriately.
 */
Sphere::Sphere()
    : Shape()
{
    makeVerts();
    setUpVBO();
}

Sphere::~Sphere()
{
}

void Sphere::makeVerts() {
    // clears m_vec, as the same vector is used for every VBO/VAO.
    m_vec.clear();
    int p1 = glm::max(settings.shapeParameter1, 2);
    int p2 = glm::max(settings.shapeParameter2, 3);
    float theta = 2*3.1415/p2;
    float phi = 3.1415/p1;
    // builds the entire sphere in rows of strips from top to bottom. No need for degeneratives, as each subsequent row begins
    // with the last vertex from the prior row.
    for (float y = 0; y < p1; y++) {
        for (float i = 0; i <= p2; i++) {
            m_vec.push_back(.5*glm::sin(y*phi)*glm::cos(i*theta)); m_vec.push_back(.5*cos(y*phi)); m_vec.push_back(-.5*glm::sin(y*phi)*glm::sin(i*theta));
            m_vec.push_back(.5*glm::sin(y*phi)*glm::cos(i*theta)); m_vec.push_back(.5*cos(y*phi)); m_vec.push_back(-.5*glm::sin(y*phi)*glm::sin(i*theta));
            m_vec.push_back(.5*glm::sin((y+1)*phi)*glm::cos(i*theta)); m_vec.push_back(.5*cos((y+1)*phi)); m_vec.push_back(-.5*glm::sin((y+1)*phi)*glm::sin(i*theta));
            m_vec.push_back(.5*glm::sin((y+1)*phi)*glm::cos(i*theta)); m_vec.push_back(.5*cos((y+1)*phi)); m_vec.push_back(-.5*glm::sin((y+1)*phi)*glm::sin(i*theta));
        }
    }
}

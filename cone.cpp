#include "cone.h"
#include <memory>
#include "Settings.h"
#include <SupportCanvas3D.h>

/**
 * A subclass of Shape. Fills m_vec with the appropriate math for a tessellating cone with a call to makeVerts() in the constructor.
 * Then uses Shape's setUpVBO method to build the VBO/VAO appropriately.
 */
Cone::Cone()
    : Shape()
{
    makeVerts();
    setUpVBO();

}

Cone::~Cone()
{
}

void Cone::makeVerts() {
    int p1 = settings.shapeParameter1;
    int p2 = glm::max(settings.shapeParameter2, 3);
    float theta = 2*3.1415/p2;
    // clears m_vec, as the same vector is used for every VBO/VAO.
    m_vec.clear();
    // builds the cone's sides.
    for (float y = 0; y < p1; y++) {
        for (float i = 0; i <= p2; i++) {
            m_vec.push_back(.5*glm::cos(i*theta)*y/p1); m_vec.push_back(.5-y/p1); m_vec.push_back(-.5*glm::sin(i*theta)*y/p1);
            m_vec.push_back(glm::cos(i*theta)); m_vec.push_back(.5); m_vec.push_back(-glm::sin(i*theta));
            m_vec.push_back(.5*glm::cos(i*theta)*(y+1)/p1); m_vec.push_back(.5-(y+1)/p1); m_vec.push_back(-.5*glm::sin(i*theta)*(y+1)/p1);
            m_vec.push_back(glm::cos(i*theta)); m_vec.push_back(.5); m_vec.push_back(-glm::sin(i*theta));
        }
    }
    // builds the cone's base. Same math used to build cylinder bases.
    for (float y = 0; y < p1; y++) {
        for (float i = 0; i <= p2; i++) {
            m_vec.push_back(.5*glm::cos(i*theta)*(p1-y)/p1); m_vec.push_back(-.5); m_vec.push_back(-.5*glm::sin(i*theta)*(p1-y)/p1);
            m_vec.push_back(0); m_vec.push_back(-1); m_vec.push_back(0);
            m_vec.push_back(.5*glm::cos(i*theta)*(p1-(y+1))/p1); m_vec.push_back(-.5); m_vec.push_back(-.5*glm::sin(i*theta)*(p1-(y+1))/p1);
            m_vec.push_back(0); m_vec.push_back(-1); m_vec.push_back(0);
        }
    }
}

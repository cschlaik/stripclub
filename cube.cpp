#include "cube.h"
#include <memory>
#include "Settings.h"

/**
 * A subclass of Shape. Fills m_vec with the appropriate math for a tessellating cone with a call to makeVerts() in the constructor.
 * Then uses Shape's setUpVBO method to build the VBO/VAO appropriately.
 */
Cube::Cube()
    : Shape()
{
    makeVerts();
    setUpVBO();
}

Cube::~Cube()
{
}

void Cube::makeVerts() {
    // clears m_vec, as the same vector is used for every VBO/VAO.
    m_vec.clear();
    int p1 = settings.shapeParameter1;
    // builds cube faces on the XY and YZ planes.
    for (float y = 0; y < p1; y++) {
        for (float x = 0; x <= p1; x++) {
            m_vec.push_back(-.5+x/p1); m_vec.push_back(.5-y/p1); m_vec.push_back(.5);
            m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(1);
            m_vec.push_back(-.5+x/p1); m_vec.push_back(.5-(y+1)/p1); m_vec.push_back(.5);
            m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(1);
        }
        for (float z = 0; z <= p1; z++) {
            m_vec.push_back(.5); m_vec.push_back(.5-y/p1); m_vec.push_back(.5-z/p1);
            m_vec.push_back(1); m_vec.push_back(0); m_vec.push_back(0);
            m_vec.push_back(.5); m_vec.push_back(.5-(y+1)/p1); m_vec.push_back(.5-z/p1);
            m_vec.push_back(1); m_vec.push_back(0); m_vec.push_back(0);
        }
        for (float x = 0; x <= p1; x++) {
            m_vec.push_back(.5-x/p1); m_vec.push_back(.5-y/p1); m_vec.push_back(-.5);
            m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(-1);
            m_vec.push_back(.5-x/p1); m_vec.push_back(.5-(y+1)/p1); m_vec.push_back(-.5);
            m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(-1);
        }
        for (float z = 0; z <= p1; z++) {
            m_vec.push_back(-.5); m_vec.push_back(.5-y/p1); m_vec.push_back(-.5+z/p1);
            m_vec.push_back(-1); m_vec.push_back(0); m_vec.push_back(0);
            m_vec.push_back(-.5); m_vec.push_back(.5-(y+1)/p1); m_vec.push_back(-.5+z/p1);
            m_vec.push_back(-1); m_vec.push_back(0); m_vec.push_back(0);
        }
    }
    // builds -XZ plane, using denegerative triangles to remove artifacts between rows.
    for (float z = 0; z < p1; z++) {
        for (float x = 0; x <= p1; x++) {
            m_vec.push_back(-.5+x/p1); m_vec.push_back(-.5); m_vec.push_back(.5-z/p1);
            m_vec.push_back(0); m_vec.push_back(-1); m_vec.push_back(0);
            m_vec.push_back(-.5+x/p1); m_vec.push_back(-.5); m_vec.push_back(.5-(z+1)/p1);
            m_vec.push_back(0); m_vec.push_back(-1); m_vec.push_back(0);
        }
        m_vec.push_back(.5); m_vec.push_back(-.5); m_vec.push_back(.5-(z+1)/p1);
        m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(0);
        m_vec.push_back(-.5); m_vec.push_back(-.5); m_vec.push_back(.5-(z+1)/p1);
        m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(0);
    }
    // uses degenerative triangles between -XZ and +XZ planes to prevent internal artifacts.
    m_vec.push_back(-.5); m_vec.push_back(-.5); m_vec.push_back(-.5);
    m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(0);
    m_vec.push_back(-.5); m_vec.push_back(.5); m_vec.push_back(-.5);
    m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(0);
    // builds +XZ plane, using denegerative triangles to remove artifacts between rows.
    for (float z = 0; z < p1; z++) {
        for (float x = 0; x <= p1; x++) {
            m_vec.push_back(-.5+x/p1); m_vec.push_back(.5); m_vec.push_back(-.5+z/p1);
            m_vec.push_back(0); m_vec.push_back(1); m_vec.push_back(0);
            m_vec.push_back(-.5+x/p1); m_vec.push_back(.5); m_vec.push_back(-.5+(z+1)/p1);
            m_vec.push_back(0); m_vec.push_back(1); m_vec.push_back(0);
        }
        m_vec.push_back(.5); m_vec.push_back(.5); m_vec.push_back(-.5+(z+1)/p1);
        m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(0);
        m_vec.push_back(-.5); m_vec.push_back(.5); m_vec.push_back(-.5+(z+1)/p1);
        m_vec.push_back(0); m_vec.push_back(0); m_vec.push_back(0);
    }
}

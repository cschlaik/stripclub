#include "shape.h"
#include <SupportCanvas3D.h>
#include "Settings.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "gl/shaders/Shader.h"

/**
 * @brief Shape::Shape
 * Shape is a subclass of OpenGLShape, a class I imported from Lab 2. Shape is the super class for all four shapes; its constructor
 * initializes a vector for vertex and normal data, much like the Brush constructor initializes a mask vector. Each subclass fills in
 * m_vec with the appropriate math, and then calls setUpVBO to use the methods from OpenGLShape that are shared by all shapes.
 */
Shape::Shape()
    : OpenGLShape(),
      m_vec(std::vector<float>())
{
}

/*Shape::~Shape()
{
}*/

void Shape::setUpVBO() {
    // Each vertex has 3 position coordinates and 3 normal coordinates, thus m_vec's size divided by 6 is the number of verts.
    setVertexData(m_vec.data(), m_vec.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, m_vec.size()/6);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    // Normals are interwoven with vertices in m_vec, thus their offset is 3 floats * 4 bytes/float = 12 bytes.
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
}

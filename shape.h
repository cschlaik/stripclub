#ifndef SHAPE_H
#define SHAPE_H
#include "openglshape.h"


class Shape
    : public OpenGLShape
{
public:
    Shape();
    //virtual ~Shape();
    void setUpVBO();

protected:
    std::vector<float> m_vec;
};

#endif // SHAPE_H

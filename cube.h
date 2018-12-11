#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include "shape.h"

class Cube
    : public Shape
{
public:
    Cube();
    virtual ~Cube();
protected:
    void makeVerts();
};

#endif // CUBE_H

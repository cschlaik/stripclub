#ifndef CYLINDER_H
#define CYLINDER_H

#include <memory>
#include "shape.h"


class Cylinder
        : public Shape
{
public:
    Cylinder();
    virtual ~Cylinder();
protected:
    void makeVerts();
};

#endif // CYLINDER_H

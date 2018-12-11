#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include "shape.h"

class Sphere
        : public Shape
{
public:
    Sphere();
    virtual ~Sphere();
protected:
    void makeVerts();
};

#endif // SPHERE_H

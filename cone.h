#ifndef CONE_H
#define CONE_H

#include <memory>
#include "shape.h"

class Cone
        : public Shape
{
public:
    Cone();
    virtual ~Cone();
protected:
    void makeVerts();
};

#endif // CONE_H

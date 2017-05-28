#ifndef COLLISION_OBJECT_H
#define COLLISION_OBJECT_H

#include "../lib/headers/billmaterialpoint.h"
#include "../lib/headers/billvector.h"

class CollisionObject {
public:
    bill::vector normalizedVector;
    bill::vector position;
    const double SUPPRESION = 0.4;
    const double FRICTION = 0.7;

    CollisionObject(){};
    CollisionObject(const bill::vector &vector1, const bill::vector &vector2);
    virtual bool collide(std::shared_ptr<bill::BillMaterialPoint> &mp);
    virtual void draw();
    virtual void slowVelocities(bill::vector &vel1, bill::vector &vel2);
    void calculateVels(std::shared_ptr<bill::BillMaterialPoint> &billMaterialPoint);
};

#endif //COLLISION_OBJECT_H

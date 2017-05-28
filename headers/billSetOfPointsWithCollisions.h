#ifndef EX4_BILL_SET_OF_POINTS_WITH_COLLISIONS_H
#define EX4_BILL_SET_OF_POINTS_WITH_COLLISIONS_H

#include "../lib/headers/billmaterialpoint.h"
#include "collisionObject.h"

class BillSetOfPointsWithCollisions : public bill::BillSetOfPoints {
public:

    std::vector<std::shared_ptr<CollisionObject>> collisionObjects;

    void Draw();
    void addObject(std::shared_ptr<CollisionObject> o);
    void Act();
};

#endif //EX4_BILL_SET_OF_POINTS_WITH_COLLISIONS_H

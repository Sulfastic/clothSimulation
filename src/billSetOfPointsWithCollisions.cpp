#include "../headers/billSetOfPointsWithCollisions.h"

void BillSetOfPointsWithCollisions::Draw() {
    bill::BillSetOfPoints::Draw();

    for (auto &obj : collisionObjects) {
        obj->draw();
    }
};

void BillSetOfPointsWithCollisions::addObject(std::shared_ptr<CollisionObject> o) {
    collisionObjects.push_back(o);
}

void BillSetOfPointsWithCollisions::Act() {
    for (auto &point : points) {
        for (auto &obj : collisionObjects) {
            obj->collide(point);
        }
    }
}



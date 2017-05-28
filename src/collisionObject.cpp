#include "../headers/collisionObject.h"

CollisionObject::CollisionObject(const bill::vector &vector1, const bill::vector &vector2) {
    this->normalizedVector = vector1;
    this->normalizedVector.normalize();
    this->position = vector2;
}

bool CollisionObject::collide(std::shared_ptr<bill::BillMaterialPoint> &billMaterialPoint) {
    double collisionFrame = this->normalizedVector * (billMaterialPoint->position() - this->position);

    if (collisionFrame <= 0.0) {

        bill::vector futurePosition = billMaterialPoint->position() - collisionFrame * this->normalizedVector;
        billMaterialPoint->set_future_position(futurePosition);

        calculateVels(billMaterialPoint);

        return true;
    } else {
        return false;
    }
}

void CollisionObject::calculateVels(std::shared_ptr<bill::BillMaterialPoint> &billMaterialPoint) {
    bill::vector VComponentNorm = this->normalizedVector * (this->normalizedVector * billMaterialPoint->velocity());
    bill::vector VComponentSphere = billMaterialPoint->velocity() - VComponentNorm;

    slowVelocities(VComponentNorm, VComponentSphere);

    billMaterialPoint->set_future_velocity(VComponentSphere - VComponentNorm);
}

void CollisionObject::slowVelocities(bill::vector &vel1, bill::vector &vel2) {
    vel1 *= this->SUPPRESION;
    vel2 *= this->FRICTION;
}

void CollisionObject::draw() {
    glPushMatrix();
    glLineWidth(2.5);
    glColor3f(0.0, 0.5, 0.0);
    glBegin(GL_LINES);
    glVertex3f(normalizedVector[0], normalizedVector[1], normalizedVector[2]);
    glVertex3f(position[0], position[1], position[2]);
    glEnd();
    glPopMatrix();
}

#include "../headers/triangle.h"

Triangle::Triangle(const bill::vector &vector1, const bill::vector &vector2, const bill::vector &vector3)
        : Triangle(vector1, vector2, vector3, false) {}

Triangle::Triangle(const bill::vector &vector1, const bill::vector &vector2, const bill::vector vector3,
                   bool surface) {
    this->vec1 = vector1;
    this->vec2 = vector2;
    this->vec3 = vector3;
    this->isClosed = surface;

    this->perpLength = this->calculatePerpLength(this->normalizedVector, this->vec1, this->vec2, this->vec3);
    this->normalizedVector.normalize();
}

bool Triangle::collide(std::shared_ptr<bill::BillMaterialPoint> &billMaterialPoint) {
    double collisionFrame = this->normalizedVector * (billMaterialPoint->position() - this->vec1);

    if (fabs(collisionFrame) > 0.01) {
        return false;
    }

    bill::vector currentPosition = this->normalizedVector * collisionFrame;
    bill::vector futurePosition = billMaterialPoint->position() - collisionFrame * this->normalizedVector;

    if (insideTriangle(futurePosition)) {
        billMaterialPoint->set_future_position(billMaterialPoint->position() + currentPosition);

        CollisionObject::calculateVels(billMaterialPoint);

        return true;
    }

    return false;
}

void Triangle::slowVelocities(bill::vector &vel1, bill::vector &vel2) {
    vel1 *= this->SUPPRESION;
}

double Triangle::calculatePerpLength(bill::vector &changeableVector, const bill::vector &vec1, const bill::vector &vec2,
                                     const bill::vector &vec3) {
    bill::vector left = vec2 - vec1;
    bill::vector right = vec3 - vec1;
    changeableVector[0] = (left[1] * right[2]) - (left[2] * right[1]);
    changeableVector[1] = (left[2] * right[0]) - (left[0] * right[2]);
    changeableVector[2] = (left[0] * right[1]) - (left[1] * right[0]);

    return bill::vector::norm(changeableVector);
}

double Triangle::calculatePerpLength(const bill::vector &vec1, const bill::vector &vec2, const bill::vector &vec3) {
    bill::vector vector = {0,0,0};
    return this->calculatePerpLength(vector, vec1, vec2, vec3);
}

bool Triangle::insideTriangle(const bill::vector &point) {
    double distance23 = calculatePerpLength(point, this->vec2, this->vec3) / this->perpLength;
    bool isValid23 = (0.0 < distance23 && distance23 <= 1.0);

    double distance31 = calculatePerpLength(point, this->vec3, this->vec1) / this->perpLength;
    bool isValid31 = (0.0 < distance31 && distance31 <= 1.0);

    double distance21 = calculatePerpLength(point, this->vec2, this->vec1) / this->perpLength;
    bool isValid21 = (0.0 < distance21 && distance21 <= 1.0);

    return isValid23 && isValid31 && isValid21 && (distance23 + distance31 + distance21 <= 1.0);
}

void Triangle::draw() {
    if (this->isClosed) {
        Triangle::drawSurface(vec1, vec2, vec3);
    } else {
        Triangle::drawLine(vec1, vec2);
        Triangle::drawLine(vec2, vec3);
        Triangle::drawLine(vec3, vec1);
    }
}

void Triangle::drawLine(bill::vector vector1, bill::vector vector2) {
    glPushMatrix();
    glLineWidth(2.5);
    glColor3f(0.8, 0.5, 0.8);
    glBegin(GL_LINES);
    glVertex3f(vector1[0], vector1[1], vector1[2]);
    glVertex3f(vector2[0], vector2[1], vector2[2]);
    glEnd();
    glPopMatrix();
}

void Triangle::drawSurface(bill::vector vector1, bill::vector vector2, bill::vector vector3) {
    glPushMatrix();
    glLineWidth(2.5);
    glColor3f(0.8, 0.5, 0.8);
    glBegin(GL_POLYGON);
    glVertex3f(vector1[0], vector1[1], vector1[2]);
    glVertex3f(vector2[0], vector2[1], vector2[2]);
    glVertex3f(vector3[0], vector3[1], vector3[2]);
    glEnd();
    glPopMatrix();
}
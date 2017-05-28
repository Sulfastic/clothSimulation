#ifndef EX4_TRIANGLE_H
#define EX4_TRIANGLE_H

#include "collisionObject.h"

class Triangle : public CollisionObject {
public:
    bill::vector vec1;
    bill::vector vec2;
    bill::vector vec3;
    const double CollisionObject::SUPPRESION = 0.6;
    bool isClosed;
    double perpLength;

    Triangle(const bill::vector &normal, const bill::vector &position, const bill::vector vector3, bool surface);
    Triangle(const bill::vector &vector1, const bill::vector &vector2, const bill::vector &vector3);

    void draw();
    void drawLine(bill::vector vector1, bill::vector vector2);
    void drawSurface(bill::vector vector1, bill::vector vector2, bill::vector vector3);
    void slowVelocities(bill::vector &vel1, bill::vector &vel2);
    bool is_inside_triangle(double distance23, double distance31, double distance21);
    bool collide(std::shared_ptr<bill::BillMaterialPoint> &mp);
    bool insideTriangle(const bill::vector &point);
    double calculatePerpLength(bill::vector &changeableVector, const bill::vector &vec1, const bill::vector &vec2,
                               const bill::vector &vec3);
    double calculatePerpLength(const bill::vector &vec1, const bill::vector &vec2, const bill::vector &vec3);
};

#endif //EX4_TRIANGLE_H

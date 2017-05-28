#ifndef GRASS_H
#define GRASS_H

#include <cmath>
#include <memory>
#include "../lib/headers/billmaterialpoint.h"
#include "../lib/headers/billvector.h"

class oscillator : public bill::BillMaterialPoint {
public:
    double SUPPRESSION;
    double k; // współczynnik sprężystości
    double l; // odległość spoczynkowa
    double g; // stała grawitacji

    std::shared_ptr<oscillator> left;
    std::shared_ptr<oscillator> right;
    std::shared_ptr<oscillator> up;
    std::shared_ptr<oscillator> down;
    std::shared_ptr<oscillator> upLeft;
    std::shared_ptr<oscillator> upRight;
    std::shared_ptr<oscillator> downLeft;
    std::shared_ptr<oscillator> downRight;
    int leftIndex;
    int rightIndex;
    int upIndex;
    int downIndex;
    int upLeftIndex;
    int upRightIndex;
    int downLeftIndex;
    int downRightIndex;

    oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position = bill::vector({0., 0., 0.}),
               bill::vector velocity = bill::vector({0., 0., 0.}), double mass = 1.0,
               bill::vector color = bill::vector({1.0, 0.0, 0.0}), double step = 0.2);

    virtual ~oscillator() {};

    bill::vector createForce(std::shared_ptr<oscillator> neighbor, double length);
    void setRight(std::shared_ptr<oscillator> right, int index);
    void setLeft(std::shared_ptr<oscillator> left, int index);
    void setUp(std::shared_ptr<oscillator> up, int index);
    void setDown(std::shared_ptr<oscillator> down, int index);
    void setUpLeft(std::shared_ptr<oscillator> upLeft, int index);
    void setUpRight(std::shared_ptr<oscillator> upRight, int index);
    void setDownLeft(std::shared_ptr<oscillator> downLeft, int index);
    void setDownRight(std::shared_ptr<oscillator> downRight, int index);

    virtual bill::vector Force();

    void printNeighbours();
}; //end of class oscillator
#endif

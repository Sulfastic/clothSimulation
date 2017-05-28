#include "../headers/oscillator.h"

bill::vector oscillator::Force() {
    double hypotenuse = sqrt(2.0) * this->l;

    bill::vector ForceLeft = createForce(this->left, this->l);
    bill::vector ForceRight = createForce(this->right, this->l);
    bill::vector ForceUp = createForce(this->up, this->l);
    bill::vector ForceDown = createForce(this->down, this->l);
    bill::vector ForceUpLeft = createForce(this->upLeft, hypotenuse);
    bill::vector ForceUpRight = createForce(this->upRight, hypotenuse);
    bill::vector ForceDownLeft = createForce(this->downLeft, hypotenuse);
    bill::vector ForceDownRight = createForce(this->downRight, hypotenuse);

    return ForceLeft + ForceRight + ForceUp + ForceDown +
           ForceUpLeft + ForceUpRight + ForceDownLeft + ForceDownRight +
           this->mass * this->g * bill::vector({0.0, -1.0, 0.0});
}

oscillator::oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position, bill::vector velocity,
                       double mass, bill::vector color, double step) : bill::BillMaterialPoint(algorithm, position,
                                                                                               velocity, mass, color,
                                                                                               step) {
    this->SUPPRESSION = 0.8;
    this->k = k;
    this->g = 0.005;
    this->l = l;
    this->right = NULL;
    this->left = NULL;
    this->up = NULL;
    this->down = NULL;
    this->upRight = NULL;
    this->upLeft = NULL;
    this->downRight = NULL;
    this->downLeft = NULL;
}

void oscillator::setRight(std::shared_ptr<oscillator> right, int index) {
    this->right = right;
    this->rightIndex = index;
}

void oscillator::setLeft(std::shared_ptr<oscillator> left, int index) {
    this->left = left;
    this->leftIndex = index;
}

void oscillator::setUp(std::shared_ptr<oscillator> up, int index) {
    this->up = up;
    this->upIndex = index;
}

void oscillator::setDown(std::shared_ptr<oscillator> down, int index) {
    this->down = down;
    this->downIndex = index;
}

void oscillator::setUpLeft(std::shared_ptr<oscillator> upLeft, int index) {
    this->upLeft = upLeft;
    this->upLeftIndex = index;
}

void oscillator::setUpRight(std::shared_ptr<oscillator> upRight, int index) {
    this->upRight = upRight;
    this->upRightIndex = index;
}

void oscillator::setDownLeft(std::shared_ptr<oscillator> downLeft, int index) {
    this->downLeft = downLeft;
    this->downLeftIndex = index;
}

void oscillator::setDownRight(std::shared_ptr<oscillator> downRight, int index) {
    this->downRight = downRight;
    this->downRightIndex = index;
}

bill::vector oscillator::createForce(std::shared_ptr<oscillator> neighbour, double length) {
    if (neighbour != NULL) {
        bill::vector vecDistanceDiff = this->position() - neighbour->position();
        const double vecDistaneLength = bill::vector::norm(vecDistanceDiff);
        vecDistanceDiff.normalize();
        const bill::vector vecVelocityDiff = neighbour->velocity() - this->velocity();
        const bill::vector deduct = vecVelocityDiff * vecDistanceDiff * vecDistanceDiff;
        return -this->k * vecDistanceDiff * (vecDistaneLength - length) + (this->SUPPRESSION * deduct);
    } else {
        return bill::vector({0, 0, 0});
    }
}

void oscillator::printNeighbours() {
    std::cout << "up: " << this->upIndex << " down: " << this->downIndex << " left: " <<this->leftIndex << " right: " <<this->rightIndex
              << " upLeft: " << this->upLeftIndex << " upRight: " << this->upRightIndex << " downLeft: " << this->downLeftIndex
              << " downRight: " << this->downRightIndex <<"\n";
}
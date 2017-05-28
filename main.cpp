#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include "lib/headers/billwindow.h"
#include "lib/headers/billGLfunctions.h"
#include "lib/headers/billmaterialpoint.h"
#include "lib/headers/billengine.h"
#include "lib/headers/billintegrators.h"
#include "headers/oscillator.h"
#include "headers/collisionObject.h"
#include "headers/billSetOfPointsWithCollisions.h"
#include "headers/triangle.h"

void renderScene(void);

void mainLoop(void);

void setNeighbours(std::vector<std::shared_ptr<oscillator>> array);

void printNeighbours(std::vector<std::shared_ptr<oscillator>> array);
void createIcosahedron(BillSetOfPointsWithCollisions &grass, bool isSurface);

class Grass : public BillSetOfPointsWithCollisions {
protected:
    float ground_level;
public:

    Grass() : BillSetOfPointsWithCollisions() {
        ground_level = 0.f;
    }

    void set_level(float level) {
        ground_level = level;
    }

    void drawLine(bill::vector vec1, bill::vector vec2, bill::vector color) {
        glPushMatrix();
        glLineWidth(2.5);
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_LINES);
        glVertex3f(vec1[0], vec1[1], vec1[2]);
        glVertex3f(vec2[0], vec2[1], vec2[2]);
        glEnd();
        glPopMatrix();
    }

    virtual void Draw() {
        BillSetOfPointsWithCollisions::Draw();

        glColor4f(0.28627451, 0.15686275, 0.12156863, 0.8);
        glBegin(GL_QUADS);
        glVertex3f(-5.0f, ground_level - 0.2, -5.0f);
        glVertex3f(-5.0f, ground_level - 0.2, 5.0f);
        glVertex3f(5.0f, ground_level, 5.0f);
        glVertex3f(5.0f, ground_level, -5.0f);
        glEnd();
        glPopMatrix();

        for (unsigned int i = 1; i < points.size(); ++i) {
            bill::vector x0 = points[i]->position();
            bill::vector xm = points[i - 1]->position();
            bill::vector color = {1.0, 1.0, 1.0};
            drawLine(x0, xm, color);
        }

        int offset = 2;

        bill::vector r1 = {-1, 0, 0};//x
        bill::vector r2 = {0, 0, 0};
        bill::vector rcolor = {1.0, 0, 0};
        bill::vector g1 = {-1, 0, 0};//y
        bill::vector g2 = {-1, 1, 0};
        bill::vector gcolor = {0, 1.0, 0};
        bill::vector b1 = {-1, 0, 0};//z
        bill::vector b2 = {-1, 0, 1};
        bill::vector bcolor = {0, 0, 1};

        r1[0] -= offset;
        r1[2] -= offset;
        r2[0] -= offset;
        r2[2] -= offset;
        g1[0] -= offset;
        g1[2] -= offset;
        g2[0] -= offset;
        g2[2] -= offset;
        b1[0] -= offset;
        b1[2] -= offset;
        b2[0] -= offset;
        b2[2] -= offset;

        drawLine(r1, r2, rcolor);
        drawLine(g1, g2, gcolor);
        drawLine(b1, b2, bcolor);
    }
} grass;

void processNormalKeys(unsigned char key, int x, int y) {
    bill::GLaux::processNormalKeys(key, x, y);
    if (std::isspace(key)) {
        bill::GLaux::eye[1] += 0.1;
        bill::GLaux::center[1] += 0.1;
    } else if (key == 'c') {
        bill::GLaux::eye[1] -= 0.1;
        bill::GLaux::center[1] -= 0.1;
    }
}

bill::BillEngine engine;
int SIZE_WIDTH = 10;
int SIZE_HEIGHT = 10;

int main(int argc, char **argv) {

    bill::GLaux::eye = bill::vector({-7, 1, 1});
    bill::GLaux::center = bill::vector({0, 0, 0});
    std::vector<std::shared_ptr<oscillator>> O;
    bill::vector color({0.5, 0.5, 0.5});

    std::vector<std::shared_ptr<CollisionObject>> object1;
    object1.emplace_back(new CollisionObject({0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}));
    grass.addObject(object1.back());

    createIcosahedron(grass, false); //true = surface, false = wired

    double step = 0.03; // integration step
    double k = 1;
    double mass = 0.25;

    //Tworzymy Oscylatory Sprzężone

    for (int i = 0; i < SIZE_HEIGHT; i++) {
        for (int j = 0; j < SIZE_WIDTH; j++) {
            O.push_back(std::shared_ptr<oscillator>(
                    new oscillator(bill::ModEuler, k, 0.35, bill::vector({-1 + 0.35 * i, 2, -1.5 + 0.35 * j}),
                                   bill::vector({0.0, 0.0, 0.0}),
                                   mass, color, step)));
            grass.AddPoint(O.back());
        }
    }

    setNeighbours(O);
//    printNeighbours(O); //just for debug purposes

    engine = bill::BillEngine(grass);

    bill::Window window(argc, argv);
    window.set_processNormalKeys(processNormalKeys);
    window.set_processNormalKeysUp(bill::GLaux::processNormalKeysUp);
    window.set_renderScene(renderScene);
    window.set_mainLoop(mainLoop);

    window.initiate();
    return 0;
}

void setNeighbours(std::vector<std::shared_ptr<oscillator>> array) {
    for (int i = 0; i < SIZE_HEIGHT * SIZE_WIDTH; i++) {
//      right springs
        if ((i + 1) % SIZE_HEIGHT) {
            array[i]->setRight(array[i + 1], i + 1);
        }
//      left springs
        if (i % SIZE_HEIGHT - 1 >= 0) {
            array[i]->setLeft(array[i - 1], i - 1);
        }
//      down springs
        if (i + SIZE_HEIGHT < SIZE_HEIGHT * SIZE_WIDTH) {
            array[i]->setDown(array[i + SIZE_HEIGHT], i + SIZE_HEIGHT);
        }
//      up springs
        if (i - SIZE_HEIGHT >= 0) {
            array[i]->setUp(array[i - SIZE_HEIGHT], i - SIZE_HEIGHT);
        }
//      up right
        if (((i + 1) % SIZE_HEIGHT) && (i - SIZE_HEIGHT >= 0)) {
            array[i]->setUpRight(array[i + 1 - SIZE_HEIGHT], i + 1 - SIZE_HEIGHT);
        }
//      up left
        if ((i - SIZE_HEIGHT >= 0) && (i % SIZE_HEIGHT - 1 >= 0)) {
            array[i]->setUpLeft(array[i - 1 - SIZE_HEIGHT], i - 1 - SIZE_HEIGHT);
        }
//      down right
        if ((i + SIZE_HEIGHT < SIZE_HEIGHT * SIZE_WIDTH) && ((i + 1) % SIZE_HEIGHT)) {
            array[i]->setDownRight(array[i + SIZE_HEIGHT + 1], i + SIZE_HEIGHT + 1);
        }
//      down left
        if ((i + SIZE_HEIGHT < SIZE_HEIGHT * SIZE_WIDTH) && (i % SIZE_HEIGHT - 1 >= 0)) {
            array[i]->setDownLeft(array[i + SIZE_HEIGHT - 1], i + SIZE_HEIGHT - 1);
        }
    }
}

void printNeighbours(std::vector<std::shared_ptr<oscillator>> array) {
    for (int i = 0; i < SIZE_HEIGHT * SIZE_WIDTH; i++) {
        std::cout << i << " ";
        array[i]->printNeighbours();
    }
}

void mainLoop(void) {
    engine.step();
    renderScene();
}

void renderScene(void) {
    if (bill::GLaux::moveParallel | bill::GLaux::movePerpendicular | bill::GLaux::rotateParallel |
        bill::GLaux::rotatePerpendicular)
        bill::GLaux::computePos();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 1000000000.0);
    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();
    gluLookAt(bill::GLaux::eye[0], bill::GLaux::eye[1], bill::GLaux::eye[2],
              bill::GLaux::center[0], bill::GLaux::center[1], bill::GLaux::center[2],
              bill::GLaux::versor_up[0], bill::GLaux::versor_up[1], bill::GLaux::versor_up[2]);


    grass.Draw();

    glutSwapBuffers();
}

//https://schneide.wordpress.com/2016/07/15/generating-an-icosphere-in-c/
void createIcosahedron(BillSetOfPointsWithCollisions &grass, bool isSurface) {
    const double X=.525731112119133606 * 0.8;
    const double Z=.850650808352039932 * 0.8;
    const double N=0.0;
    const double yOffset = 0.5;

    bill::vector v[12] = {
            {-X,N + yOffset,Z}, {X,N + yOffset,Z}, {-X,N + yOffset,-Z}, {X,N + yOffset,-Z},
            {N,Z + yOffset,X}, {N,Z + yOffset,-X}, {N,-Z + yOffset,X}, {N,-Z + yOffset,-X},
            {Z,X + yOffset,N}, {-Z,X + yOffset, N}, {Z,-X + yOffset,N}, {-Z,-X + yOffset, N}
    };

    std::vector<std::shared_ptr<Triangle>> triangles[20];

    triangles[0].emplace_back(new Triangle(v[0], v[4], v[1], isSurface));
    triangles[1].emplace_back(new Triangle(v[0], v[9], v[4], isSurface));
    triangles[2].emplace_back(new Triangle(v[9], v[5], v[4], isSurface));
    triangles[3].emplace_back(new Triangle(v[4], v[5], v[8], isSurface));
    triangles[4].emplace_back(new Triangle(v[4], v[8], v[1], isSurface));
    triangles[5].emplace_back(new Triangle(v[8], v[10], v[1], isSurface));
    triangles[6].emplace_back(new Triangle(v[8], v[3], v[10], isSurface));
    triangles[7].emplace_back(new Triangle(v[5], v[3], v[8], isSurface));
    triangles[8].emplace_back(new Triangle(v[5], v[2], v[3], isSurface));
    triangles[9].emplace_back(new Triangle(v[2], v[7], v[3], isSurface));
    triangles[10].emplace_back(new Triangle(v[7], v[10], v[3], isSurface));
    triangles[11].emplace_back(new Triangle(v[7], v[6], v[10], isSurface));
    triangles[12].emplace_back(new Triangle(v[7], v[11], v[6], isSurface));
    triangles[13].emplace_back(new Triangle(v[11], v[0], v[6], isSurface));
    triangles[14].emplace_back(new Triangle(v[0], v[1], v[6], isSurface));
    triangles[15].emplace_back(new Triangle(v[6], v[1], v[10], isSurface));
    triangles[16].emplace_back(new Triangle(v[9], v[0], v[11], isSurface));
    triangles[17].emplace_back(new Triangle(v[9], v[11], v[2], isSurface));
    triangles[18].emplace_back(new Triangle(v[9], v[2], v[5], isSurface));
    triangles[19].emplace_back(new Triangle(v[7], v[2], v[11], isSurface));

    for(std::vector<std::shared_ptr<Triangle>> triangle : triangles) {
        grass.addObject(triangle.back());
    }
}


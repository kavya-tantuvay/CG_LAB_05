#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <iostream>

// ---------- Point structure ----------
struct Point {
    int x, y;
    Point(int a, int b) : x(a), y(b) {}
};

// ---------- Global variables ----------
std::vector<Point> ddaPoints;
std::vector<Point> bresPoints;
int ddaIndex = 0, bresIndex = 0;

// ---------- DDA Algorithm ----------
std::vector<Point> getDDApoints(int x1, int y1, int x2, int y2) {
    std::vector<Point> points;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(abs(dx), abs(dy));

    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    float x = x1, y = y1;
    for (int i = 0; i <= steps; i++) {
        points.push_back(Point((int)round(x), (int)round(y)));
        x += Xinc;
        y += Yinc;
    }
    return points;
}

// ---------- Bresenham Algorithm ----------
std::vector<Point> getBresenhamPoints(int x1, int y1, int x2, int y2) {
    std::vector<Point> points;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        points.push_back(Point(x1, y1));
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
    return points;
}

// ---------- Function to draw text ----------
void drawText(const char* text, int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2i(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// ---------- Draw one step ----------
void drawStep() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw DDA points
    glColor3f(1.0, 0.0, 0.0); // red
    glBegin(GL_POINTS);
    for (int i = 0; i < ddaIndex && i < (int)ddaPoints.size(); i++) {
        glVertex2i(ddaPoints[i].x, ddaPoints[i].y);
    }
    glEnd();

    // Draw Bresenham points
    glColor3f(0.0, 1.0, 0.0); // green
    glBegin(GL_POINTS);
    for (int i = 0; i < bresIndex && i < (int)bresPoints.size(); i++) {
        glVertex2i(bresPoints[i].x, bresPoints[i].y);
    }
    glEnd();

    // Labels
    drawText("DDA (Red)", 50, 480, 1.0, 0.0, 0.0);
    drawText("Bresenham (Green)", 50, 460, 0.0, 1.0, 0.0);

    glFlush();
}

// ---------- Timer function ----------
void timer(int) {
    if (ddaIndex < (int)ddaPoints.size()) ddaIndex++;
    if (bresIndex < (int)bresPoints.size()) bresIndex++;

    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);
}

// ---------- Init ----------
void init() {
    glClearColor(0, 0, 0, 1);
    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

// ---------- Main ----------
int main(int argc, char** argv) {
    int x1 = 50, y1 = 50, x2 = 400, y2 = 300;

    ddaPoints = getDDApoints(x1, y1, x2, y2);
    bresPoints = getBresenhamPoints(x1+50, y1+50, x2+50, y2+50);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Step-by-Step DDA vs Bresenham Line Drawing");

    init();
    glutDisplayFunc(drawStep);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}

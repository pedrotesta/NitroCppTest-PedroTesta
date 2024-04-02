#ifndef RECTANGLES_H
#define RECTANGLES_H

#include <vector>
#include <string>

using namespace std;

class Inter {
public:
    int x, y, w, h;
    string r1, r2;

    Inter(int X, int Y, int W, int H, string R1, string R2);
};

class Rect {
public:
    int x, y, w, h;
    string index;

    Rect(int X, int Y, int W, int H, string Index);

    void compare(const vector<Rect>& query_rectangles, int start, vector<Inter>& Intersections) const;
};

vector<Rect> loadRectangles(const char* input_file);

vector<Inter> findIntersections(const vector<Rect>* rectangles);

void showIntersections(const vector<Inter>& output);

#endif

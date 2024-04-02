#include "rectangles.h"
#include <iostream>
#include <fstream>
#include "json/include/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

Rect::Rect(int X, int Y, int W, int H, string Index) : 
    x(X), y(Y), w(W), h(H), index(Index) {}
    
void Rect::compare(const vector<Rect>& query_rectangles, int start, vector<Inter>& Intersections) const{
    // Current rectangle
    int rx1 {this->x}, ry1 {this->y};
    int w1 {this->w}, h1 {this->h};

    // Loop to find intersections
    for(size_t i = start; i < query_rectangles.size(); i++){ 
        int rx2 {query_rectangles[i].x}, ry2 {query_rectangles[i].y};
        int w2 {query_rectangles[i].w}, h2 {query_rectangles[i].h};
        
        // Intersection condition
        if(min(rx1 + w1 - rx2, rx2 + w2 - rx1) >= 0){
            if(min(ry1 + h1 - ry2, ry2 + h2 - ry1) >= 0){
                Intersections.push_back(Inter(max(rx1, rx2), max(ry1, ry2),
                                        min(rx1 + w1 - max(rx1, rx2), rx2 + w2 - max(rx1, rx2)),
                                        min(ry1 + h1 - max(ry1, ry2), ry2 + h2 - max(ry1, ry2)),
                                        this->index,
                                        query_rectangles[i].index));
                
                // Recursively check the subsequent rectangle
                Rect new_rectangle = Rect(max(rx1, rx2), max(ry1, ry2),
                                          min(rx1 + w1 - rx2, rx2 + w2 - rx1),
                                          min(ry1 + h1 - ry2, ry2 + h2 - ry1),
                                          this->index + ", " + query_rectangles[i].index);
                new_rectangle.compare(query_rectangles, i + 1, Intersections);
            }
        }
    }
}

Inter::Inter(int X, int Y, int W, int H, string R1, string R2) :
    x(X), y(Y), w(W), h(H), r1(R1), r2(R2) {}

vector<Rect> loadRectangles(const char * input_file){

    vector<Rect> rectangles;

    ifstream file(input_file);
    
    if (!file.is_open()) {
        cout << "Failed to open input file\n";
        return rectangles;
    }

    json input_json;
    try{
        file >> input_json;
    } catch (const std::exception& e) {
        cout << "Failed to parse JSON" << endl;
        return rectangles;
    }

    cout << "Input:" << endl;

    for(auto& rectangle : input_json["rects"].items()){
        cout << "\t" << stoi(rectangle.key()) + 1 
             << " rectangle at (" << rectangle.value()["x"] 
             << "," << rectangle.value()["y"] 
             << "), w=" << rectangle.value()["w"] 
             << ", h=" << rectangle.value()["h"] << "." << endl;
        rectangles.push_back(Rect(rectangle.value()["x"],
                                  rectangle.value()["y"],
                                  rectangle.value()["w"],
                                  rectangle.value()["h"],
                                  to_string(stoi(rectangle.key()) + 1)));
    }
    
    if (!rectangles.size()) {
        cout << "Empty list\n";
    }

    return rectangles;
}

vector<Inter> findIntersections(const vector<Rect>& rectangles){

    vector<Inter> Intersections;
    int start {0};

    for(size_t i = 0; i < rectangles.size(); i++){      
        rectangles[i].compare(rectangles, ++start, Intersections);
    }

    return Intersections;
}


void showIntersections(const vector<Inter>& output){

    cout << "Intersections" << endl;

    for(auto inter : output){
        cout << "Between rectangle " << inter.r1 
             << " and " << inter.r2
             << " at (" << inter.x << ","  << inter.y << "), "
             << "w=" << inter.w << ", h=" << inter.h << ".\n";
    }
}

int main(int argc, char* argv[]){ 

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    vector<Rect> rectangles = loadRectangles(argv[1]);
    
    if(!rectangles.size()){
        cout << "Closing application" << endl;
        return 1;
    }

    vector<Inter> output = findIntersections(rectangles);

    if(!output.size()){
        cout << "No intersection" << endl;
        return 1;
    }
    
    showIntersections(output);

    return 0;
}
#include <vector>
#include "./Transform2D.hpp"
using namespace std;
#include <cmath>
// int total_items;
// Vector2 child[];
// vector <float> xpos;
// void sortChilds(){
//     for(int i=0; i<total_items; i++){
//         Transform2D transform;
//         transform.getPosition();
//         xpos.push_back(child[i].x);
//     }
//     sort(xpos.begin(), xpos.end());
// }

int total_items=7;
struct Point {
    float x, y;
};
Vector2 child[] = {
    {13.0f, 14.0f},
    {3.0f, 4.0f},
    {5.0f, 6.0f},
    {11.0f, 12.0f},
    {1.0f, 2.0f},
    {9.0f, 10.0f},
    {7.0f, 8.0f}
};

vector <float> xpos;
vector<vector<float>> newpos;

void sortAndAssign(vector<float> &vec, int start, int end){
    if(start == end){
        newpos.push_back({vec[start]});
        return;
    }
    int midpoint = start+(end-start)/2;
    sortAndAssign(vec, start, midpoint);
    sortAndAssign(vec, midpoint+1, end);
}

void sortChilds(){
    for (int i = 0; i < total_items; i++) {
        xpos.push_back(child[i].x);
    }

    sort(xpos.begin(), xpos.end());

    sortAndAssign(xpos, 0, xpos.size() - 1);
}

int main(){
    sortChilds();
        // Output for verification
    for (int i = 0; i < newpos.size(); i++) {
        for (float val : newpos[i]) {
            cout << "Child " << i + 1 << ": " << val << endl;
        }
    }

    return 0;
}


//for bounding retangle

vector<Point> getRectangleVertices(float cx, float cy, float w, float h, float theta) {
    vector<Point> boundingVertices(4);
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    float w2 = w / 2;
    float h2 = h / 2;

    boundingVertices[0] = {cx + (w2 * cosTheta - h2 * sinTheta), cy + (w2 * sinTheta + h2 * cosTheta)};
    boundingVertices[1] = {cx + (-w2 * cosTheta - h2 * sinTheta), cy + (-w2 * sinTheta + h2 * cosTheta)};
    boundingVertices[2] = {cx + (-w2 * cosTheta + h2 * sinTheta), cy + (-w2 * sinTheta - h2 * cosTheta)};
    boundingVertices[3] = {cx + (w2 * cosTheta + h2 * sinTheta), cy + (w2 * sinTheta - h2 * cosTheta)};

    return boundingVertices;
}

vector<Point> getBoundingVertices(const vector<Point>& vertices) {
    float minX = vertices[0].x;
    float maxX = vertices[0].x;
    float minY = vertices[0].y;
    float maxY = vertices[0].y;

    for (const auto& vertex : vertices) {
        if (vertex.x < minX) minX = vertex.x;
        if (vertex.x > maxX) maxX = vertex.x;
        if (vertex.y < minY) minY = vertex.y;
        if (vertex.y > maxY) maxY = vertex.y;
    }

    vector<Point> boundingBox = {
        {minX, maxY}, 
        {maxX, maxY}, 
        {minX, minY}, 
        {maxX, minY}   
    };

    return boundingBox;
}

vector<Point> getBoundingVertices(float cx, float cy, float r) {
    vector<Point> boundingVertices(4); 

    boundingVertices[0] = {cx - r, cy + r}; 
    boundingVertices[1] = {cx + r, cy + r}; 
    boundingVertices[2] = {cx - r, cy - r}; 
    boundingVertices[3] = {cx + r, cy - r}; 
    return boundingVertices;
}
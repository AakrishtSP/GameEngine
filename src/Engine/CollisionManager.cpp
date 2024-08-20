#include "CollisionManager.hpp"

//To check if a point passes origin with respect to a point
bool CollisionManager::pointPassedOrigin (Vector2 refrenceVec, Vector2 testingVec){
    if (DotProduct(refrenceVec, testingVec) < 0)
        return false;
    else
        return true;
}

//To check if a point passes origin with respect to two points
bool CollisionManager::pointPassedOrigin (Vector2 refrenceVec1, Vector2 refrenceVec2, Vector2 testingVec){
    Vector2 perpendiVec = directionToOrigin(refrenceVec1, refrenceVec2);
    if (DotProduct(perpendiVec, testingVec) < 0)
        return false;
    else   
        return true;
}

//Gives perpendicular vector pointing towards orign 
Vector2 CollisionManager::directionToOrigin (Vector2 vec1, Vector2 vec2){
    Vector2 lineVec = vec2 - vec1;
    Vector2 perpendiVec = {lineVec.y, lineVec.x * -1};
    if (DotProduct(vec1, perpendiVec) < 0)
        perpendiVec = perpendiVec * -1;

    return Normalize(perpendiVec);
}

//Returns Zero if the triangle contains the orign, else returns region in which the origin lies
int CollisionManager::triangleContainOrigin (Vector2 oldVec1, Vector2 oldVec2, Vector2 recentVec){
    Vector2 vecRecnet = recentVec * -1;
    Vector2 perpendiVec = directionToOrigin(oldVec1, recentVec);
    if (DotProduct(vecRecnet, perpendiVec) > 0)
        return 1;
    perpendiVec = directionToOrigin(oldVec2, recentVec);
    if (DotProduct(vecRecnet, perpendiVec) > 0)
        return 2;
    return 0;
}

//Support function of Rectangle
Vector2 CollisionManager::supportFunction(const Rect& rect, const Vector2& direction) {

    Vector2 dir = Normalize(direction);
    Vector2 vertices[4]= {
        {rect.x, rect.y},
        {rect.x + rect.width, rect.y},
        {rect.x + rect.width, rect.y + rect.height},
        {rect.x, rect.y + rect.height}
    };
    
    Vector2 rotationPoint = {rect.x + rect.width / 2, rect.y + rect.height / 2};
    for (int i = 0; i < 4; i++) {
        RotatePoint(vertices[i], rotationPoint, rect.rotation);
    }

    float maxDot = DotProduct(vertices[0], dir);
    Vector2 returnVertex = vertices[0];
    for (int i = 1; i < 4; i++) {
        float dot = DotProduct(vertices[i], dir);
        if (dot > maxDot) {
            maxDot = dot;
            returnVertex = vertices[i];
        }
    }

    return returnVertex;
}

//Support function of Circle
Vector2 CollisionManager::supportFunction(const Circle &circle, const Vector2 &direction)
{
    Vector2 dir = Normalize(direction);
    return circle.center + dir * circle.radius;
}

//Simplex support function for two rectangles
Vector2 CollisionManager::simplexSupportFunction(const Rect& rect1,const Rect& rect2 ,const Vector2& direction){
    Vector2 support1 = supportFunction(rect1, direction);
    Vector2 support2 = supportFunction(rect2, direction * -1);
    return support1 - support2;
}
//Simplex support function a rectangle and a circle
Vector2 CollisionManager::simplexSupportFunction(const Rect& rect, const Circle& circle, const Vector2& direction){
    Vector2 support1 = supportFunction(rect, direction);
    Vector2 support2 = supportFunction(circle, direction * -1);
    return support1 - support2;
}

//Simplex support function for two circles
Vector2 CollisionManager::simplexSupportFunction(const Circle& circle1, const Circle& circle2, const Vector2& direction){
    Vector2 support1 = supportFunction(circle1, direction);
    Vector2 support2 = supportFunction(circle2, direction * -1);
    return support1 - support2;
}

//Check if two rectangles collide
bool CollisionManager::didCollide (Rect rect1, Rect rect2){
    Vector2 direction = Normalize(Vector2{rect2.x - rect1.x, rect2.y - rect1.y});
    Vector2 simplex1 = simplexSupportFunction(rect1, rect2, direction);
    direction = Normalize(simplex1 * -1);
    Vector2 simplex2 = simplexSupportFunction(rect1, rect2, direction);
    if (!pointPassedOrigin(simplex1, simplex2))
        return false;

    direction = directionToOrigin(simplex1, simplex2);
    Vector2 simplex3 = simplexSupportFunction(rect1, rect2, direction);
    Vector2 newSimplex;
    int region;
    while (true){
        if (!pointPassedOrigin(simplex1,simplex2,simplex3))
            return false;
        region = triangleContainOrigin(simplex1, simplex2, simplex3);
        if (region == 0)
            return true;
        else if (region == 1)
            simplex2 = simplex3;
        else if (region == 2)
            simplex1 = simplex3;
        direction = directionToOrigin(simplex1, simplex2);
        newSimplex = simplexSupportFunction(rect1, rect2, direction);
        if(newSimplex == simplex1 || newSimplex == simplex2)
            return false;
        else
            simplex3 = newSimplex;
    }
    return false;
}

//Check if a rectangle and a circle collide
bool CollisionManager::didCollide (Rect rect, Circle Cir){
    Vector2 direction = Normalize(Vector2{Cir.center.x - rect.x, Cir.center.y - rect.y});
    Vector2 simplex1 = simplexSupportFunction(rect, Cir, direction);
    direction = Normalize(simplex1 * -1);
    Vector2 simplex2 = simplexSupportFunction(rect, Cir, direction);
    if (!pointPassedOrigin(simplex1, simplex2))
        return false;
    direction = directionToOrigin(simplex1, simplex2);
    Vector2 simplex3 = simplexSupportFunction(rect, Cir, direction);
    Vector2 newSimplex;
    int region;
    while (true){
        if (!pointPassedOrigin(simplex1,simplex2,simplex3))
            return false;
        region = triangleContainOrigin(simplex1, simplex2, simplex3);
        if (region == 0)
            return true;
        else if (region == 1)
            simplex2 = simplex3;
        else if (region == 2)
            simplex1 = simplex3;
        direction = directionToOrigin(simplex1, simplex2);
        newSimplex = simplexSupportFunction(rect, Cir, direction);
        if(newSimplex == simplex1 || newSimplex == simplex2)
            return false;
        else
            simplex3 = newSimplex;
    }
    return false;
}

//Check if two circles collide
bool CollisionManager::didCollide (Circle Cir1, Circle Cir2){
    float distance = Magnitude(Cir1.center - Cir2.center);
    if (distance < Cir1.radius + Cir2.radius)
        return true;
    else
        return false;
}
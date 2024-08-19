#include "CollisionManager.hpp"
#include "Vector2Ext.hpp"


bool CollisionManager::pointPassedOrigin (Vector2 refrenceVec, Vector2 testingVec){
    if (DotProduct(refrenceVec, testingVec) < 0)
        return false;
    else
        return true;
}

bool CollisionManager::pointPassedOrigin (Vector2 refrenceVec1, Vector2 refrenceVec2, Vector2 testingVec){
    Vector2 perpendiVec = directionToOrigin(refrenceVec1, refrenceVec2);
    if (DotProduct(perpendiVec, testingVec) < 0)
        return false;
    else   
        return true;
}

Vector2 CollisionManager::directionToOrigin (Vector2 vec1, Vector2 vec2){
    Vector2 lineVec = vec2 - vec1;
    Vector2 perpendiVec = {lineVec.y, lineVec.x * -1};
    if (DotProduct(vec1, perpendiVec) < 0)
        perpendiVec = perpendiVec * -1;

    return Normalize(perpendiVec);
}

int CollisionManager::triangleContainOrigin (Vector2 oldVec1, Vector2 oldVec2, Vector2 recentVec){
    Vector2 vecRecnet = recentVec * -1;
    Vector2 perpendiVec = directionToOrigin(oldVec2, recentVec);
    if (DotProduct(vecRecnet, perpendiVec) > 0)
        return 1;
    perpendiVec = directionToOrigin(oldVec1, recentVec);
    if (DotProduct(vecRecnet, perpendiVec) > 0)
        return 2;
    return 0;
}

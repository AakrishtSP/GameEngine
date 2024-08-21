#pragma once
#include "Vector2Ext.hpp"
class CollisionManager{
    public:
    static CollisionManager& getInstance() {
        static CollisionManager instance;
        return instance;
    }

    bool didCollide (Rect rect1, Rect rect2);
    bool didCollide (Circle Cir1, Circle Cir2);
    bool didCollide (Rect rect, Circle Cir);
    bool didCollide (Circle Cir, Rect rect){return didCollide(rect, Cir);};



    int triangleContainOrigin (Vector2 vec1, Vector2 vec2, Vector2 vec3);
    bool pointPassedOrigin (Vector2 refrenceVec, Vector2 testingVec);
    bool pointPassedOrigin (Vector2 refrenceVec1, Vector2 refrenceVec2, Vector2 testingVec);
    Vector2 directionToOrigin (Vector2 vec1, Vector2 vec2);
    Vector2 supportFunction(const Rect& rect, const Vector2& direction);
    Vector2 supportFunction(const Circle &circle, const Vector2 &direction);
    Vector2 simplexSupportFunction(const Rect& rect1,const Rect& rect2 ,const Vector2& direction);
    Vector2 simplexSupportFunction(const Rect& rect, const Circle& circle, const Vector2& direction);
    Vector2 simplexSupportFunction(const Circle& circle1, const Circle& circle2, const Vector2& direction);


    private:
        CollisionManager() = default;
        CollisionManager(const CollisionManager&) = delete;
        CollisionManager& operator=(const CollisionManager&) = delete;
        //std::unordered_map<std::string, vector<>> creators;
};
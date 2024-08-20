#pragma once
#include <raylib.h>
class CollisionManager{
    public:
    static CollisionManager& getInstance() {
        static CollisionManager instance;
        return instance;
    }

    inline float getRadian(float dgree){
        return (dgree/180)*3.14;
    }
    inline float getDgree(float radian){
        return (radian*180)/3.14;
    }

    bool didCollide (Rectangle rect1, Rectangle rect2);
    bool didCollide (Rectangle rect, Circle Cir);
    bool didCollide (Circle Cir1, Circle Cir2);
    int triangleContainOrigin (Vector2 vec1, Vector2 vec2, Vector2 vec3);
    bool pointPassedOrigin (Vector2 refrenceVec, Vector2 testingVec);
    bool pointPassedOrigin (Vector2 refrenceVec1, Vector2 refrenceVec2, Vector2 testingVec);
    Vector2 directionToOrigin (Vector2 vec1, Vector2 vec2);
    Vector2 supportFunction(const Rectangle& rect, float rotation, const Vector2& direction);
    Vector2 supportFunction(const Circle &circle, const Vector2 &direction);
    Vector2 simplexSupportFunction(const Rectangle& rect1,const Rectangle& rect2 ,const Vector2& direction);
    Vector2 simplexSupportFunction(const Rectangle& rect, const Circle& circle, const Vector2& direction);
    Vector2 simplexSupportFunction(const Circle& circle1, const Circle& circle2, const Vector2& direction);


    private:
        
};

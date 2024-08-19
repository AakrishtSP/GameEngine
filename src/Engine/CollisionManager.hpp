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
    int containOrigin (Vector2 vec1, Vector2 vec2, Vector2 vec3);
    bool pointPassedOrigin (Vector2 refrenceVec, Vector2 testingVec);
    Vector2 directionToOrigin (Vector2 vec1, Vector2 vec2);


    private:
        
};
#pragma once

class CollisionManager{
    public:
    static CollisionManager& getInstance() {
        static CollisionManager instance;
        return instance;
    }
    private:
        
};
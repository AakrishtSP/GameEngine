#pragma once
#include "Vector2Ext.hpp"
#include "Components/Collider.hpp"

class AABB;
class BVHNode;
class CollisionManager{
    public:
    static CollisionManager& getInstance() {
        static CollisionManager instance;
        return instance;
    }

        // Broad phase collision detection
    void updateAABBs(const std::vector<Collider>& colliders);
    bool checkAABBCollision(const AABB& aabb1, const AABB& aabb2) const;
    void buildBVH(const std::vector<Collider>& colliders);
    bool checkBVHCollision(const BVHNode& node1, const BVHNode& node2) const;


    bool didCollide (Rect rect1, Rect rect2);
    bool didCollide (Circle Cir1, Circle Cir2);
    bool didCollide (Rect rect, Circle Cir);
    bool didCollide (Circle Cir, Rect rect){return didCollide(rect, Cir);};

    //For EPA
    template<class T>
    Vector2 penetrationVector(T rect1, T rect2);
    Vector2 closestEdgetoOrigin();

    int triangleContainOrigin (Vector2 vec1, Vector2 vec2, Vector2 vec3);
    bool pointPassedOrigin (Vector2 refrenceVec, Vector2 testingVec);
    bool pointPassedOrigin (Vector2 refrenceVec1, Vector2 refrenceVec2, Vector2 testingVec);
    float distanceToOrigin (Vector2 vec1, Vector2 vec2);
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
        std::vector<AABB> aabbs;  // List of AABBs for broad-phase collision detection
        std::unique_ptr<BVHNode> bvhRoot;  // Root of the BVH tree
        std::vector<Vector2> polytope;
};

// AABB structure definition
class AABB {
    Vector2 min;
    Vector2 max;

public:
    AABB() = default;
    AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}

    bool intersects(const AABB& other) const;
};

// BVH Node structure definition
class BVHNode {
public:
    AABB boundingBox;
    std::vector<std::shared_ptr<Collider>> colliders;  // Colliders within this node
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;

    BVHNode() = default;
    BVHNode(const AABB& box) : boundingBox(box) {}
};
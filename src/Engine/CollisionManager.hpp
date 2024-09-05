#pragma once
#include "Components/Collider.hpp"
#include "Vector2Ext.hpp"
#include <thread>
#include <mutex>
#include <future>
#include <set>

class AABB;
class BVHNode;
class Collider;
class CollisionShape;
class GameObject;
class CollisionManager {
public:
    static CollisionManager &getInstance() {
        static CollisionManager instance;
        return instance;
    }

    //For GJK
    template<typename Tm1,typename Tm2>
    bool didCollide(Tm1 shape1, Tm2 shape2);
    /*bool didCollide(Rect rect1, Rect rect2);
    bool didCollide(Circle Cir1, Circle Cir2);
    bool didCollide(Rect rect, Circle Cir);
    bool didCollide(Circle Cir, Rect rect) { return didCollide(rect, Cir); };*/


    // For EPA
    template<typename T1, typename T2>
    Vector2 penetrationVector(T1 shp1, T2 shp2);
    Vector2 closestEdgetoOrigin();

    int triangleContainOrigin(Vector2 vec1, Vector2 vec2, Vector2 vec3);
    bool pointPassedOrigin(Vector2 refrenceVec, Vector2 testingVec);
    bool pointPassedOrigin(Vector2 refrenceVec1, Vector2 refrenceVec2, Vector2 testingVec);
    float distanceToOrigin(Vector2 vec1, Vector2 vec2);
    Vector2 directionToOrigin(Vector2 vec1, Vector2 vec2);

    Vector2 GJKinitialDirection(const Rect &rect1, const Rect &rect2);
    Vector2 GJKinitialDirection(const Rect &rect, const Circle &circle);
    Vector2 GJKinitialDirection(const Circle &circle1, const Circle &circle2);
    Vector2 GJKinitialDirection(const Circle &circle, const Rect &rect){return GJKinitialDirection(rect, circle); }
    Vector2 supportFunction(const Rect &rect, const Vector2 &direction);
    Vector2 supportFunction(const Circle &circle, const Vector2 &direction);
    Vector2 simplexSupportFunction(const Rect &rect1, const Rect &rect2, const Vector2 &direction);
    Vector2 simplexSupportFunction(const Rect &rect, const Circle &circle, const Vector2 &direction);
    Vector2 simplexSupportFunction(const Circle &circle1, const Circle &circle2, const Vector2 &direction);
    Vector2 simplexSupportFunction(const Circle &circle, const Rect &rect, const Vector2 &direction) { return simplexSupportFunction(rect, circle, direction); }

    void update(float deltaTime);
    void renderUpdate(float renderDeltaTime){};

    void addCollider(Collider *collider);
    void resetColliders();

    void checkBroadCollisions();
    void checkNarrowCollisions();

    void resolveCollisions();

    void addPossibleCollision(std::vector<std::shared_ptr<CollisionShape>>);
    void addActualCollision(const std::shared_ptr<CollisionShape> &shape1,
                            const std::shared_ptr<CollisionShape> &shape2);
    void resetCollisions();


private:
    CollisionManager() = default;
    CollisionManager(const CollisionManager &) = delete;
    CollisionManager &operator=(const CollisionManager &) = delete;

    std::unique_ptr<BVHNode> bvhRoot; // Root of the BVH tree
    std::vector<Collider*> colliders; // List of colliders
    std::vector<std::shared_ptr<CollisionShape>> shapes; // List of collision shapes
    std::vector<std::vector<std::shared_ptr<CollisionShape>>>
            potentialCollisions; // List of colliders that are potentially colliding
    std::vector<std::vector<std::shared_ptr<CollisionShape>>>
            actualCollisions; // List of colliders that are actually colliding
    std::vector<std::vector<std::shared_ptr<GameObject>>>
            potentialCollisionsGO; // List of GameObjects that are potentially colliding
    std::vector<std::vector<std::shared_ptr<GameObject>>>
            actualCollisionsGO; // List of GameObjects that are actually colliding



    std::vector<Vector2> polytope;
};

// AABB structure definition
class AABB {
public:
    Vector2 min;
    Vector2 max;
    AABB() : min(Vector2({0, 0})), max(Vector2({0, 0})) {};
    AABB(const Vector2 &min, const Vector2 &max) : min(min), max(max) {}
    AABB(CollisionShape &shape);
    AABB(const Circle &circle);
    AABB(const Rect &rect);
    AABB(const Rectangle &rect);
    bool intersects(const AABB &other) const;
    AABB merge(const AABB &other);
};

// BVH Node structure definition
class BVHNode {
public:
    AABB boundingBox;
    std::vector<std::shared_ptr<CollisionShape>> colliders; // Colliders within this node
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::mutex bvhMutex;

    BVHNode() = default;
    BVHNode(const AABB &box) : boundingBox(box) {}
    BVHNode(const std::vector<std::shared_ptr<CollisionShape>> &colliders) : colliders(colliders) {}

    AABB calculateBoundingBox();

    void subdivide(int currentDepth, int maxDepth = 10, int minColliders = 1);
};

#include "CollisionManager.hpp"

// To check if a point passes origin with respect to a point
bool CollisionManager::pointPassedOrigin(Vector2 refrenceVec, Vector2 testingVec) {
    if (DotProduct(refrenceVec, testingVec) > 0)
        return false;
    else
        return true;
}

// To check if a point passes origin with respect to two points
bool CollisionManager::pointPassedOrigin(Vector2 refrenceVec1, Vector2 refrenceVec2, Vector2 testingVec) {
    Vector2 perpendiVec = directionToOrigin(refrenceVec1, refrenceVec2);
    if (DotProduct(perpendiVec, testingVec) < 0)
        return false;
    else
        return true;
}
// Gives perpendicular vector pointing towards orign
Vector2 CollisionManager::directionToOrigin(Vector2 vec1, Vector2 vec2) {
    Vector2 lineVec = vec2 - vec1;
    Vector2 perpendiVec = {lineVec.y, lineVec.x * -1};
    if (DotProduct(vec1, perpendiVec) < 0)
        perpendiVec = perpendiVec * -1;

    return Normalize(perpendiVec);
}

// Distance of a non-positional vector to origin
float CollisionManager::distanceToOrigin(Vector2 vec1, Vector2 vec2) {
    return Magnitude(vec1) * cos(Angle(vec1, directionToOrigin(vec1, vec2) * -1));
}
// Returns Zero if the triangle contains the orign, else returns region in which the origin lies
int CollisionManager::triangleContainOrigin(Vector2 oldVec1, Vector2 oldVec2, Vector2 recentVec) {
    Vector2 vecRecnet = recentVec * -1;
    Vector2 perpendiVec = directionToOrigin(oldVec1, recentVec);
    if (DotProduct(vecRecnet, perpendiVec) > 0)
        return 1;
    perpendiVec = directionToOrigin(oldVec2, recentVec);
    if (DotProduct(vecRecnet, perpendiVec) > 0)
        return 2;
    return 0;
}

// Support function of Rectangle
Vector2 CollisionManager::supportFunction(const Rect &rect, const Vector2 &direction) {

    Vector2 dir = Normalize(direction);
    Vector2 vertices[4] = {{rect.x - rect.width/2, rect.y - rect.height/2},
                           {rect.x + rect.width/2, rect.y - rect.height/2},
                           {rect.x + rect.width/2, rect.y + rect.height/2},
                           {rect.x - rect.width/2, rect.y + rect.height/2}};

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

// Support function of Circle
Vector2 CollisionManager::supportFunction(const Circle &circle, const Vector2 &direction) {
    Vector2 dir = Normalize(direction);
    return circle.center + dir * circle.radius;
}

//Simplex support fuction for two shapes
template<typename T1, typename T2>
Vector2 CollisionManager::simplexSupportFunction(const T1 &shape1, const T2 &shape2, const Vector2 &direction){
    Vector2 support1 = supportFunction(shape1, direction);
    Vector2 support2 = supportFunction(shape2, direction * -1);
    return support1 - support2;
}

// // Simplex support function for two rectangles
// Vector2 CollisionManager::simplexSupportFunction(const Rect &rect1, const Rect &rect2, const Vector2 &direction) {
//     Vector2 support1 = supportFunction(rect1, direction);
//     Vector2 support2 = supportFunction(rect2, direction * -1);
//     return support1 - support2;
// }
// // Simplex support function a rectangle and a circle
// Vector2 CollisionManager::simplexSupportFunction(const Rect &rect, const Circle &circle, const Vector2 &direction) {
//     Vector2 support1 = supportFunction(rect, direction);
//     Vector2 support2 = supportFunction(circle, direction * -1);
//     return support1 - support2;
// }

// // Simplex support function for two circles
// Vector2 CollisionManager::simplexSupportFunction(const Circle &circle1, const Circle &circle2,
//                                                  const Vector2 &direction) {
//     Vector2 support1 = supportFunction(circle1, direction);
//     Vector2 support2 = supportFunction(circle2, direction * -1);
//     return support1 - support2;
// }

void CollisionManager::update(float deltaTime) {
    checkBroadCollisions();
    checkNarrowCollisions();
    resolveCollisions();
    // std::cout << "No of potential collisions: " << potentialCollisions.size() << std::endl;
    resetCollisions();
}

void CollisionManager::renderUpdate(float renderDeltaTime) {
    float halfWidth = GetScreenWidth() / 2;
    float halfHeight = GetScreenHeight() / 2;
    int size = boundingBoxs.size();

    int scale = (size>0)? 255/size:0;
    int i=0;
    for (const auto &box: boundingBoxs) {
        Vector2 min = box->min;
        Vector2 max = box->max;
        Vector2 dif = max - min;
        min.x = halfWidth + min.x;
        min.y = halfHeight -  max.y;
        max = min + dif;

        int col = i++*scale;
        Color color = {(unsigned char) col, (unsigned char) col, (unsigned char) col, 255};

        DrawLineEx(min, min + Vector2{dif.x, 0}, 2, color);
        DrawLineEx(min, min + Vector2{0, dif.y}, 2, color);
        DrawLineEx(min + dif, min + Vector2{dif.x, 0}, 2, color);
        DrawLineEx(min + dif, min + Vector2{0, dif.y}, 2, color);
        // DrawLineEx(min, max, 2, color);
        // DrawLineEx({min.x, max.y}, {max.x, min.y}, 2, color);
        // DrawRectangleLines(min.x, min.y, dif.x, dif.y, color);
    }
}

void CollisionManager::addCollider(Collider *collider) {
    const auto &colliderShapes = collider->getShapes();

    // Add shapes if they are not already in the vector
    for (const auto &shape: colliderShapes) {
        if (std::find(shapes.begin(), shapes.end(), shape) == shapes.end()) {
            shapes.push_back(shape);
        }
    }

    // Add the collider if it's not already in the vector
    if (std::find(colliders.begin(), colliders.end(), collider) == colliders.end()) {
        colliders.push_back(collider);
    }
}

void CollisionManager::resetColliders() {
    colliders.clear();
    shapes.clear();
}

void CollisionManager::checkBroadCollisions() {
    // Reset the BVH tree
    bvhRoot.reset();
    boundingBoxs.clear();
    // std::cout << "Number of potential collisions: " << potentialCollisions.size() << std::endl;
    bvhRoot = std::make_unique<BVHNode>(shapes);
    // std::cout << "Number of potential collisions: " << potentialCollisions.size() << std::endl;
    bvhRoot->subdivide(0, 10, 1);
}

void CollisionManager::checkNarrowCollisions() {
    //for std::vector<std::vector<std::shared_ptr<CollisionShape>>> potentialCollisions
    int potentialCollisionSize = potentialCollisions.size();
    int n;
    bool ifColide=false;
    Vector2 moveVector;
    for(int i = 0; i < potentialCollisionSize; i++){
        n = potentialCollisions[i].size();
        for (int j = 0; j < n; j++){
            Rect rect1 = potentialCollisions[i][j]->getRectangle();
            Circle circle1 = potentialCollisions[i][j]->getCircle();
            for (int k = j+1; k < n; k++){
                Rect rect2 = potentialCollisions[i][k]->getRectangle();
                Circle circle2 = potentialCollisions[i][k]->getCircle();
                auto shape1 = potentialCollisions[i][j];
                auto shape2 = potentialCollisions[i][k];
                int shapes = potentialCollisions[i][j]->whatShape()*10 + potentialCollisions[i][k]->whatShape();
                switch (shapes)
                {
                    case 11:
                        ifColide = didCollide(circle1, circle2);
                        if (ifColide)
                            moveVector = penetrationVector(circle1, circle2);                        
                        break;
                    case 12:
                        ifColide = didCollide(circle1, rect2);
                        if (ifColide)
                            moveVector = penetrationVector(circle1, rect2);    
                        break;
                    case 21:
                        ifColide = didCollide(rect1, circle2);
                        if (ifColide)
                            moveVector = penetrationVector(rect1, circle2);    
                        break;
                    case 22:
                        ifColide = didCollide(rect1, rect2);
                        if (ifColide)
                            moveVector = penetrationVector(rect1, rect2);    
                        break;
                    default:
                        break;
                }
                if (ifColide){
                    potentialCollisionsGO[i][j]->getComponent<Transform2D>()->translate(moveVector/250 * -1); //Move the object back
                    potentialCollisionsGO[i][k]->getComponent<Transform2D>()->translate(moveVector/250);   //Sweet spot for resolution 3
                    addActualCollision(potentialCollisions[i][j],potentialCollisions[i][k]);
                }
            }
        }
    }
}

void CollisionManager::resolveCollisions() {}

void CollisionManager::addPossibleCollision(std::vector<std::shared_ptr<CollisionShape>> colshapes) {
    // Add the possible collision to the list
    potentialCollisions.push_back(colshapes);

    std::vector<GameObject *> colShapesGO;
    colShapesGO.reserve(colshapes.size()); // Reserve space for optimization

    for (const auto &shape: colshapes) {
        // Ensure the game object exists
        auto gameObject = shape->getGameObject();
        if (gameObject) { // Only add valid game objects
            colShapesGO.push_back(gameObject); // Avoid creating an extra shared_ptr
        } else {
            // Optionally, log or handle null game objects
            std::cerr << "Warning: CollisionShape has no associated GameObject" << std::endl;
        }
    }

    // Add the list of GameObjects corresponding to the collision shapes
    potentialCollisionsGO.push_back(colShapesGO);
}

void CollisionManager::addActualCollision(const std::shared_ptr<CollisionShape> &shape1, const std::shared_ptr<CollisionShape> &shape2){
    actualCollisions.push_back(std::vector<std::shared_ptr<CollisionShape>>({shape1, shape2}));
    actualCollisionsGO.push_back(std::vector<GameObject*>({shape1->getGameObject(), shape2->getGameObject()}));
}

// void CollisionManager::resetCollisions() {
//     for (auto &col: actualCollisions)
//         col.clear();
//     actualCollisions.clear();
//     for (auto &col: actualCollisionsGO)
//         col.clear();
//     actualCollisionsGO.clear();
//     for (auto &col: potentialCollisions)
//         col.clear();
//     potentialCollisions.clear();
//     for (auto &col: potentialCollisionsGO)
//         col.clear();
//     potentialCollisionsGO.clear();
// }

void CollisionManager::resetCollisions() {
    for (auto &col : actualCollisions) {
        for (auto &ptr : col) {
            ptr.reset(); // Explicitly reset shared_ptrs
        }
        col.clear();
    }
    actualCollisions.clear();

    for (auto &col : actualCollisionsGO) {
        col.clear();
    }
    actualCollisionsGO.clear();

    for (auto &col : potentialCollisions) {
        for (auto &ptr : col) {
            ptr.reset();
        }
        col.clear();
    }
    potentialCollisions.clear();

    for (auto &col : potentialCollisionsGO) {
        col.clear();
    }
    potentialCollisionsGO.clear();
}

//To get initial direction for GJK
Vector2 CollisionManager::GJKinitialDirection(const Rect &rect1, const Rect &rect2) {
    return Normalize(Vector2{rect2.x - rect1.x, rect2.y - rect1.y});
}
Vector2 CollisionManager::GJKinitialDirection(const Rect &rect, const Circle &circle) {
    return Normalize(Vector2{circle.center.x - rect.x, circle.center.y - rect.y});
}
Vector2 CollisionManager::GJKinitialDirection(const Circle &circle1, const Circle &circle2) {
    return Normalize(Vector2{circle2.center.x - circle1.center.x, circle2.center.y - circle1.center.y});
}

// Check if two Shapes collide
template<typename Tm1, typename Tm2>
bool CollisionManager::didCollide(Tm1 shape1, Tm2 shape2) {
    polytope.clear();
    Vector2 direction = GJKinitialDirection(shape1, shape2);
    Vector2 simplex1 = simplexSupportFunction(shape1, shape2, direction);
    direction = Normalize(simplex1 * -1);
    Vector2 simplex2 = simplexSupportFunction(shape1, shape2, direction);

    if (!pointPassedOrigin(simplex1, simplex2))
        return false;

    direction = directionToOrigin(simplex1, simplex2);
    Vector2 simplex3 = simplexSupportFunction(shape1, shape2, direction);
    Vector2 newSimplex;
    int region;
    while (true) {
        if (!pointPassedOrigin(simplex1, simplex2, simplex3))
            return false;
        region = triangleContainOrigin(simplex1, simplex2, simplex3);
        if (region == 0){
            polytope.push_back(simplex1);
            polytope.push_back(simplex2);
            polytope.push_back(simplex3);
            return true;
        }
        else if (region == 1)
            simplex2 = simplex3;
        else if (region == 2)
            simplex1 = simplex3;
        direction = directionToOrigin(simplex1, simplex2);
        newSimplex = simplexSupportFunction(shape1, shape2, direction);
        if (isApproxEqual(newSimplex, simplex1) || isApproxEqual(newSimplex, simplex2))
            return false;
        else
            simplex3 = newSimplex;
    }
    return false;
}

/*
bool CollisionManager::didCollide(Rect rect, Circle Cir) {
    Vector2 direction = GJKinitialDirection(rect, Cir);
    Vector2 simplex1 = simplexSupportFunction(rect, Cir, direction);
    direction = Normalize(simplex1 * -1);
    Vector2 simplex2 = simplexSupportFunction(rect, Cir, direction);
    if (!pointPassedOrigin(simplex1, simplex2))
        return false;
    direction = directionToOrigin(simplex1, simplex2);
    Vector2 simplex3 = simplexSupportFunction(rect, Cir, direction);
    Vector2 newSimplex;
    int region;
    while (true) {
        if (!pointPassedOrigin(simplex1, simplex2, simplex3))
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
        if (newSimplex == simplex1 || newSimplex == simplex2)
            return false;
        else
            simplex3 = newSimplex;
    }
    return false;
}

// Check if two circles collide
bool CollisionManager::didCollide(Circle Cir1, Circle Cir2) {
    float distance = Magnitude(Cir1.center - Cir2.center);
    if (distance < Cir1.radius + Cir2.radius)
        return true;
    else
        return false;
}
*/
// For closest edge to origin
Vector2 CollisionManager::closestEdgetoOrigin() {
    int n = polytope.size();
    Vector2 closestEdge = {0, (float) n - 1};
    float minDistance = distanceToOrigin(polytope.at(0), polytope.at(n - 1));
    float distance;
    float point1, point2;
    for (int i = 0; i < n; i++) {
        point1 = i;
        point2 = (i + 1) % n;
        distance = distanceToOrigin(polytope.at(point1), polytope.at(point2));
        if (distance < minDistance) {
            minDistance = distance;
            closestEdge = {point1, point2};
        }
    }
    return closestEdge;
}

// For penetration vector of two rectangles
template<typename T1, typename T2>
Vector2 CollisionManager::penetrationVector(T1 shp1, T2 shp2) {
//    if (!didCollide(shp1, shp2))
//         return Vector2{0, 0};
    Vector2 closestEdge, newPolytope;
    while (true) {
        closestEdge = closestEdgetoOrigin();
        newPolytope = simplexSupportFunction(shp1, shp2,
                                             directionToOrigin(polytope.at(closestEdge.x), polytope.at(closestEdge.y)));
        if (isApproxEqual(newPolytope, polytope.at(closestEdge.x)) || isApproxEqual(newPolytope, polytope.at(closestEdge.y))) {
            // return directionToOrigin(polytope.at(closestEdge.x), polytope.at(closestEdge.y)) 
            //         * distanceToOrigin(polytope.at(closestEdge.x), polytope.at(closestEdge.y));
            return ((polytope.at(closestEdge.y) + polytope.at(closestEdge.x))/2)* -1;
        } else
            polytope.insert(polytope.begin() + closestEdge.y, newPolytope);
    }
}

AABB::AABB(CollisionShape &shape) {
    Rectangle bbox = shape.getBoundingBox();
    float halfWidth = bbox.width / 2;
    float halfHeight = bbox.height / 2;
    min = {bbox.x - halfWidth, bbox.y - halfHeight};
    max = {bbox.x + halfWidth, bbox.y + halfHeight};
}

AABB::AABB(const Circle &circle) {
    min = circle.center - Vector2{circle.radius, circle.radius};
    max = circle.center + Vector2{circle.radius, circle.radius};
}

AABB::AABB(const Rect &rect) {
    float sinRot = sinf(rect.rotation);
    float cosRot = cosf(rect.rotation);
    Vector2 halfSize = {rect.width / 2, rect.height / 2};
    Vector2 rotatedHalfSize = {fabs(halfSize.x * cosRot - halfSize.y * sinRot),
                               fabs(halfSize.x * sinRot + halfSize.y * cosRot)};
    min = Vector2{rect.x, rect.y} - rotatedHalfSize;
    max = Vector2{rect.x, rect.y} + rotatedHalfSize;
}

AABB::AABB(const Rectangle &rect) {
    Vector2 halfSize = {rect.width / 2, rect.height / 2};
    min = Vector2{rect.x, rect.y} - halfSize;
    max = Vector2{rect.x, rect.y} + halfSize;
}

// AABB collision check
bool AABB::intersects(const AABB &other) const {
    return (max.x >= other.min.x && min.x <= other.max.x && max.y >= other.min.y && min.y <= other.max.y);
}

AABB AABB::merge(const AABB &other) {
    this->min = Vector2{std::min(this->min.x, other.min.x), std::min(this->min.y, other.min.y)};
    this->max = Vector2{std::max(this->max.x, other.max.x), std::max(this->max.y, other.max.y)};
    return *this;
}

AABB &BVHNode::calculateBoundingBox() {
    if (!colliders.empty())
        boundingBox = colliders.front()->getBoundingBox();
    for (const auto &collider: colliders) {
        boundingBox.merge(collider->getBoundingBox());
    }
    return boundingBox;
}

// void BVHNode::subdivide(int currentDepth, int maxDepth, int minColliders) {
//     CollisionManager &CollisionManager = CollisionManager::getInstance();
//     if ((colliders.size() <= minColliders || currentDepth >= maxDepth) && !colliders.empty()) {
//         if (colliders.size() < 2)
//             return;
//         CollisionManager.addPossibleCollision(colliders);
//         return;
//     }

//     // std::cout << "Subdividing node at depth " << currentDepth << std::endl;

//     // Calculate the bounding box of the current node
//     calculateBoundingBox();

//     CollisionManager.boundingBoxs.push_back(&boundingBox);
//     std::cout << "Size of bounding boxs: " << CollisionManager.boundingBoxs.size() << std::endl;
//     std::cout << "Bounding Box: " << boundingBox.min.x << " " << boundingBox.min.y << " " << boundingBox.max.x << " "
//               << boundingBox.max.y << std::endl;

//     // Determine whether to split vertically or horizontally
//     bool splitVertically = (boundingBox.max.x - boundingBox.min.x) >= (boundingBox.max.y - boundingBox.min.y);

//     // // Compute the mean center position along the split axis
//     // float meanPosition = 0.0f;
//     // for (const auto &collider: colliders) {
//     //     const AABB &box = collider->getBoundingBox();
//     //     float center = splitVertically ? (box.min.x + box.max.x) / 2.0f : (box.min.y + box.max.y) / 2.0f;
//     //     meanPosition += center;
//     // }
//     // meanPosition /= colliders.size();

//     float meanPosition = (splitVertically) ? boundingBox.max.x + boundingBox.min.x : boundingBox.max.y + boundingBox.min.y;
//     meanPosition /= 2;

//     // Create two child AABBs for the left and right nodes based on the mean position
//     AABB leftBox = boundingBox;
//     AABB rightBox = boundingBox;

//     if (splitVertically) {
//         leftBox.max.x = meanPosition;
//         rightBox.min.x = meanPosition;
//     } else {
//         leftBox.max.y = meanPosition;
//         rightBox.min.y = meanPosition;
//     }

//     // Create left and right child nodes
//     left = std::make_unique<BVHNode>(leftBox);
//     right = std::make_unique<BVHNode>(rightBox);

//     // Distribute colliders to left and right nodes
//     for (const auto &collider: colliders) {
//         const AABB &colliderBox = collider->getBoundingBox();
//         // Check which node the collider belongs to based on the bounding box
//         if (colliderBox.intersects(leftBox)) {
//             left->colliders.push_back(collider);
//         }
//         if (colliderBox.intersects(rightBox)) {
//             right->colliders.push_back(collider);
//         }
//     }

//     if (colliders.size() == left->colliders.size() || colliders.size() == right->colliders.size()) {
//         // If all colliders are in one child node, add them to potential collisions
//         if (colliders.size() < 2)
//             return;
//         CollisionManager.addPossibleCollision(colliders);
//         return;
//     }

//     // Clear the colliders from the current node after distributing them
//     colliders.clear();

//     // Recursively subdivide the left and right child nodes
//     if (!left->colliders.empty() && currentDepth < maxDepth && left->colliders.size() > minColliders) {
//         left->subdivide(currentDepth + 1, maxDepth, minColliders);
//     } else if (left->colliders.size() > 1) {
//         // If the left node can't be subdivided further, add it to potential collisions
//         CollisionManager.addPossibleCollision(left->colliders);
//     }

//     if (!right->colliders.empty() && currentDepth < maxDepth && right->colliders.size() > minColliders) {
//         right->subdivide(currentDepth + 1, maxDepth, minColliders);
//     } else if (right->colliders.size() > 1) {
//         // If the right node can't be subdivided further, add it to potential collisions
//         CollisionManager.addPossibleCollision(right->colliders);
//     }
// }

// void BVHNode::subdivide(int currentDepth, int maxDepth, int minColliders) {
//     CollisionManager &CollisionManager = CollisionManager::getInstance();
    
//     // Stop subdivision if max depth is reached or collider count is too small
//     if ((colliders.size() <= minColliders || currentDepth >= maxDepth) && !colliders.empty()) {
//         if (colliders.size() < 2)
//             return;
//         CollisionManager.addPossibleCollision(colliders);
//         calculateBoundingBox();
//         CollisionManager.boundingBoxs.push_back(&boundingBox);
//         return;
//     }

//     // Calculate the bounding box of the current node
//     calculateBoundingBox();

//     // Store the bounding box in the collision manager for debugging or future use
//     CollisionManager.boundingBoxs.push_back(&boundingBox);
//     std::cout << "Size of boundingBoxs: " << CollisionManager.boundingBoxs.size() << std::endl;
//     std::cout << "Bounding Box: " << boundingBox.min.x << " " << boundingBox.min.y << " " << boundingBox.max.x << " "
//               << boundingBox.max.y << std::endl;

//     // Determine whether to split vertically or horizontally
//     bool splitVertically = (boundingBox.max.x - boundingBox.min.x) >= (boundingBox.max.y - boundingBox.min.y);

//     // Compute the mean center position along the split axis
//     float meanPosition = 0.0f;
//     for (const auto &collider: colliders) {
//         const AABB &box = collider->getBoundingBox();
//         float center = splitVertically ? (box.min.x + box.max.x) / 2.0f : (box.min.y + box.max.y) / 2.0f;
//         meanPosition += center;
//     }
//     meanPosition /= colliders.size();
//     // // Calculate the mean position along the split axis
//     // float meanPosition = (splitVertically) ? (boundingBox.max.x + boundingBox.min.x) / 2.0f 
//     //                                        : (boundingBox.max.y + boundingBox.min.y) / 2.0f;

//     // Create two child AABBs for the left and right nodes based on the mean position
//     AABB leftBox = boundingBox;
//     AABB rightBox = boundingBox;

//     if (splitVertically) {
//         leftBox.max.x = meanPosition;
//         rightBox.min.x = meanPosition;
//     } else {
//         leftBox.max.y = meanPosition;
//         rightBox.min.y = meanPosition;
//     }

//     // Create left and right child nodes
//     left = std::make_unique<BVHNode>(leftBox);
//     right = std::make_unique<BVHNode>(rightBox);

//     // Distribute colliders to left and right nodes based on their centroids
//     for (const auto &collider: colliders) {
//         const AABB &colliderBox = collider->getBoundingBox();
        
//         // Calculate the centroid of the collider's bounding box
//         float centroid = (splitVertically) 
//                             ? (colliderBox.min.x + colliderBox.max.x) / 2.0f 
//                             : (colliderBox.min.y + colliderBox.max.y) / 2.0f;
        
//         // Assign the collider based on its centroid position relative to the split axis
//         if ((splitVertically && centroid < meanPosition) || (!splitVertically && centroid < meanPosition)) {
//             left->colliders.push_back(collider);
//         } else {
//             right->colliders.push_back(collider);
//         }
//     }

//     // Handle the case where all colliders end up in one node, preventing further subdivision
//     if (left->colliders.size() == colliders.size() || right->colliders.size() == colliders.size()) {
//         // If all colliders are in one child node, add them to potential collisions
//         if (colliders.size() < 2)
//             return;
//         CollisionManager.addPossibleCollision(colliders);
//         return;
//     }

//     // Clear the colliders from the current node after distributing them
//     colliders.clear();

//     // Recursively subdivide the left and right child nodes if possible
//     if (!left->colliders.empty() && currentDepth < maxDepth && left->colliders.size() > minColliders) {
//         left->subdivide(currentDepth + 1, maxDepth, minColliders);
//     } else if (left->colliders.size() > 1) {
//         // If the left node can't be subdivided further, add it to potential collisions
//         CollisionManager.addPossibleCollision(left->colliders);
//         left->calculateBoundingBox();
//         CollisionManager.boundingBoxs.push_back(&left->boundingBox);
//     }

//     if (!right->colliders.empty() && currentDepth < maxDepth && right->colliders.size() > minColliders) {
//         right->subdivide(currentDepth + 1, maxDepth, minColliders);
//     } else if (right->colliders.size() > 1) {
//         // If the right node can't be subdivided further, add it to potential collisions
//         CollisionManager.addPossibleCollision(right->colliders);
//         right->calculateBoundingBox();
//         CollisionManager.boundingBoxs.push_back(&right->boundingBox);
//     }
// }

void BVHNode::subdivide(int currentDepth, int maxDepth, int minColliders) {
    CollisionManager &CollisionManager = CollisionManager::getInstance();

    for (int i=0;i<colliders.size();i++){
        for(int j=i+1;j<colliders.size();j++){
            std::vector<std::shared_ptr<CollisionShape>> collid = {colliders[i], colliders[j]};
            CollisionManager.addPossibleCollision(collid);
        }
    }
}


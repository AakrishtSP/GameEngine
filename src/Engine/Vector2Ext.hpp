#pragma once
#include <raylib.h>
#include <math.h>
#include <vector>

typedef std::vector<Vector2> Polygon;

struct Circle {
    Vector2 center;
    float radius;
};

struct Rect {
    float x;
    float y;
    float height;
    float width;
    float rotation;
};

inline float getRadian(float dgree){
        return (dgree/180)*3.14;
    }
inline float getDgree(float radian){
        return (radian*180)/3.14;
}

// Addition operator for Vector2
inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

// Subtraction operator for Vector2
inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

// Multiplication operator for Vector2 with a scalar
inline Vector2 operator*(const Vector2& vec, const float scalar) {
    return { vec.x * scalar, vec.y * scalar };
}

// Multiplication operator for Vector2 with a scalar (reverse order)
inline Vector2 operator*(const float scalar, const Vector2& vec) {
    return { vec.x * scalar, vec.y * scalar };
}

// Division operator for Vector2 with a scalar
inline Vector2 operator/(const Vector2& vec, const float scalar) {
    return { vec.x / scalar, vec.y / scalar };
}

//Comparison operator for Vector2
inline bool operator==(const Vector2& lhs, const Vector2& rhs){
    return (lhs.x == rhs.x && lhs.y == rhs.y);
}

// Dot product for Vector2
inline float DotProduct(const Vector2& lhs, const Vector2& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

// Magnitude (length) of Vector2
inline float Magnitude(const Vector2& vec) {
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

// Normalize Vector2 (convert to unit vector)
inline Vector2 Normalize(const Vector2& vec) {
    float mag = Magnitude(vec);
    if (mag != 0.0f) {
        return { vec.x / mag, vec.y / mag };
    }
    return { 0.0f, 0.0f }; // Handle zero vector case
}

// Distance between two Vector2 points
inline float Distance(const Vector2& point1, const Vector2& point2) {
    return Magnitude(point1 - point2);
}

// Angle between two Vector2 vectors (in radians)
inline float Angle(const Vector2& vec1, const Vector2& vec2) {
    float dot = DotProduct(vec1, vec2);
    float mag1 = Magnitude(vec1);
    float mag2 = Magnitude(vec2);
    return acosf(dot / (mag1 * mag2));
}

// Project Vector2 `vec` onto Vector2 `onto`
inline Vector2 Project(const Vector2& vec, const Vector2& onto) {
    float ontoMagSq = DotProduct(onto, onto);
    if (ontoMagSq != 0.0f) {
        float scalar = DotProduct(vec, onto) / ontoMagSq;
        return onto * scalar;
    }
    return { 0.0f, 0.0f }; // Handle zero vector case
}

// Reflect Vector2 `vec` off Vector2 `normal`
inline Vector2 Reflect(const Vector2& vec, const Vector2& normal) {
    return vec - 2.0f * DotProduct(vec, normal) * normal;
}

//Rotate a Vector2 'vec' around a rotationPoint by `angle` radians
inline void RotatePoint(Vector2& vec, Vector2 rotationPoint, float rotationAngle){
    float x = vec.x - rotationPoint.x;
    float y = vec.y - rotationPoint.y;
    float radi = getRadian(rotationAngle);
    float cosA = cos(radi);
    float sinA = sin(radi);
    vec.x = x * cosA - y * sinA + rotationPoint.x;
    vec.y = x * sinA + y * cosA + rotationPoint.y;
}

//To create a vector with Magnitude and Direction
inline Vector2 MakeVector(float magnitude, Vector2 direction){
    return Normalize(direction) * magnitude;
}

//To truncate a float to two decimal places
inline float truncateToOneDecimals(float value) {
    return static_cast<int>(value * 10) / 10.0f;
}

//To check if two Vector are aproximately equal
inline bool isApproxEqual(Vector2 vec1, Vector2 vec2){
    return (truncateToOneDecimals(vec1.x) == truncateToOneDecimals(vec2.x) && truncateToOneDecimals(vec1.y) == truncateToOneDecimals(vec2.y));
}
#include "raylib.h"

class RigidBody
{
protected:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    double mass;
    double width;
    double height;

public:
    RigidBody(double m, double w, double h): mass(m), width(w), height(h){
        position = {0.0f, 0.0f};
        velocity = {0.0f, 0.0f};
        acceleration = {0.0f, 0.0f};
    }
    Vector2 getpos()const {return position;}
    Vector2 getvel()const {return velocity;}
    Vector2 getacc()const {return acceleration;}
    double getmass()const {return mass;}
    double getWidth() const {return width;}
    double getHeight() const {return height;}

    void setPos(Vector2 pos){position = pos;}
    void setVel(Vector2 vel){velocity = vel;}
    void setAcc(Vector2 acc){acceleration = acc;}
};

void UpdateRigidBody(RigidBody &body, double dt){
    Vector2 newVelocity = body.getvel();
    newVelocity.x += body.getacc().x * dt;
    newVelocity.y += body.getacc().y * dt;
    body.setVel(newVelocity);

    Vector2 newPosition = body.getpos();
    newPosition.x += body.getpos().x * dt;
    newPosition.y += body.getpos().y * dt;
    body.setPos(newPosition);

    body.setAcc({0, 0});
}

void applyForce(RigidBody &body, Vector2 force){
    Vector2 newAcceleration = body.getacc();
    newAcceleration.x += force.x / body.getmass();
    newAcceleration.y += force.y / body.getmass();
}


int main() {
    int screenHeight = 700;
    int screenWidth = 1200;
    InitWindow(screenWidth, screenHeight, "RigidBody Example");

    RigidBody box(1.0, 50.0, 50.0); 
    box.setPos({screenWidth / 2.0f, screenHeight / 2.0f});  

    Vector2 gravity = {0, 1.0f};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        double deltaTime = GetFrameTime();

        applyForce(box, gravity);

        UpdateRigidBody(box, deltaTime);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(box.getpos().x, box.getpos().y, box.getWidth(), box.getHeight(), BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
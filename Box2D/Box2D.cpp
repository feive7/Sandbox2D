#include <iostream>
#include <vector>
#include <box2d/box2d.h>
#include <raylib.h>
#include <rlgl.h>
#include <raybox.h>
#include <BodyOps.h>

struct RayBody {
    b2BodyId id;
    Color color;
    void draw() {
        DrawBody(id, color);
    }
};

struct {
    bool active;
    b2BodyId bodyId;
} Selection;

void ResetScene(b2WorldId worldId, std::vector<RayBody>& bodies) {
    for (RayBody& body : bodies) {
        b2DestroyBody(body.id); // Destroy all bodies
    }
    bodies.clear(); // Clear boxes

    bodies.push_back({ CreateBox(worldId, {-120.0f,0.0f}, {10.0f,110.0f}, false), GREEN });
    bodies.push_back({ CreateBox(worldId, {120.0f,0.0f}, {10.0f,110.0f}, false), GREEN });
    bodies.push_back({ CreateBox(worldId, {0.0f,-120.0f}, {110.0f,10.0f}, false), GREEN });
    bodies.push_back({ CreateBox(worldId, {0.0f,120.0f}, {110.0f,10.0f}, false), GREEN});
}
int main() {
    // Window Definition
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Box2D");
    SetTargetFPS(60);

    // Viewport Definition
    Camera2D viewport;
    viewport.offset = { 400.0f, 400.0f };
    viewport.rotation = 180;
    viewport.target = { 0.0f,0.0f };
    viewport.zoom = 3.0f;

    // World Definition
    b2WorldDef worldDef = b2DefaultWorldDef();

    worldDef.gravity = { 0.0f, -200.0f };
    
    b2WorldId worldId = b2CreateWorld(&worldDef);

    // Dynamic Body Definition
    std::vector<RayBody> bodies;
    ResetScene(worldId, bodies); // Reset scene

    // Simulation setup
    float timeStep = 1.0f / 60.0f; // 60Hz

    int subStepCount = 4; // Run 4 simsteps each frame

    while (!WindowShouldClose()) {
        // Handle inputs
        float mwMove = GetMouseWheelMove();
        if (IsKeyPressed(KEY_R)) {
			ResetScene(worldId, bodies); // Reset scene
        }
        if (mwMove) {
            viewport.zoom *= pow(2.0,mwMove / 10.0f);
        }
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), viewport);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mPos = GetScreenToWorld2D(GetMousePosition(), viewport);
            b2Vec2 mVec = { mPos.x,mPos.y };
            for (RayBody body : bodies) {
                b2AABB test = b2Body_ComputeAABB(body.id);
                if (AABBContains(test, mVec)) {
                    Selection.active = true;
                    Selection.bodyId = body.id;
                    break;
                }
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            Selection.active = false;
        }
        if (Selection.active) {
            Vector2 mPos = GetScreenToWorld2D(GetMousePosition(), viewport);
            b2Vec2 mVec = { mPos.x,mPos.y };
            DragBody(Selection.bodyId, mVec);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mPos = GetScreenToWorld2D(GetMousePosition(), viewport);
            b2Vec2 mVec = { mPos.x,mPos.y };
            RayBody newBody = { CreateBall(worldId, mVec, 10.0f, true), RandomColor() };
            bodies.push_back(newBody);
        }
        // Simulate
        float deltaTime = GetFrameTime();
        b2World_Step(worldId, deltaTime, subStepCount);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(viewport);

        DrawLine(0, 3, 0, -3, GRAY);
        DrawLine(3, 0, -3, 0, GRAY);

        for (RayBody& body : bodies) {
            body.draw();

            b2JointId jointArray[3];
            b2Body_GetJoints(body.id, jointArray, 3);
            for (int i = 0; i < b2Body_GetJointCount(body.id); i++) {
                DrawJoint(jointArray[i]);
            }
        }

        
        EndMode2D();
        EndDrawing();
    }

    b2DestroyWorld(worldId); // Clear memory
    return 0;
}
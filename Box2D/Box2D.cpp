#include <iostream>
#include <box2d/box2d.h>
#include <raylib.h>

void DrawBody(b2BodyId id) {
    // Compute AABB
    b2ShapeId shapes[1]; // Only one shape for now
    b2Body_GetShapes(id, shapes, 1);

    b2ShapeId shape = shapes[0]; // Shape
    b2Polygon poly = b2Shape_GetPolygon(shape); // Polygon

    // Get Transformation
    b2Transform transformation = b2Body_GetTransform(id);

    b2Transform unrotated;
    unrotated.p = transformation.p;
    unrotated.q = b2Rot_identity;

    b2AABB aabb = b2ComputePolygonAABB(&poly, unrotated);
    b2Vec2 extents = b2AABB_Extents(aabb);
    b2Vec2 position = b2Body_GetPosition(id); // Get position of dynamic body
    b2Rot rotation = b2Body_GetRotation(id); // Get rotation of dynamic body
    DrawRectanglePro({ position.x, - position.y, 2 * extents.x, 2 * extents.y }, { extents.x, extents.y }, - b2Rot_GetAngle(rotation) * RAD2DEG, GREEN); // Draw ground
}
int main() {
    // Window Definition
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Box2D");
    SetTargetFPS(60);

    // Viewport Definition
    Camera2D viewport;
    viewport.offset = { 400.0f, 225.0f };
    viewport.rotation = 0.0f;
    viewport.target = { 0.0f,0.0f };
    viewport.zoom = 3.0f;

    // World Definition
    b2WorldDef worldDef = b2DefaultWorldDef();

    worldDef.gravity = { 0.0f, -200.0f };
    
    b2WorldId worldId = b2CreateWorld(&worldDef);

    // Ground Definition
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = { 0.0f, -10.0f };

    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);

    b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);

    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    // Dynamic Body Definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = { 60.0f, 40.0f };
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon dynamicBox = b2MakeBox(20.0f, 10.0f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;

    b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox); // Attaches shapeDef to dynamicBox

    // Simulation setup
    float timeStep = 1.0f / 60.0f; // 60Hz

    int subStepCount = 4; // Run 4 simsteps each frame

    while (!WindowShouldClose()) {
        // Simulate
        float deltaTime = GetFrameTime();
        b2World_Step(worldId, deltaTime, subStepCount);


        b2Vec2 position = b2Body_GetPosition(bodyId); // Get position of dynamic body
        b2Rot rotation = b2Body_GetRotation(bodyId); // Get rotation of dynamic body

        b2Vec2 groundPos = b2Body_GetPosition(groundId);
        b2Rot groundRot = b2Body_GetRotation(groundId);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(viewport);

        DrawBody(bodyId);
        DrawBody(groundId);

        EndMode2D();
        EndDrawing();
    }

    b2DestroyWorld(worldId); // Clear memory
    return 0;
}
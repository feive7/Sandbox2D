#include <iostream>
#include <box2d/box2d.h>
#include <raylib.h>

void DrawBody(b2BodyId id, Color color) {
    // Compute AABB
    b2ShapeId shapes[10];
    b2Body_GetShapes(id, shapes, 10);

    for (int i = 0; i < b2Body_GetShapeCount(id); i++) {
        b2ShapeId shape = shapes[i]; // Shape
        int shapeType = b2Shape_GetType(shape);

        switch (shapeType) {
        case b2_polygonShape: {
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
            DrawRectanglePro({ position.x, position.y, 2 * extents.x, 2 * extents.y }, { extents.x, extents.y }, b2Rot_GetAngle(rotation) * RAD2DEG, color); // Draw ground
            break;
        }
        case b2_circleShape: {
            b2Circle circle = b2Shape_GetCircle(shape);
            b2Transform transformation = b2Body_GetTransform(id);
            b2Vec2 position = b2Body_GetPosition(id); // Get position of dynamic body
            b2Rot rotation = b2Body_GetRotation(id); // Get rotation of dynamic body
            DrawCircleV({ position.x, position.y }, circle.radius, color); // Draw circle
            break;
        }
        }
    }
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
    viewport.rotation = 180;
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

    b2Polygon groundBox = b2MakeBox(50.0f, 2.0f);

    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

	b2Body_SetTransform(groundId, { 0.0f, -10.0f }, b2MakeRot(PI / 16.0f)); // Set ground position

    // Dynamic Body Definition
    b2BodyDef boxBodyDef = b2DefaultBodyDef();
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.position = { 0.0f, 40.0f };
    b2BodyId boxBodyId = b2CreateBody(worldId, &boxBodyDef);

    b2Polygon dynamicBox = b2MakeBox(20.0f, 10.0f);

    b2ShapeDef boxShapeDef = b2DefaultShapeDef();
    boxShapeDef.density = 1.0f;
    boxShapeDef.material.friction = 0.3f;

    b2CreatePolygonShape(boxBodyId, &boxShapeDef, &dynamicBox);

    // Circle Body Definition
    b2BodyDef circleBodyDef = b2DefaultBodyDef();
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position = { 0.0f, 20.0f };
    b2BodyId circleBodyId = b2CreateBody(worldId, &circleBodyDef);

    b2Circle dynamicCircle;
    dynamicCircle.center = { 0.0f,0.0f };
    dynamicCircle.radius = 10.0f;

    b2ShapeDef circleShapeDef = b2DefaultShapeDef();

    b2CreateCircleShape(circleBodyId, &circleShapeDef, &dynamicCircle);

	// Mouse Joint Definition


    // Simulation setup
    float timeStep = 1.0f / 60.0f; // 60Hz

    int subStepCount = 4; // Run 4 simsteps each frame

    while (!WindowShouldClose()) {
        // Handle inputs
        float mwMove = GetMouseWheelMove();
        if (mwMove) {
            viewport.zoom *= pow(2.0,mwMove / 10.0f);
        }
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), viewport);
        //b2Body_SetTransform(circleBodyId, { mousePos.x,mousePos.y }, b2Rot_identity);

        // Simulate
        float deltaTime = GetFrameTime();
        b2World_Step(worldId, deltaTime, subStepCount);
        

        b2Vec2 position = b2Body_GetPosition(boxBodyId); // Get position of dynamic body
        b2Rot rotation = b2Body_GetRotation(boxBodyId); // Get rotation of dynamic body

        b2Vec2 groundPos = b2Body_GetPosition(groundId);
        b2Rot groundRot = b2Body_GetRotation(groundId);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(viewport);

        DrawBody(boxBodyId, RED);
        DrawBody(circleBodyId, GREEN);
        DrawBody(groundId, BLUE);

        EndMode2D();
        EndDrawing();
    }

    b2DestroyWorld(worldId); // Clear memory
    return 0;
}
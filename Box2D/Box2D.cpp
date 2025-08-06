#include <iostream>
#include <vector>
#include <box2d/box2d.h>
#include <raylib.h>
#include <rlgl.h>
#include <RLGUI.h>
#include <raybox.h>
#include <BodyOps.h>

enum Selection_mode {
    MODE_SELECT,
    MODE_JOINT,
    MODE_COUNT,
};
struct RayBody {
    b2BodyId id;
    Color color;
    void draw() {
        DrawBody(id, color);
    }
};

struct {
    int mode = MODE_SELECT;
    bool active;
    b2BodyId bodyIds[10];
    int numOfBodyIds;
    b2Vec2 localPoint;
    void clear() {
        Selection.numOfBodyIds = {};
        Selection.numOfBodyIds = 0;
    }
} Selection;
struct {
    bool open;
    GUI gui;
} SpawnMenu;

void ResetScene(b2WorldId worldId, std::vector<RayBody>& bodies) {
    for (RayBody& body : bodies) {
        b2DestroyBody(body.id); // Destroy all bodies
    }
    bodies.clear(); // Clear bodies

    bodies.push_back({ CreateHollowBox(worldId, {0.0f,0.0f}, {130.0f,130.0f}, false),BLUE });
}
void InitSpawnMenu() {
    SpawnMenu.gui.addButton({
        .x = 10,
        .y = 10,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .fontColor = DARKGRAY,
        .text = "Spawn Ball",
        .id = 0,
        });
    SpawnMenu.gui.addButton({
        .x = 145,
        .y = 10,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .fontColor = DARKGRAY,
        .text = "Spawn Box",
        .id = 1,
        });
    SpawnMenu.gui.addButton({
        .x = 280,
        .y = 10,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .fontColor = DARKGRAY,
        .text = "Spawn Cup",
        .id = 2,
        });
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

    // Spawn menu setup
    InitSpawnMenu();

    // Simulation setup
    float timeStep = 1.0f / 60.0f; // 60Hz

    int subStepCount = 4; // Run 4 simsteps each frame

    while (!WindowShouldClose()) {
        // Handle inputs
        float mwMove = GetMouseWheelMove();
        if (IsKeyPressed(KEY_R)) {
			ResetScene(worldId, bodies); // Reset scene
        }
        if (IsKeyDown(KEY_Q)) {
            // Open spawn menu
			SpawnMenu.open = true;
        }
        else {
            // Close spawn menu
            SpawnMenu.open = false;
        }
        if (IsKeyPressed(KEY_M)) {
            Selection.mode++;
            Selection.mode %= MODE_COUNT;
        }
        if (mwMove) {
            viewport.zoom *= pow(2.0,mwMove / 10.0f);
        }

        if (SpawnMenu.open) {
            Vector2 mPos = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int buttonId = SpawnMenu.gui.getHovering(mPos); // Get ID of button clicked
                /*switch (buttonId) {
                    case 0: bodies.push_back({ CreateBall(worldId, {0.0f,0.0f}, 10.0f, true), RED }); break;
                    case 1: bodies.push_back({ CreateBox(worldId, {0.0f,0.0f}, {10.0f,10.0f}, true), RED }); break;
                    case 2: bodies.push_back({ CreateCup(worldId, {0.0f,0.0f}, {30.0f,30.0f}, true), RED }); break;
                }*/
                if (buttonId == 0) {
                    bodies.push_back({ CreateBall(worldId, {0.0f,0.0f}, 10.0f, true), RandomColor()});
                }
                if (buttonId == 1) {
                    bodies.push_back({ CreateBox(worldId, {0.0f,0.0f}, {10.0f,10.0f}, true), RandomColor() });
                }
                if (buttonId == 2) {
                    bodies.push_back({ CreateCup(worldId, {0.0f,0.0f}, {20.0f,20.0f}, true), RandomColor() });
                }
            }
        }
        else {
            Vector2 mPos = GetScreenToWorld2D(GetMousePosition(), viewport);
            b2Vec2 mVec = { mPos.x,mPos.y };
            if (Selection.mode == MODE_SELECT) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // Check if a body is under the mouse
                    for (RayBody body : bodies) {
                        if (BodyContains(body.id, mVec)) {
                            Selection.bodyIds[0] = body.id;
                            Selection.localPoint = b2Body_GetLocalPoint(body.id, mVec);
                            Selection.numOfBodyIds = 1;
                            break;
                        }
                    }
                }
                else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    b2Body_SetMotionLocks(Selection.bodyIds[0], { false, false, false });
                    Selection.numOfBodyIds = 0;
                }
                if (Selection.numOfBodyIds) {
                    BodyUnfreeze(Selection.bodyIds[0]); // Freeze the body
                    b2Body_SetMotionLocks(Selection.bodyIds[0], {false, false, true}); // Unfreeze the body
                    DragBody(Selection.bodyIds[0], mVec, Selection.localPoint);

                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        BodyFreeze(Selection.bodyIds[0]); // Freeze the body
                        Selection.clear(); // Deselect
                    }
                    if (IsKeyPressed(KEY_DELETE) && b2Body_IsValid(Selection.bodyIds[0])) {
                        // Delete code
                    }
                }
            }
            else if (Selection.mode == MODE_JOINT) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // Check if a body is under the mouse
                    Vector2 mPos = GetScreenToWorld2D(GetMousePosition(), viewport);
                    b2Vec2 mVec = { mPos.x,mPos.y };
                    for (RayBody body : bodies) {
                        if (BodyContains(body.id, mVec)) {
                            // Add body to selection
                            Selection.bodyIds[Selection.numOfBodyIds] = body.id;
                            Selection.numOfBodyIds++;
                            break;
                        }
                    }
                }
                if (Selection.numOfBodyIds == 2) {
                    if (Selection.bodyIds[0].index1 != Selection.bodyIds[1].index1) { // Make sure we aren't jointing an object to itself
                        HingeBodies(worldId, Selection.bodyIds[0], Selection.bodyIds[1]);
                        printf("Jointed bodies %i and %i\n", Selection.bodyIds[0].index1, Selection.bodyIds[1].index1);
                    }
                    else {
                        printf("Ignoring self joint\n");
                    }
                    Selection.clear(); // Clear selection regardless
                }
            }
        }

        // Simulate
        float deltaTime = GetFrameTime();
        b2World_Step(worldId, deltaTime, subStepCount);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(viewport);
        /*DrawLine(0, 3, 0, -3, GRAY);
        DrawLine(3, 0, -3, 0, GRAY);*/

        for (RayBody& body : bodies) {
            body.draw();
            b2JointId jointArray[3];
            b2Body_GetJoints(body.id, jointArray, 3);
            for (int i = 0; i < b2Body_GetJointCount(body.id); i++) {
                DrawJoint(jointArray[i]);
            }
        }
        //DrawAABB(b2Body_ComputeAABB(Selection.bodyId));
        
        EndMode2D();
        if (SpawnMenu.open) {
            SpawnMenu.gui.draw();
        }

        EndDrawing();
    }

    b2DestroyWorld(worldId); // Clear memory
    return 0;
}
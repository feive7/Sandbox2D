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

GUI spawnMenu;

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

void ResetScene(b2WorldId worldId, std::vector<RayBody>& bodies) {
    for (RayBody& body : bodies) {
        b2DestroyBody(body.id); // Destroy all bodies
    }
    bodies.clear(); // Clear bodies

    bodies.push_back({ CreateHollowBox(worldId, {0.0f,0.0f}, {130.0f,130.0f}, false),BLUE });
}
void InitGUIs() {
    spawnMenu.x = 0;
    spawnMenu.y = 0;
    spawnMenu.padding = 10;
    spawnMenu.addButton({
        .x = 0,
        .y = 0,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .fontColor = WHITE,
        .text = "Spawn Ball",
        .id = 0,
        });
    spawnMenu.addButton({
        .x = 135,
        .y = 0,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .fontColor = WHITE,
        .text = "Spawn Box",
        .id = 1,
        });
    spawnMenu.addButton({
        .x = 270,
        .y = 0,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .fontColor = WHITE,
        .text = "Spawn Cup",
        .id = 2,
        });
    spawnMenu.addLabel({
        .x = 10,
        .y = 70,
        .fontSize = 20,
        .fontColor = BLACK,
        .text = "Q: Open Spawn Menu\n"
                 "Left Click: Drag Body\n"
				 "Right Click while dragging: Freeze Body\n"
                 "R: Reset Scene\n"
                 //"M: Change Selection Mode\n"
                 "Scroll Wheel: Zoom Viewport\n"
		});

    spawnMenu.sizeToFit();
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
    InitGUIs();

    // One time control display
    bool OTCD = false;

    // Simulation setup
    float timeStep = 1.0f / 60.0f; // 60Hz

    int subStepCount = 4; // Run 4 simsteps each frame

    while (!WindowShouldClose()) {
        // Handle inputs
        float mwMove = GetMouseWheelMove();
        if (IsKeyPressed(KEY_R)) {
			ResetScene(worldId, bodies); // Reset scene
        }
        if (IsKeyPressed(KEY_Z)) {
            // Undo
            if (bodies.size() > 1) {
                b2DestroyBody(bodies.back().id); // Destroy last body
                bodies.pop_back(); // Remove last body
            }
        }
        if (IsKeyDown(KEY_Q)) {
            // Open spawn menu
            OTCD = false;
			spawnMenu.active = true;
        }
        else {
            // Close spawn menu
            spawnMenu.active = false;
        }
        if (IsKeyPressed(KEY_M)) {
            Selection.mode++;
            Selection.mode %= MODE_COUNT;
        }
        if (mwMove) {
            viewport.zoom *= pow(2.0,mwMove / 10.0f);
        }

        if (spawnMenu.active) {
            Vector2 mPos = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int buttonId = spawnMenu.getHovering(mPos); // Get ID of button clicked
                Vector2 spawnPos = GetScreenToWorld2D({ (float)spawnMenu.x,(float)spawnMenu.y }, viewport);
                /*switch (buttonId) {
                    case 0: bodies.push_back({ CreateBall(worldId, {0.0f,0.0f}, 10.0f, true), RED }); break;
                    case 1: bodies.push_back({ CreateBox(worldId, {0.0f,0.0f}, {10.0f,10.0f}, true), RED }); break;
                    case 2: bodies.push_back({ CreateCup(worldId, {0.0f,0.0f}, {30.0f,30.0f}, true), RED }); break;
                }*/
                if (buttonId == 0) {
                    bodies.push_back({ CreateBall(worldId, {spawnPos.x,spawnPos.y}, 10.0f, true), RandomColor()});
                }
                if (buttonId == 1) {
                    bodies.push_back({ CreateBox(worldId, {spawnPos.x,spawnPos.y}, {10.0f,10.0f}, true), RandomColor() });
                }
                if (buttonId == 2) {
                    bodies.push_back({ CreateCup(worldId, {spawnPos.x,spawnPos.y}, {20.0f,20.0f}, true), RandomColor() });
                }
            }
        }
        else {
            Vector2 mousePos = GetMousePosition();
            spawnMenu.x = mousePos.x;
            spawnMenu.y = mousePos.y;
            Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, viewport);
            b2Vec2 mVec = { mouseWorldPos.x,mouseWorldPos.y };
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
                        //HingeBodies(worldId, Selection.bodyIds[0], Selection.bodyIds[1]);
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
        if (OTCD) {
            const char* text = "Hold Q to open spawn menu";
            int measure = MeasureText(text, 40);
            DrawText(text, (screenWidth - measure) / 2, (screenHeight - 40) / 2, 40, {200,200,200,255});
        }
        if (spawnMenu.active) {
            spawnMenu.draw();
            DrawCircle(spawnMenu.x, spawnMenu.y, 10, { 120,120,120,255 });
            DrawCircle(spawnMenu.x, spawnMenu.y, 6, { 255,255,255,255 });
        }

        EndDrawing();
    }

    b2DestroyWorld(worldId); // Clear memory
    return 0;
}
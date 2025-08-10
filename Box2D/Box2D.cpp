#include <iostream>
#include <vector>
#include <box2d.h>
#include <raylib.h>
#include <rlgl.h>
#include <RLGUI.h>
#include <raybox.h>
#include <BodyOps.h>

enum Selection_mode {
    MODE_DRAG,
    MODE_WELD,
    MODE_WHEEL,
    MODE_DRAW,
    MODE_EXPLODE,
    MODE_COUNT,
};

GUI spawnMenu;
GUI controlMenu;
GUI toolMenu;

struct {
    int mode = MODE_DRAG;
    b2Vec2 spawnPos;
    b2BodyId bodyIds[10];
    int numOfBodyIds;
    b2Vec2 localPoints[10];
    void clear() {
        Selection.numOfBodyIds = 0;
    }
} Selection;

void ResetScene(b2WorldId worldId, std::vector<RayBody>& bodies) {
    for (RayBody& body : bodies) {
        b2DestroyBody(body.id); // Destroy all bodies
    }
    bodies.clear(); // Clear bodies

    bodies.push_back({ CreateHollowBox(worldId, {0.0f,0.0f}, {130.0f,130.0f}, b2_staticBody),BLUE });
}
void InitGUIs() {
    spawnMenu.x = 30;
    spawnMenu.y = 30;
    spawnMenu.width = 730;
    spawnMenu.height = 730;
    spawnMenu.padding = 10;
	// Regular Objects
    spawnMenu.addLabel({
        .x = 0,
        .y = 0,
        .fontSize = 30,
        .fontColor = BLACK,
        .text = "Regular Objects",
        });
    spawnMenu.addButton({
        .x = 0,
        .y = 35,
        .width = 100,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = GREEN,
        .fontColor = WHITE,
        .text = "Ball",
        .id = 0,
        });
    spawnMenu.addButton({
        .x = 102,
        .y = 35,
        .width = 100,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = GREEN,
        .fontColor = WHITE,
        .text = "Box",
        .id = 1,
        });
	// Misc Objects
    spawnMenu.addLabel({
        .x = 0,
        .y = 100,
        .fontSize = 30,
        .fontColor = BLACK,
        .text = "Misc Objects",
        });
    spawnMenu.addButton({
        .x = 0,
        .y = 135,
        .width = 100,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = GREEN,
        .fontColor = WHITE,
        .text = "Cup",
        .id = 2,
        });
    spawnMenu.addButton({
        .x = 102,
        .y = 135,
        .width = 100,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = GREEN,
        .fontColor = WHITE,
        .text = "Plank",
        .id = 3,
        });
    spawnMenu.addButton({
        .x = 204,
        .y = 135,
        .width = 100,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = GREEN,
        .fontColor = WHITE,
        .text = "Pellet",
        .id = 4,
        });
    spawnMenu.addButton({
        .x = 306,
        .y = 135,
        .width = 100,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = GREEN,
        .fontColor = WHITE,
        .text = "Tiny Box",
        .id = 5,
        });
    spawnMenu.addButton({
		.x = 408,
		.y = 135,
        .width = 100,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = GREEN,
        .fontColor = WHITE,
        .text = "Pentagon",
        .id = 6,
        });

    toolMenu.x = 0;
    toolMenu.y = 0;
    toolMenu.padding = 20;
    toolMenu.addButton({
        .x = 0,
        .y = 0,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = BLUE,
        .fontColor = WHITE,
        .text = "Drag Mode (1)",
        .id = MODE_DRAG,
        .selected = true,
        });
    toolMenu.addButton({
        .x = 0,
        .y = 55,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = BLUE,
        .fontColor = WHITE,
        .text = "Weld Mode (2)",
        .id = MODE_WELD,
        });
    toolMenu.addButton({
        .x = 0,
        .y = 110,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = BLUE,
        .fontColor = WHITE,
        .text = "Wheel Mode (3)",
        .id = MODE_WHEEL,
        });
    toolMenu.addButton({
        .x = 0,
        .y = 165,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = BLUE,
        .fontColor = WHITE,
        .text = "Drawing Mode (4)",
        .id = MODE_DRAW,
        });
    toolMenu.addButton({
        .x = 0,
        .y = 220,
        .width = 130,
        .height = 50,
        .fontSize = 20,
        .bgColor = GRAY,
        .bgColorSelected = BLUE,
        .fontColor = WHITE,
        .text = "Explosion Mode",
        .id = MODE_EXPLODE,
        });
    toolMenu.addLabel({
		.x = 0,
        .y = 300,
		.fontSize = 20,
		.fontColor = BLACK,
        .text = "Left Click: Grab Body, Select\n"
                "Right Click While Grabbing: Freeze Body\n"
                "C: Open Tool Menu\n"
                "Z: Undo Last Body\n"
                "R: Reset Scene\n"
                "Q: Open Menu\n"
				"Scroll Wheel: Zoom Viewport\n"
				"Middle Mouse Button: Pan Viewport\n"
        });
    toolMenu.sizeToFit();
}
void SetMode(int mode) {
    Selection.mode = mode;
    toolMenu.deselectButtons();
    toolMenu.selectButton(mode);
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

    // menu setup
    InitGUIs();

    // One time control display
    bool OTCD = true;

    // Simulation setup
    float timeStep = 1.0f / 60.0f; // 60Hz

    int subStepCount = 4; // Run 4 simsteps each frame

    bool paused = false;

    while (!WindowShouldClose()) {
        // Get inputs
        Vector2 mPos = GetMousePosition();
		Vector2 mWorldPos = GetScreenToWorld2D(mPos, viewport);
        Vector2 mDelta = GetMouseDelta();

        // Handle inputs
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            viewport.offset.x += mDelta.x;
            viewport.offset.y += mDelta.y;
        }
        if (IsKeyPressed(KEY_R)) {
            Selection.numOfBodyIds = 0;
            ResetScene(worldId, bodies); // Reset scene
            viewport.offset = { 400.0f, 400.0f };
            viewport.rotation = 180;
            viewport.target = { 0.0f,0.0f };
            viewport.zoom = 3.0f;
        }
        if (IsKeyPressed(KEY_Z) || IsKeyPressedRepeat(KEY_Z)) {
            // Undo
            Selection.numOfBodyIds = 0;
            if (bodies.size() > 1) {
                b2DestroyBody(bodies.back().id); // Destroy last body
                bodies.pop_back(); // Remove last body
            }
        }
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }
		if (IsKeyPressed(KEY_ONE)) SetMode(MODE_DRAG);
		if (IsKeyPressed(KEY_TWO)) SetMode(MODE_WELD);
		if (IsKeyPressed(KEY_THREE)) SetMode(MODE_WHEEL);
		if (IsKeyPressed(KEY_FOUR)) SetMode(MODE_DRAW);
        spawnMenu.active = IsKeyDown(KEY_Q);
		toolMenu.active = IsKeyDown(KEY_C);

        float mwMove = GetMouseWheelMove();
        if (mwMove) {
            viewport.zoom *= pow(2.0,mwMove / 10.0f);
        }

        if (toolMenu.active) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int buttonId = toolMenu.click(mPos); // Get ID of button clicked
                if (buttonId != -1) { // Button was pressed
					SetMode(buttonId); // Set the mode based on button ID
                }
            }
        }
        if (spawnMenu.active) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                spawnMenu.deselectButtons(); // Deselect all buttons
                int buttonId = spawnMenu.click(mPos); // Get ID of button clicked
                Vector2 spawnPos = GetScreenToWorld2D({ (float)spawnMenu.x,(float)spawnMenu.y }, viewport);
                switch (buttonId) {
                case 0:
                    bodies.push_back({ CreateBall(worldId, Selection.spawnPos, 10.0f, b2_dynamicBody), RandomColor() }); break;
                case 1:
                    bodies.push_back({ CreateBox(worldId, Selection.spawnPos, {10.0f,10.0f}, b2_dynamicBody), RandomColor() }); break;
                case 2:
                    bodies.push_back({ CreateCup(worldId, Selection.spawnPos, {20.0f,20.0f}, b2_dynamicBody), RandomColor() }); break;
                case 3:
                    bodies.push_back({ CreateBox(worldId, Selection.spawnPos, {20.0f,2.0f}, b2_dynamicBody), RandomColor() }); break;
                case 4:
                    bodies.push_back({ CreateBall(worldId, Selection.spawnPos, 3.0f, b2_dynamicBody), RandomColor() }); break;
                case 5:
                    bodies.push_back({ CreateBox(worldId, Selection.spawnPos, {3.0f,3.0f}, b2_dynamicBody), RandomColor() }); break;
                case 6:
					bodies.push_back({ CreateRegularPolygon(worldId, Selection.spawnPos, 5, 10.0f, b2_dynamicBody), RandomColor() }); break;
                case 7:
					bodies.push_back({ CreateRock(worldId, Selection.spawnPos, b2_dynamicBody), RandomColor() }); break;
                }
            }
        }
        if (!toolMenu.active && !spawnMenu.active) {
            Selection.spawnPos = { mWorldPos.x,mWorldPos.y };
            if (Selection.mode == MODE_DRAG) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // Check if a body is under the mouse
                    for (RayBody body : bodies) {
                        if (BodyContains(body.id, { mWorldPos.x,mWorldPos.y })) {
                            Selection.bodyIds[0] = body.id;
                            if (IsKeyDown(KEY_LEFT_CONTROL)) Selection.localPoints[0] = {0.0f,0.0f};
                            else Selection.localPoints[0] = b2Body_GetLocalPoint(body.id, { mWorldPos.x,mWorldPos.y });
                            Selection.numOfBodyIds = 1;
                            break;
                        }
                    }
                }
                else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    b2Body_SetFixedRotation(Selection.bodyIds[0], false);
                    Selection.numOfBodyIds = 0;
                }
                if (Selection.numOfBodyIds) {
                    BodyUnfreeze(Selection.bodyIds[0]); // Unfreeze the body
                    //DragBody(Selection.bodyIds[0], { mWorldPos.x,mWorldPos.y }, Selection.localPoints[0]);		

                    if (IsKeyDown(KEY_A)) {
                        // Allow rotation
                        b2Body_SetFixedRotation(Selection.bodyIds[0], false);
                        BodyRotate(Selection.bodyIds[0], -0.05f, Selection.localPoints[0], {mWorldPos.x,mWorldPos.y});
                    }
                    else if (IsKeyDown(KEY_D)) {
                        // Allow rotation
                        b2Body_SetFixedRotation(Selection.bodyIds[0], false);
                        BodyRotate(Selection.bodyIds[0], 0.05f, Selection.localPoints[0], { mWorldPos.x,mWorldPos.y });
                    }
                    else {
                        // Lock rotation
                        b2Body_SetFixedRotation(Selection.bodyIds[0], true);

                        // Keep body aligned to mouse without rotating
                        b2Vec2 worldPoint = b2Body_GetWorldPoint(Selection.bodyIds[0], Selection.localPoints[0]);
                        b2Vec2 newPos = b2Sub({ mWorldPos.x, mWorldPos.y }, b2Sub(worldPoint, b2Body_GetPosition(Selection.bodyIds[0])));
                        b2Body_SetTargetTransform(Selection.bodyIds[0], { newPos, b2Body_GetRotation(Selection.bodyIds[0]) }, 0.01f);
                    }

                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        BodyFreeze(Selection.bodyIds[0]); // Freeze the body
                        Selection.clear(); // Deselect
                    }
                    if (IsKeyPressed(KEY_DELETE) && b2Body_IsValid(Selection.bodyIds[0])) {
                        // Delete code
                    }
                }
            }
            else if (Selection.mode == MODE_WELD) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // Check if a body is under the mouse
                    for (RayBody body : bodies) {
                        if (BodyContains(body.id, { mWorldPos.x,mWorldPos.y })) {
                            // Add body to selection
                            Selection.bodyIds[Selection.numOfBodyIds] = body.id;
                            Selection.localPoints[Selection.numOfBodyIds] = b2Body_GetLocalPoint(body.id, { mWorldPos.x,mWorldPos.y });
                            Selection.numOfBodyIds++;
                            break;
                        }
                    }
                }
                if (Selection.numOfBodyIds == 2) {
                    if (Selection.bodyIds[0].index1 != Selection.bodyIds[1].index1) { // Make sure we aren't jointing an object to itself
                        //WeldBodies(worldId, Selection.bodyIds[0], Selection.bodyIds[1], Selection.localPoints[0], Selection.localPoints[1]);
                        WeldBodies(worldId, Selection.bodyIds[0], Selection.bodyIds[1]);
                        printf("Jointed bodies %i and %i\n", Selection.bodyIds[0].index1, Selection.bodyIds[1].index1);
                    }
                    else {
                        printf("Ignoring self joint\n");
                    }
                    Selection.clear(); // Clear selection regardless
                }
            }
            else if (Selection.mode == MODE_WHEEL) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    for (RayBody body : bodies) {
                        if (BodyContains(body.id, { mWorldPos.x,mWorldPos.y })) {
                            b2Vec2 pointOnBody = b2Body_GetLocalPoint(body.id, { mWorldPos.x,mWorldPos.y });
                            RayBody wheel = { CreateBall(worldId, { mWorldPos.x,mWorldPos.y }, 10.0f, b2_dynamicBody), DARKGRAY };
                            WheelBodies(worldId, body.id, wheel.id, pointOnBody);
                            bodies.push_back(wheel);
                            break;
                        }
                    }
                }
            }
            else if (Selection.mode == MODE_DRAW) {
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    // a new box at the mouse position
                    bodies.push_back({ CreateBox(worldId, { mWorldPos.x,mWorldPos.y }, {3.0f,3.0f}, b2_dynamicBody), RandomColor() });
                }
                else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
					// a new ball at the mouse position
					bodies.push_back({ CreateBall(worldId, { mWorldPos.x,mWorldPos.y }, 3.0f, b2_dynamicBody), RandomColor() });
                }
            }
            else if (Selection.mode == MODE_EXPLODE) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					b2ExplosionDef explosionDef = b2DefaultExplosionDef();
                    explosionDef.position = { mWorldPos.x,mWorldPos.y };
                    explosionDef.radius = 100.0f;
					explosionDef.impulsePerLength = 10000.0f;
					b2World_Explode(worldId, &explosionDef); // Explode at mouse position
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    b2ExplosionDef explosionDef = b2DefaultExplosionDef();
                    explosionDef.position = { mWorldPos.x,mWorldPos.y };
                    explosionDef.radius = 100.0f;
                    explosionDef.impulsePerLength = -10000.0f;
                    b2World_Explode(worldId, &explosionDef); // Explode at mouse position
                }
			}
        }

        // Simulate
        float deltaTime = GetFrameTime();
        if(!paused) b2World_Step(worldId, deltaTime, subStepCount);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(viewport);

        rlBegin(RL_TRIANGLES);
        for (RayBody& body : bodies) {
            body.draw();
            /*b2JointId jointArray[3];
            b2Body_GetJoints(body.id, jointArray, 3);*/
        }
        rlEnd();
        
        if (Selection.mode == MODE_WELD && Selection.numOfBodyIds) {
            if (b2Body_IsValid(Selection.bodyIds[0])) DrawAABB(b2Body_ComputeAABB(Selection.bodyIds[0]));
            if (b2Body_IsValid(Selection.bodyIds[1])) DrawAABB(b2Body_ComputeAABB(Selection.bodyIds[1]));
        }

        EndMode2D();
        if (OTCD) {
            const char* text = "Hold C to open tool menu";
            int measure = MeasureText(text, 40);
            DrawText(text, (screenWidth - measure) / 2, (screenHeight - 40) / 2, 40, {200,200,200,255});
        }
        spawnMenu.draw();
        toolMenu.draw();
        if (toolMenu.active) {
            OTCD = false;
		}

        Vector2 screenPos = GetWorldToScreen2D({ Selection.spawnPos.x, Selection.spawnPos.y }, viewport);
        if (spawnMenu.active) {
            DrawCircle(screenPos.x, screenPos.y, 10, { 120,120,120,255 });
            DrawCircle(screenPos.x, screenPos.y, 6, { 255,255,255,255 });
        }

        EndDrawing();
    }

    b2DestroyWorld(worldId); // Clear memory
    return 0;
}
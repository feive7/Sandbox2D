#define CIRCLE_TRI_COUNT 100
// Drawing
void DrawBody(b2BodyId id, Color color) {
    // Compute AABB
    b2ShapeId shapes[10];
    b2Body_GetShapes(id, shapes, 10);

    for (int i = 0; i < b2Body_GetShapeCount(id); i++) {
        b2ShapeId shape = shapes[i]; // Shape
        int shapeType = b2Shape_GetType(shape);
        b2Transform bodyTransform = b2Body_GetTransform(id);
        b2Vec2 bodyCenter = b2Body_GetPosition(id); // Get position of dynamic body
        b2Rot bodyRotation = b2Body_GetRotation(id); // Get rotation of dynamic body

        switch (shapeType) {
        case b2_polygonShape: {
            b2Polygon poly = b2Shape_GetPolygon(shape); // Polygon
            b2Vec2 centroid = poly.centroid;
			b2Vec2 rotOffset = b2RotateVector(bodyRotation, centroid); // Rotate centroid by body rotation

            // Get Transformation
            b2Transform transformation = b2Body_GetTransform(id);

            b2Transform unrotated;
            unrotated.p = transformation.p;
            unrotated.q = b2Rot_identity;

            b2AABB aabb = b2ComputePolygonAABB(&poly, unrotated);
            b2Vec2 extents = b2AABB_Extents(aabb);

            if (poly.count == 4) {
                DrawRectanglePro({ bodyCenter.x + rotOffset.x, bodyCenter.y + rotOffset.y , 2 * extents.x, 2 * extents.y }, { extents.x, extents.y }, b2Rot_GetAngle(bodyRotation) * RAD2DEG, ColorBrightness(color, -0.2f)); // Draw ground
                DrawRectanglePro({ bodyCenter.x + rotOffset.x, bodyCenter.y + rotOffset.y , 2 * extents.x - 2, 2 * extents.y - 2 }, { extents.x - 1, extents.y - 1 }, b2Rot_GetAngle(bodyRotation) * RAD2DEG, color); // Draw ground
            }
            else {
                DrawPoly({ bodyCenter.x + rotOffset.x, bodyCenter.y + rotOffset.y }, poly.count, 10, b2Rot_GetAngle(bodyRotation) * RAD2DEG, ColorBrightness(color, -0.2f)); // Draw ground
                DrawPoly({ bodyCenter.x + rotOffset.x, bodyCenter.y + rotOffset.y }, poly.count, 9, b2Rot_GetAngle(bodyRotation) * RAD2DEG, color); // Draw ground
            }
            break;
        }
        case b2_circleShape: {
            b2Circle circle = b2Shape_GetCircle(shape);
            b2Vec2 edge = b2Add(bodyTransform.p, b2MulSV(circle.radius, { bodyTransform.q.c,bodyTransform.q.s }));

            float r = circle.radius;
            //DrawCircleV({ center.x, center.y }, circle.radius, (b2Body_IsAwake(id) ? RED : GRAY)); // Draw circle
            b2Vec2 last = { 0.0f,0.0f };
            DrawCircleV({ bodyCenter.x, bodyCenter.y }, circle.radius, ColorBrightness(color, -0.2f)); // Draw circle border
            DrawCircleV({ bodyCenter.x, bodyCenter.y }, circle.radius - 1, color); // Draw inner circle
            DrawLineEx({ bodyCenter.x, bodyCenter.y }, { edge.x, edge.y }, 1.0f, ColorBrightness(color, -0.2f)); // Draw line from center to edge to show rotation
            break;
        }
        }
    }
}
void DrawBodySolid(b2BodyId id, Color color) {
    b2ShapeId shapes[10];
    b2Body_GetShapes(id, shapes, 10);

	rlColor4ub(color.r, color.g, color.b, color.a); // Set color for drawing
    for (int i = 0; i < b2Body_GetShapeCount(id); i++) {
        b2ShapeId shape = shapes[i]; // Shape
        int shapeType = b2Shape_GetType(shape);
        b2Transform bodyTransform = b2Body_GetTransform(id);
        b2Vec2 bodyCenter = b2Body_GetPosition(id); // Get position of dynamic body
        b2Rot bodyRotation = b2Body_GetRotation(id); // Get rotation of dynamic body
        switch (shapeType) {
        case b2_polygonShape: {
            b2Polygon poly = b2Shape_GetPolygon(shape); // Polygon
			b2Vec2 centroid = poly.centroid;
			for (int j = 0; j < poly.count; j++) {
                b2Vec2 v1 = b2Add(bodyCenter, b2RotateVector(bodyRotation, poly.vertices[j])); // Rotate vertex by body rotation
                b2Vec2 v2 = b2Add(bodyCenter, b2RotateVector(bodyRotation, poly.vertices[(j+1)%poly.count])); // Rotate vertex by body rotation
                rlVertex2f(v1.x, v1.y); // Vertex of the polygon
                rlVertex2f(centroid.x + bodyCenter.x, centroid.y + bodyCenter.y); // Center of the body
                rlVertex2f(v2.x, v2.y); // Vertex of the polygon
			}
            break;
        }
        case b2_circleShape: {
			b2Circle circle = b2Shape_GetCircle(shape);
			b2Vec2 edge = b2Add(bodyTransform.p, b2MulSV(circle.radius, { bodyTransform.q.c,bodyTransform.q.s }));
			float r = circle.radius;
			for (int j = 0; j < CIRCLE_TRI_COUNT; j++) {
                float angle = 2 * PI * j / CIRCLE_TRI_COUNT; // Angle for the circle segment
                b2Vec2 v1 = { r * cos(angle), r * sin(angle) }; // Vertex of the circle segment
                b2Vec2 v2 = { r * cos(angle + 2 * PI / CIRCLE_TRI_COUNT), r * sin(angle + 2 * PI / CIRCLE_TRI_COUNT) }; // Next vertex of the circle segment
                rlVertex2f(v1.x + bodyCenter.x, v1.y + bodyCenter.y); // Vertex of the circle segment
                rlVertex2f(edge.x, edge.y); // Edge of the circle
                rlVertex2f(v2.x + bodyCenter.x, v2.y + bodyCenter.y); // Next vertex of the circle segment
			}
            break;
        }
        }
    }
}
void DrawJoint(b2WorldId world, b2JointId joint) {
    if (!b2Joint_IsValid(joint)) return;
    b2BodyId bodyA = b2Joint_GetBodyA(joint);
    b2BodyId bodyB = b2Joint_GetBodyB(joint);

    b2Transform bodyATransform = b2Body_GetTransform(bodyA);
    b2Transform bodyBTransform = b2Body_GetTransform(bodyB);

    b2Vec2 localFrameA = b2Joint_GetLocalAnchorA(joint);
    b2Vec2 localFrameB = b2Joint_GetLocalAnchorB(joint);

    // Combine localFrameA with bodyA's transform to get world anchor A
    b2Vec2 anchorA = b2TransformPoint(bodyATransform, localFrameA);

    // Combine localFrameB with bodyB's transform to get world anchor B
    b2Vec2 anchorB = b2TransformPoint(bodyBTransform, localFrameB);

    DrawLine(anchorA.x, anchorA.y, anchorB.x, anchorB.y, BLACK);
}


void DrawAABB(b2AABB aabb) {
    b2Vec2 extents = b2MulSV(2.0f,b2AABB_Extents(aabb));
    DrawRectangleLines(aabb.lowerBound.x, aabb.lowerBound.y, extents.x, extents.y, LIME);
}

// Extras
Color RandomColor() {
    unsigned char r = GetRandomValue(0, 255);
    unsigned char g = GetRandomValue(0, 255);
    unsigned char b = GetRandomValue(0, 255);
    return { r,g,b,255 };
}

struct RayBody {
    b2BodyId id;
    Color color;
    void draw() {
        DrawBody(id, color);
    }
};
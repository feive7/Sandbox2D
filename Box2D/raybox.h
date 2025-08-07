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

            //DrawRectanglePro({ position.x, position.y, 2 * extents.x, 2 * extents.y }, { extents.x, extents.y }, b2Rot_GetAngle(rotation) * RAD2DEG, (b2Body_IsAwake(id) ? RED : GRAY)); // Draw ground
            DrawRectanglePro({ bodyCenter.x + rotOffset.x, bodyCenter.y + rotOffset.y , 2 * extents.x, 2 * extents.y }, { extents.x, extents.y }, b2Rot_GetAngle(bodyRotation)* RAD2DEG, ColorBrightness(color, -0.2f)); // Draw ground
			DrawRectanglePro({ bodyCenter.x + rotOffset.x, bodyCenter.y + rotOffset.y , 2 * extents.x - 2, 2 * extents.y - 2 }, { extents.x - 1, extents.y - 1 }, b2Rot_GetAngle(bodyRotation) * RAD2DEG, color); // Draw ground
            DrawCircleV({ bodyCenter.x + rotOffset.x, bodyCenter.y + rotOffset.y }, 2, ColorBrightness(color, -0.2f)); // Draw center circle
            break;
        }
        case b2_circleShape: {
            b2Circle circle = b2Shape_GetCircle(shape);
            b2Vec2 edge = b2Add(bodyTransform.p, b2MulSV(circle.radius, { bodyTransform.q.c,bodyTransform.q.s }));
            //DrawCircleV({ center.x, center.y }, circle.radius, (b2Body_IsAwake(id) ? RED : GRAY)); // Draw circle
            DrawCircleV({ bodyCenter.x, bodyCenter.y }, circle.radius, ColorBrightness(color, -0.2f)); // Draw circle border
            DrawCircleV({ bodyCenter.x, bodyCenter.y }, circle.radius - 1, color); // Draw inner circle
            DrawLineEx({ bodyCenter.x, bodyCenter.y }, { edge.x, edge.y }, 1.0f, ColorBrightness(color, -0.2f)); // Draw line from center to edge to show rotation
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

    b2Transform localFrameA = b2Joint_GetLocalFrameA(joint);
    b2Transform localFrameB = b2Joint_GetLocalFrameB(joint);

    // Combine localFrameA with bodyA's transform to get world anchor A
    b2Vec2 anchorA = b2TransformPoint(bodyATransform, localFrameA.p);

    // Combine localFrameB with bodyB's transform to get world anchor B
    b2Vec2 anchorB = b2TransformPoint(bodyBTransform, localFrameB.p);

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
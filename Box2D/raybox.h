// Raylib + box2d Compatibility
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

            //DrawRectanglePro({ position.x, position.y, 2 * extents.x, 2 * extents.y }, { extents.x, extents.y }, b2Rot_GetAngle(rotation) * RAD2DEG, (b2Body_IsAwake(id) ? RED : GRAY)); // Draw ground
            DrawRectanglePro({ position.x, position.y, 2 * extents.x, 2 * extents.y }, { extents.x, extents.y }, b2Rot_GetAngle(rotation) * RAD2DEG, color); // Draw ground
            break;
        }
        case b2_circleShape: {
            b2Circle circle = b2Shape_GetCircle(shape);
            b2Transform transformation = b2Body_GetTransform(id);
            b2Vec2 center = b2Body_GetPosition(id); // Get position of dynamic body
            b2Rot rotation = b2Body_GetRotation(id); // Get rotation of dynamic body
            b2Vec2 edge = b2Add(center, b2MulSV(circle.radius, { rotation.c,rotation.s }));
            //DrawCircleV({ center.x, center.y }, circle.radius, (b2Body_IsAwake(id) ? RED : GRAY)); // Draw circle
            DrawCircleV({ center.x, center.y }, circle.radius, color); // Draw circle
            DrawLineV({ center.x, center.y }, { edge.x, edge.y }, ColorBrightness(color,-0.5f));
            break;
        }
        }
    }
}

// Extras
Color RandomColor() {
    unsigned char r = GetRandomValue(0, 255);
    unsigned char g = GetRandomValue(0, 255);
    unsigned char b = GetRandomValue(0, 255);
    return { r,g,b,255 };
}
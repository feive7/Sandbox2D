// Basic creation
b2BodyId CreateBox(b2WorldId worldId, b2Vec2 position, b2Vec2 extent, bool dynamic) {
    b2BodyDef boxBodyDef = b2DefaultBodyDef(); // Create body
    boxBodyDef.position = position; // Set position
    if (dynamic) boxBodyDef.type = b2_dynamicBody;
    b2BodyId boxBodyId = b2CreateBody(worldId, &boxBodyDef);

    b2Polygon box = b2MakeBox(extent.x, extent.y); // Create box polygon
    b2ShapeDef boxShape = b2DefaultShapeDef(); // Create box shape
    boxShape.density = 1.0f; // Density required nonzero

    b2CreatePolygonShape(boxBodyId, &boxShape, &box); // Bind it all together

    return boxBodyId; // Return the id
}
b2BodyId CreateBall(b2WorldId worldId, b2Vec2 center, float radius, bool dynamic) {
    b2BodyDef ballBodyDef = b2DefaultBodyDef(); // Create body
    ballBodyDef.position = center; // Set position
    if (dynamic) ballBodyDef.type = b2_dynamicBody;
    b2BodyId ballBodyId = b2CreateBody(worldId, &ballBodyDef);

    b2Circle ball;
    ball.center = { 0.0f,0.0f };
    ball.radius = radius;

    b2ShapeDef ballShape = b2DefaultShapeDef();
    ballShape.density = 1.0f; // Density required nonzero

    b2CreateCircleShape(ballBodyId, &ballShape, &ball); // Bind it all together

    return ballBodyId; // Return the id
}
b2BodyId CreateCup(b2WorldId worldId, b2Vec2 center, b2Vec2 extent, bool dynamic) {
    // Create body
    b2BodyDef cupBodyDef = b2DefaultBodyDef();
    cupBodyDef.position = center;
    if (dynamic) cupBodyDef.type = b2_dynamicBody;
    b2BodyId cupBodyId = b2CreateBody(worldId, &cupBodyDef);

    // Create bottom wall shape
    b2Polygon bottom = b2MakeOffsetBox(extent.x + 4.0f, 2.0f, { 0.0f,-extent.y - 2.0f }, b2Rot_identity);
    b2ShapeDef bottomShape = b2DefaultShapeDef();
    bottomShape.density = 1.0f;

    // Create left side shape
    b2Polygon leftside = b2MakeOffsetBox(2.0f, extent.y, { -extent.x-2.0f,0.0f }, b2Rot_identity);
    b2ShapeDef leftsideShape = b2DefaultShapeDef();
    leftsideShape.density = 1.0f;

    // Create left side shape
    b2Polygon rightside = b2MakeOffsetBox(2.0f, extent.y, { extent.x+2.0f,0.0f }, b2Rot_identity);
    b2ShapeDef rightsideShape = b2DefaultShapeDef();
    rightsideShape.density = 1.0f;

    // Bind it all together
    b2CreatePolygonShape(cupBodyId, &bottomShape, &bottom);
    b2CreatePolygonShape(cupBodyId, &leftsideShape, &leftside);
    b2CreatePolygonShape(cupBodyId, &rightsideShape, &rightside);

    return cupBodyId;
}
b2BodyId CreateHollowBox(b2WorldId worldId, b2Vec2 center, b2Vec2 extent, bool dynamic) {
    // Create body
    b2BodyDef hollowBoxBodyDef = b2DefaultBodyDef();
    hollowBoxBodyDef.position = center;
    if (dynamic) hollowBoxBodyDef.type = b2_dynamicBody;
    b2BodyId hollowBoxBodyId = b2CreateBody(worldId, &hollowBoxBodyDef);

    // Create bottom wall shape
    b2Polygon bottom = b2MakeOffsetBox(extent.x + 4.0f, 2.0f, { 0.0f,-extent.y - 2.0f }, b2Rot_identity);
    b2ShapeDef bottomShape = b2DefaultShapeDef();
    bottomShape.density = 1.0f;

    // Create top wall shape
    b2Polygon top = b2MakeOffsetBox(extent.x + 4.0f, 2.0f, { 0.0f,extent.y + 2.0f }, b2Rot_identity);
    b2ShapeDef topShape = b2DefaultShapeDef();
    topShape.density = 1.0f;

    // Create left side shape
    b2Polygon leftside = b2MakeOffsetBox(2.0f, extent.y, { -extent.x - 2.0f,0.0f }, b2Rot_identity);
    b2ShapeDef leftsideShape = b2DefaultShapeDef();
    leftsideShape.density = 1.0f;

    // Create left side shape
    b2Polygon rightside = b2MakeOffsetBox(2.0f, extent.y, { extent.x + 2.0f,0.0f }, b2Rot_identity);
    b2ShapeDef rightsideShape = b2DefaultShapeDef();
    rightsideShape.density = 1.0f;

    // Bind it all together
    b2CreatePolygonShape(hollowBoxBodyId, &bottomShape, &bottom);
    b2CreatePolygonShape(hollowBoxBodyId, &topShape, &top);
    b2CreatePolygonShape(hollowBoxBodyId, &leftsideShape, &leftside);
    b2CreatePolygonShape(hollowBoxBodyId, &rightsideShape, &rightside);

    return hollowBoxBodyId;
}

// Constraints & Joints
void HingeBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, float length) {
    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef(); // Create joint definition
    jointDef.base.bodyIdA = bodyA; // Set first body
    jointDef.base.bodyIdB = bodyB; // Set second body
    jointDef.length = length; // Set length
    b2CreateDistanceJoint(id, &jointDef); // Create the joint
}
void HingeBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB) {
    HingeBodies(id, bodyA, bodyB, b2Distance(b2Body_GetPosition(bodyA), b2Body_GetPosition(bodyB)));
}
void WeldBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, b2Vec2 anchor) {
    b2WeldJointDef jointDef = b2DefaultWeldJointDef(); // Create joint definition
    jointDef.base.bodyIdA = bodyA; // Set first body
    jointDef.base.bodyIdB = bodyB; // Set second body
    jointDef.base.localFrameA.p = anchor; // Set first local frame
    jointDef.base.localFrameB.p = anchor; // Set second local frame
    b2CreateWeldJoint(id, &jointDef); // Create the joint
}
void RevoluteJointBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, b2Vec2 anchor) {
    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef(); // Create joint definition
    jointDef.base.bodyIdA = bodyA; // Set first body
    jointDef.base.bodyIdB = bodyB; // Set second body
    jointDef.base.localFrameA.p = anchor; // Set first local frame
    jointDef.base.localFrameB.p = anchor; // Set second local frame
    b2CreateRevoluteJoint(id, &jointDef); // Create the joint
}
void MouseJointBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, b2Vec2 anchor) {
    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef(); // Create joint definition
    jointDef.base.bodyIdA = bodyA; // Set first body
    jointDef.base.bodyIdB = bodyB; // Set second body
    jointDef.base.localFrameA.p = anchor; // Set first local frame
    jointDef.base.localFrameB.p = anchor; // Set second local frame
    jointDef.enableMotor = true; // Enable motor for dragging
    jointDef.maxMotorTorque = 1000.0f; // Set max motor torque
    b2CreateRevoluteJoint(id, &jointDef); // Create the joint
}

// Misc
void DragBody(b2BodyId bodyId, b2Vec2 point, float force = 100.0f) {
    b2Vec2 bodyPos = b2Body_GetPosition(bodyId);

    b2Vec2 vel = { point.x - bodyPos.x, point.y - bodyPos.y };
    b2Vec2 final = b2MulSV(force, vel);

    b2Body_SetLinearVelocity(bodyId, final);
}
void DragBody(b2BodyId bodyId, b2Vec2 point, b2Vec2 pointOnBody, float force = 100.0f) {
    b2Vec2 bodyPos = b2Body_GetWorldPoint(bodyId,pointOnBody);

    b2Vec2 vel = { point.x - bodyPos.x, point.y - bodyPos.y };
    b2Vec2 final = b2MulSV(force, vel);

    b2Body_SetLinearVelocity(bodyId, final);
}
bool AABBContains(b2AABB aabb, b2Vec2 vec2) {
    if (aabb.lowerBound.x < vec2.x && aabb.lowerBound.y < vec2.y && aabb.upperBound.x > vec2.x && aabb.upperBound.y > vec2.y)
        return true;
    return false;
}
bool BodyContains(b2BodyId bodyId, b2Vec2 vec2) {
    b2ShapeId shapeArray[10];
    b2Body_GetShapes(bodyId, shapeArray, 10);
    for (int i = 0; i < b2Body_GetShapeCount(bodyId); i++) {
        if (b2Shape_TestPoint(shapeArray[i], vec2)) {
            return true;
        }
    }
    return false;
}
void BodyFreeze(b2BodyId bodyId) {
	b2Body_SetType(bodyId, b2_staticBody); // Set body type to static
}
void BodyUnfreeze(b2BodyId bodyId) {
    b2Body_SetType(bodyId, b2_dynamicBody); // Set body type to dynamic
}
b2AABB b2Body_ComputeAABBEx(b2BodyId bodyId) {
    b2ShapeId shapes[10];
    b2Body_GetShapes(bodyId, shapes, 10); // Get shapes of the body
    b2AABB aabb = { { FLT_MAX, FLT_MAX }, { -FLT_MAX, -FLT_MAX } }; // Initialize AABB
	b2Transform transform = b2Body_GetTransform(bodyId); // Get body transform
	transform.q = b2Rot_identity; // Set rotation to identity for AABB computation
    for (int i = 0; i < b2Body_GetShapeCount(bodyId); i++) {
        b2ShapeId shape = shapes[i]; // Get shape
        int shapeType = b2Shape_GetType(shape); // Get shape type
        switch (shapeType) {
        case b2_polygonShape: {
            b2Polygon poly = b2Shape_GetPolygon(shape); // Get polygon
            aabb = b2AABB_Union(aabb, b2ComputePolygonAABB(&poly, transform)); // Combine AABBs
            break;
        }
        case b2_circleShape: {
            b2Circle circle = b2Shape_GetCircle(shape); // Get circle
            aabb = b2AABB_Union(aabb, b2ComputeCircleAABB(&circle, transform)); // Combine AABBs
            break;
        }
        }
    }
    return aabb; // Return the computed AABB
}
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
    b2PrismaticJointDef jointDef = b2DefaultPrismaticJointDef(); // Create joint definition
    jointDef.base.bodyIdA = bodyA; // Set first body
    jointDef.base.bodyIdB = bodyB; // Set second body
    jointDef.base.localFrameA.p = anchor; // Set first local frame
    jointDef.base.localFrameB.p = anchor; // Set second local frame
    jointDef.maxMotorForce = 100.0f; // Set maximum motor force
    jointDef.hertz = 10.0f; // Set hertz
    b2CreatePrismaticJoint(id, &jointDef); // Create the joint
}

// Misc
void DragBody(b2BodyId bodyId, b2Vec2 point, float force = 100.0f) {
    b2Vec2 bodyPos = b2Body_GetPosition(bodyId);

    b2Vec2 vel = { point.x - bodyPos.x, point.y - bodyPos.y };
    b2Vec2 final = b2MulSV(force, vel);

    b2Body_SetLinearVelocity(bodyId, final);
}
bool AABBContains(b2AABB aabb, b2Vec2 vec2) {
    if (aabb.lowerBound.x < vec2.x && aabb.lowerBound.y < vec2.y && aabb.upperBound.x > vec2.x && aabb.upperBound.y > vec2.y)
        return true;
    return false;
}

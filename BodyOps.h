// Basic creation
b2BodyId CreateBox(b2WorldId worldId, b2Vec2 position, b2Vec2 extent, b2BodyType bodyType) {
    b2BodyDef boxBodyDef = b2DefaultBodyDef(); // Create body
    boxBodyDef.position = position; // Set position
    boxBodyDef.type = bodyType;
    b2BodyId boxBodyId = b2CreateBody(worldId, &boxBodyDef);

    b2Polygon box = b2MakeBox(extent.x, extent.y); // Create box polygon
    b2ShapeDef boxShape = b2DefaultShapeDef(); // Create box shape
    boxShape.density = 1.0f; // Density required nonzero

    b2CreatePolygonShape(boxBodyId, &boxShape, &box); // Bind it all together

    return boxBodyId; // Return the id
}
b2BodyId CreateBall(b2WorldId worldId, b2Vec2 center, float radius, b2BodyType bodyType) {
    b2BodyDef ballBodyDef = b2DefaultBodyDef(); // Create body
    ballBodyDef.position = center; // Set position
    ballBodyDef.type = bodyType;
    b2BodyId ballBodyId = b2CreateBody(worldId, &ballBodyDef);

    b2Circle ball;
    ball.center = { 0.0f,0.0f };
    ball.radius = radius;

    b2ShapeDef ballShape = b2DefaultShapeDef();
    ballShape.density = 1.0f; // Density required nonzero

    b2CreateCircleShape(ballBodyId, &ballShape, &ball); // Bind it all together

    return ballBodyId; // Return the id
}
b2BodyId CreateCup(b2WorldId worldId, b2Vec2 center, b2Vec2 extent, b2BodyType bodyType) {
    // Create body
    b2BodyDef cupBodyDef = b2DefaultBodyDef();
    cupBodyDef.position = center;
    cupBodyDef.type = bodyType;
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
b2BodyId CreateHollowBox(b2WorldId worldId, b2Vec2 center, b2Vec2 extent, b2BodyType bodyType) {
    // Create body
    b2BodyDef hollowBoxBodyDef = b2DefaultBodyDef();
    hollowBoxBodyDef.position = center;
    hollowBoxBodyDef.type = bodyType;
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
b2BodyId CreatePolygon(b2WorldId worldId, b2Vec2 position, b2Hull hull, b2BodyType bodyType) {
    b2BodyDef polyBodyDef = b2DefaultBodyDef();
    polyBodyDef.position = position;
    polyBodyDef.type = bodyType;
    b2BodyId polyBodyId = b2CreateBody(worldId, &polyBodyDef);


	b2Polygon polygon = b2MakePolygon(&hull, 0); // Create polygon from hull

    b2ShapeDef polyShapeDef = b2DefaultShapeDef();
    polyShapeDef.density = 1.0f; // Density required nonzero

    b2CreatePolygonShape(polyBodyId, &polyShapeDef, &polygon);

	return polyBodyId; // Return the id
}
b2BodyId CreateRegularPolygon(b2WorldId worldId, b2Vec2 position, int sides, float radius, b2BodyType bodyType) {
    b2Vec2 verts[10];
	float angOffset = PI / (float)sides - PI / 2.0f; // Offset so the bottom of the polygon is flat
    for (int i = 0; i < sides; i++) {
        float t = 2 * PI * i / (float)sides;
        verts[i] = { radius * cos(t + angOffset), radius * sin(t + angOffset) };
    }
    b2Hull hull = b2ComputeHull(verts, sides); // Compute the convex hull from the vertices

	return CreatePolygon(worldId, position, hull, bodyType); // Create the polygon body
}
b2BodyId CreateRock(b2WorldId worldId, b2Vec2 position, b2BodyType bodyType) {
    b2Vec2 verts[8];
    for (int i = 0; i < 8; i++) {
        float radius = GetRandomValue(5,20); // Random radius between 5 and 20
        float t = 2 * PI * i / 8.0f;
        verts[i] = { radius * cos(t), radius * sin(t) };
    }
    b2Hull hull = b2ComputeHull(verts, 8.0f); // Compute the convex hull from the vertices

    return CreatePolygon(worldId, position, hull, bodyType); // Create the polygon body
}
b2BodyId CreateSegment(b2WorldId worldId, b2Vec2 start, b2Vec2 end, b2BodyType bodyType) {
	b2Vec2 midpoint = b2MulSV(.5,b2Add(start, end)); // Midpoint of the segment

    b2BodyDef segmentBodyDef = b2DefaultBodyDef(); // Create body
    segmentBodyDef.position = midpoint; // Set position
    segmentBodyDef.type = bodyType;
    b2BodyId segmentBodyId = b2CreateBody(worldId, &segmentBodyDef);
    b2Segment segment = { b2Sub(start,midpoint), b2Sub(end,midpoint) }; // Create segment
    b2ShapeDef segmentShapeDef = b2DefaultShapeDef(); // Create shape definition
    segmentShapeDef.density = 1.0f; // Density required nonzero
    b2CreateSegmentShape(segmentBodyId, &segmentShapeDef, &segment); // Bind it all together
    return segmentBodyId; // Return the id
}

// Constraints & Joints
void DistanceJointBodies(b2WorldId worldId, b2BodyId bodyA, b2BodyId bodyB, float length, bool collideConnected, float dampingRatio, bool enableLimit, bool enableMotor, bool enableSpring, float hertz, b2Vec2 localAnchorA, b2Vec2 localAnchorB, float maxLength, float maxMotorForce, float minLength, float motorSpeed) {
	b2DistanceJointDef jointDef = b2DefaultDistanceJointDef(); // Create joint definition
	jointDef.bodyIdA = bodyA; // Set first body
	jointDef.bodyIdB = bodyB; // Set second body
	jointDef.length = length; // Set length
	jointDef.localAnchorA = localAnchorA; // Set first local frame
	jointDef.localAnchorB = localAnchorB; // Set second local frame
	jointDef.collideConnected = collideConnected; // Set collision flag
	jointDef.dampingRatio = dampingRatio; // Set damping ratio
	jointDef.enableLimit = enableLimit; // Enable limit
	jointDef.enableMotor = enableMotor; // Enable motor
	jointDef.enableSpring = enableSpring; // Enable spring
	jointDef.hertz = hertz; // Set hertz
	jointDef.maxLength = maxLength; // Set maximum length
	jointDef.maxMotorForce = maxMotorForce; // Set maximum motor force
	jointDef.minLength = minLength; // Set minimum length
	jointDef.motorSpeed = motorSpeed; // Set motor speed
	b2CreateDistanceJoint(worldId, &jointDef); // Create the joint
}
void DistanceJointBodies(b2WorldId worldId, b2BodyId bodyA, b2BodyId bodyB, float length) {
    DistanceJointBodies(
        worldId, 
        bodyA, 
        bodyB, 
        length, 
		false, // Collide connected
		0.0f, // Damping ratio
		true, // Enable limit
		false, // Enable motor
		true, // Enable spring
        1.0f, // Hertz
		{ 0.0f,0.0f }, // Local anchor A
		{ 0.0f,0.0f }, // Local anchor B
		length * 2.0f, // Maximum length
		100.0f, // Maximum motor force
		0.0f, // Minimum length
		0.0f); // Motor speed
}
void HingeBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, float length) {
    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef(); // Create joint definition
    jointDef.bodyIdA = bodyA; // Set first body
    jointDef.bodyIdB = bodyB; // Set second body
    jointDef.length = length; // Set length
    b2CreateDistanceJoint(id, &jointDef); // Create the joint
}
void HingeBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB) {
    HingeBodies(id, bodyA, bodyB, b2Distance(b2Body_GetPosition(bodyA), b2Body_GetPosition(bodyB)));
}
void WeldBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, b2Vec2 anchorA, b2Vec2 anchorB) {
    b2Rot rotA = b2Body_GetRotation(bodyA);
    b2Rot rotB = b2Body_GetRotation(bodyB);
    float angleA = b2Rot_GetAngle(rotA);
    float angleB = b2Rot_GetAngle(rotB);

    b2WeldJointDef jointDef = b2DefaultWeldJointDef(); // Create joint definition
    jointDef.bodyIdA = bodyA; // Set first body
    jointDef.bodyIdB = bodyB; // Set second body
    jointDef.localAnchorA = anchorA; // Set first local frame
    jointDef.localAnchorB = anchorB; // Set second local frame
    jointDef.referenceAngle = angleB - angleA;
    b2JointId jointId = b2CreateWeldJoint(id, &jointDef); // Create the joint

    b2Joint_WakeBodies(jointId);

    // Unfreeze body that is being welded
    b2Body_SetType(bodyA, b2_dynamicBody);
}
void WeldBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB) {
    b2Vec2 posA = b2Body_GetPosition(bodyA);
    b2Vec2 posB = b2Body_GetPosition(bodyB);
    b2Vec2 midpoint = b2MulSV(.5,b2Add(posA,posB));
    b2Vec2 localA = b2Body_GetLocalPoint(bodyA, midpoint);
    b2Vec2 localB = b2Body_GetLocalPoint(bodyB, midpoint);
    WeldBodies(id, bodyA, bodyB, localA, localB);
}
void WheelBodies(b2WorldId id, b2BodyId bodyBody, b2BodyId wheelBody, b2Vec2 anchor) {
    b2WheelJointDef jointDef = b2DefaultWheelJointDef(); // Create joint definition
    jointDef.bodyIdA = bodyBody; // Set first body
    jointDef.bodyIdB = wheelBody; // Set second body
    jointDef.localAnchorA = anchor; // Set first local frame
    jointDef.hertz = 10.0f;
    jointDef.enableMotor = true; // Enable motor for wheel
    jointDef.maxMotorTorque = 1000.0f; // Set max motor torque
	b2CreateWheelJoint(id, &jointDef); // Create the joint
}
void RevoluteJointBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, b2Vec2 anchor) {
    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef(); // Create joint definition
    jointDef.bodyIdA = bodyA; // Set first body
    jointDef.bodyIdB = bodyB; // Set second body
    jointDef.localAnchorA = anchor; // Set first local frame
    jointDef.localAnchorB = anchor; // Set second local frame
    b2CreateRevoluteJoint(id, &jointDef); // Create the joint
}
void MouseJointBodies(b2WorldId id, b2BodyId bodyA, b2BodyId bodyB, b2Vec2 anchor) {
    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef(); // Create joint definition
    jointDef.bodyIdA = bodyA; // Set first body
    jointDef.bodyIdB = bodyB; // Set second body
    jointDef.localAnchorA = anchor; // Set first local frame
    jointDef.localAnchorB = anchor; // Set second local frame
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
void BodySetTargetRotation(b2BodyId bodyId, b2Rot newRot, b2Vec2 localPoint, b2Vec2 pivotPoint) {
    // Get world position of the local point using new rotation
    b2Vec2 rotatedLocal = b2RotateVector(newRot, localPoint);
    b2Vec2 newPos = b2Sub(pivotPoint, rotatedLocal);

    // Apply new transform target
    b2Body_SetTargetTransform(bodyId, { newPos, newRot }, 0.01f);
}
void BodyRotate(b2BodyId bodyId, float angleDelta, b2Vec2 localPoint, b2Vec2 pivotPoint) {
    // Get current rotation
    b2Rot rot = b2Body_GetRotation(bodyId);

    // Rotate by a small amount per frame (negative for A key)
    float angleStep = angleDelta; // radians per frame
    b2Rot newRot = b2MulRot(rot, b2MakeRot(angleStep));
    BodySetTargetRotation(bodyId, newRot, localPoint, pivotPoint);
}
struct ShapeData {
	b2ShapeDef def; // Shape definition
	b2ShapeType type; 
	b2Polygon poly;
	b2Circle circle;
	b2Segment segment;
};
struct BodyData {
	b2BodyDef def; // Body definition
	std::vector<ShapeData> shapes; // Shapes attached to the body
	Color color;
};
BodyData SaveBody(RayBody body) {
	BodyData bodyData;
	bodyData.def = b2DefaultBodyDef();
	bodyData.def.position = b2Body_GetPosition(body.id);
	bodyData.def.rotation = b2Body_GetRotation(body.id);
	bodyData.def.type = b2Body_GetType(body.id);
	bodyData.def.name = b2Body_GetName(body.id);
	bodyData.def.userData = b2Body_GetUserData(body.id);
	bodyData.def.linearVelocity = b2Body_GetLinearVelocity(body.id);
	bodyData.def.angularVelocity = b2Body_GetAngularVelocity(body.id);
	bodyData.def.linearDamping = b2Body_GetLinearDamping(body.id);
	bodyData.def.angularDamping = b2Body_GetAngularVelocity(body.id);
	bodyData.def.gravityScale = b2Body_GetGravityScale(body.id);
	bodyData.def.sleepThreshold = b2Body_GetSleepThreshold(body.id);
	bodyData.def.enableSleep = b2Body_IsSleepEnabled(body.id);
	bodyData.def.isAwake = b2Body_IsAwake(body.id);
	bodyData.def.fixedRotation = b2Body_IsFixedRotation(body.id);
	bodyData.def.isBullet = b2Body_IsBullet(body.id);
	bodyData.def.isEnabled = b2Body_IsEnabled(body.id);
	bodyData.def.allowFastRotation = false; // Can't get

	b2ShapeId shapes[10];
	b2Body_GetShapes(body.id, shapes, 10);
	for (int i = 0; i < b2Body_GetShapeCount(body.id); i++) {
		ShapeData shapeData;

		b2ShapeId shapeId = shapes[i];
		shapeData.def = b2DefaultShapeDef();
		shapeData.def.userData = b2Shape_GetUserData(shapeId);
		shapeData.def.material = b2Shape_GetSurfaceMaterial(shapeId);
		shapeData.def.density = b2Shape_GetDensity(shapeId);
		shapeData.def.filter = b2Shape_GetFilter(shapeId);
		shapeData.def.isSensor = b2Shape_IsSensor(shapeId);
		shapeData.def.enableSensorEvents = b2Shape_AreSensorEventsEnabled(shapeId);
		shapeData.def.enableContactEvents = b2Shape_AreContactEventsEnabled(shapeId);
		shapeData.def.enableHitEvents = b2Shape_AreHitEventsEnabled(shapeId);
		shapeData.def.enablePreSolveEvents = b2Shape_ArePreSolveEventsEnabled(shapeId);

		b2ShapeType type = b2Shape_GetType(shapeId);
		switch (type) {
		case b2_polygonShape: shapeData.poly = b2Shape_GetPolygon(shapeId); break;
		case b2_circleShape: shapeData.circle = b2Shape_GetCircle(shapeId); break;
		case b2_segmentShape: shapeData.segment = b2Shape_GetSegment(shapeId); break;
		}

		bodyData.shapes.push_back(shapeData);
	}

	bodyData.color = body.color;
	return bodyData;
}
std::vector<BodyData> SaveScene(std::vector<RayBody>& bodies) {
	std::vector<BodyData> bodyDataList;
	for(RayBody& body : bodies) {
		BodyData bodyData = SaveBody(body);
		bodyDataList.push_back(bodyData);
	}
	return bodyDataList;
}
RayBody LoadBody(b2WorldId worldId, const BodyData& bodyData) {
	b2BodyDef bodyDef = bodyData.def;
	b2BodyId bodyId = b2CreateBody(worldId, &bodyDef); // Create the body in the world

	for (const ShapeData& shapeData : bodyData.shapes) {
		switch (shapeData.type) {
			case b2_polygonShape: {
				b2Polygon polygon = shapeData.poly;
				b2CreatePolygonShape(bodyId, &shapeData.def, &polygon);
				break;
			}
			case b2_circleShape: {
				b2Circle circle = shapeData.circle;
				b2CreateCircleShape(bodyId, &shapeData.def, &circle);
				break;
			}
			case b2_segmentShape: {
				b2Segment segment = shapeData.segment;
				b2CreateSegmentShape(bodyId, &shapeData.def, &segment);
				break;
			}
			default:
				break; // Handle other shape types as needed
		}
	}

	return { bodyId, bodyData.color };
}
std::vector<RayBody> LoadScene(b2WorldId worldId, std::vector<BodyData> bodies) {
	std::vector<RayBody> raybodies;
	for (BodyData bodyData : bodies) {
		raybodies.push_back(LoadBody(worldId, bodyData));
	}
	return raybodies;
}
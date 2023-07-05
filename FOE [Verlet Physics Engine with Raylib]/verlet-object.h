#pragma once
#include <raylib.h>
#include "vector2.h"

//struct VerletObject {
//	Vec2 current_position;
//	Vec2 old_position;
//	Vec2 objectAcceleration;
//	Vec2 velocity;
//	float objectRadius;
//	float objectMass;
//	Color objectColor;
//
//	VerletObject(Vec2 start_position, float radius, float mass, Color color) : 
//		current_position(start_position), 
//		old_position(start_position), 
//		objectAcceleration({ 0.0f, 1000.0f }), 
//		objectRadius(radius), 
//		objectMass(mass), 
//		objectColor(color)
//	{}
//
//	void Update(float deltaTime) {
//		const Vec2 displacement = current_position - old_position;
//		old_position = current_position;
//		current_position = current_position + displacement + objectAcceleration * (deltaTime * deltaTime);
//		velocity = (current_position - old_position) / deltaTime;
//		objectAcceleration = {};
//	}
//
//	void SetVelocity(Vec2 velocity, float deltaTime) {
//		old_position = current_position - (velocity * deltaTime);
//	}
//
//	void Accelerate(Vec2 acceleration) {
//		objectAcceleration = objectAcceleration + acceleration;
//	}
//};

class VerletObject {

public:
	Vec2 current_position;
	Vec2 old_position;
	Vec2 objectAcceleration;
	Vec2 velocity;
	IntVec2 cellPosition;
	float objectRadius;
	float objectMass;
	Color objectColor;

	VerletObject(Vec2 start_position, float radius, float mass, Color color) :
		current_position(start_position),
		old_position(start_position),
		objectAcceleration({ 0.0f, 1000.0f }),
		objectRadius(radius),
		objectMass(mass),
		objectColor(color)
	{}

	void Update(float deltaTime) {
		const Vec2 displacement = current_position - old_position;
		old_position = current_position;
		current_position = current_position + displacement + objectAcceleration * (deltaTime * deltaTime);
		velocity = (current_position - old_position) / deltaTime;
		objectAcceleration = {0.0f, 0.0f};
	}

	void SetVelocity(Vec2 set_velocity, float deltaTime) {
		old_position = current_position - (set_velocity * deltaTime);
	}

	void Accelerate(Vec2 acceleration) {
		objectAcceleration = objectAcceleration + acceleration;
	}

	friend bool operator== (const VerletObject& object1, const VerletObject& object2);
};

inline bool operator== (const VerletObject& object1, const VerletObject& object2) {
	const bool samePos = (object1.current_position == object2.current_position);
	const bool sameCell = (object1.cellPosition == object2.cellPosition);
	const bool sameAcceleration = (object1.objectAcceleration == object2.objectAcceleration);
	const bool sameMass = ((object1.objectMass - object2.objectMass) < FLT_EPSILON);
	const bool sameRadius = ((object1.objectRadius - object2.objectRadius) < FLT_EPSILON);
	const bool sameOldPos = (object1.old_position == object2.old_position);
	const bool sameVelocity = (object1.velocity == object2.velocity);

	return (samePos && sameCell && sameAcceleration && sameMass && sameRadius && sameOldPos && sameVelocity) == true;
}
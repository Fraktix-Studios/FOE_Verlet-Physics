#include <raylib.h>
#include <vector>
#include "verlet-object.h"
#include "vector2.h"
#include <cmath>
#include "spatial-solver.h"
#include <string>

#include "main.h"

using namespace std;
using namespace foe;

void setUpdateRate(const float rate) {
	stepDeltaTime = 1.0f / rate;
}

int main(void) {
	// SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(windowWidth, windowHeight, "FOE [Verlet Physics Engine]");
	SetTargetFPS(windowFPS);
	setUpdateRate(windowFPS);

	while (!WindowShouldClose()) {

		SpatialSplitUpdate();

		spawnTime += GetFrameTime();
		const auto overall_time = static_cast<float>(GetTime());
		// objectDelay = 1 / GetTime();

		BeginDrawing();
		ClearBackground(Color{ 24, 24, 24, 255 });

		if (IsKeyDown(KEY_SPACE)) {
			if (SpatialSplitGetObjects() < objectAmount && spawnTime >= spawnDelay) {
				spawnTime = 0;

				for (int i = 0; i < objectGroupAmount; i++) {
					// VerletObject object(start_position, static_cast<float>(GetRandomValue(minRadius, maxRadius)), 100, getRainbow(overallTime));
					// VerletObject object({ static_cast<float>(startPosition.x), static_cast<float>(startPosition.y) + static_cast<float>(i) * (maxRadius * 2) }, static_cast<float>(GetRandomValue(minRadius, maxRadius)), 100, getRainbow(overall_time));
					VerletObject object({ static_cast<float>(startPosition.x), static_cast<float>(startPosition.y) + static_cast<float>(i) * (maxRadius * 2) }, static_cast<float>(GetRandomValue(minRadius, maxRadius)), 100, getRandColor(BLUE, SKYBLUE));
					const float angle = spawnAngleMax * static_cast<float>(sin(static_cast<double>(overall_time * spawnAngleSpeed))) + PI * spawnAngleOffset;
					object.SetVelocity(Vec2{ static_cast <float> (cos(static_cast<double>(angle))), static_cast <float> (sin(static_cast<double>(angle))) } * objectSpeed, getFrameTime());
					SpatialSplitAddObject(object);
				}
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			const Vec2 mousePos = { static_cast<float>(ClampInt(windowWidth, 0, GetMouseX())), static_cast<float>(ClampInt(windowHeight, 0, GetMouseY())) };
			// IntVec2 mouseCell = { ClampInt(spatialSplitColumns - 2, 0, static_cast<int>(mousePos.x / spatialSplit_columnWidth)), ClampInt(spatialSplitRows - 2, 0, static_cast<int>(mousePos.y / spatialSplit_rowHeight)) };
			const IntVec2 mouseCell = CalculateCellPosition(mousePos, spatialSplitColumns, spatialSplitRows, spatialSplit_columnWidth, spatialSplit_rowHeight);
			auto& referenceCell = spatialSplit[mouseCell.x][mouseCell.y];

				for (int i = 0; i < static_cast<int>(referenceCell.size()); i++) {

					const VerletObject& fetchedObject = referenceCell[i];

					if ((mousePos.x < (fetchedObject.current_position.x + fetchedObject.objectRadius) && mousePos.x >(fetchedObject.current_position.x - fetchedObject.objectRadius)) && (mousePos.y < (fetchedObject.current_position.y + fetchedObject.objectRadius) && mousePos.y >(fetchedObject.current_position.y - fetchedObject.objectRadius))) {
						referenceCell.erase(referenceCell.begin() + i);
					}
				}
			
				DrawCircleLines(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y), 16.0f, RED);
				// DrawRectangle(mouseCell.x * spatialSplit_columnWidth, mouseCell.y * spatialSplit_rowHeight, spatialSplit_columnWidth, spatialSplit_rowHeight, RED);
				// DrawPixel(mouseCell.x * spatialSplit_columnWidth, mouseCell.y * spatialSplit_rowHeight, RED);

				string cellPosDisplay = "X: " + to_string(mouseCell.x) + ", Y: " + to_string(mouseCell.y);

				DrawText(cellPosDisplay.c_str(), 64, 128, 20, LIGHTGRAY);
				
		}

		/*if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			if (objectSelected) {
				objectSelected = false;
			}
		}*/

		for (int i{ subSteps }; i--;) {
			SpatialSplitCollisions();
			// CheckCollisions();
		}

		for (auto& column : spatialSplit) {
			for (auto& cell : column) {
				for (const auto& object : cell) {
					DrawCircle(static_cast<int>(object.current_position.x), static_cast<int>(object.current_position.y), object.objectRadius, object.objectColor);
				}
			}
			//for (auto& object : column[5]) {
			//	DrawCircle(object.current_position.x, object.current_position.y, object.objectRadius, BLUE);
			//}
		}

		//if (IsKeyDown(KEY_SPACE)) {
		//	VerletObject object({ windowWidth / 2, 64 }, 16, 1000, RAYWHITE);
		//	object.SetVelocity(Vec2{ 0.0f, 5.0f } * objectSpeed, getFrameTime());
		//	SpatialSplitAddObject(object);
		//	objectArray.push_back(object);
		//}

		DrawText(to_string(SpatialSplitGetObjects()).c_str(), 64, 64, 24, WHITE);

		frameRate = (frameRate + static_cast<int>(std::ceil(1 / GetFrameTime()))) / 2;
		DrawText(to_string(frameRate).c_str(), windowWidth - 128, 64, 24, WHITE);
		EndDrawing();


		//for (auto& currentObject : objectArray) {
		//	currentObject.Update(getFrameTime());
		//	currentObject.Accelerate(gravity);
		//	if (currentObject.current_position.y >= windowHeight - currentObject.objectRadius) {
		//		currentObject.current_position.y = windowHeight - currentObject.objectRadius;
		//		currentObject.SetVelocity(Vec2{ currentObject.velocity.x, -currentObject.velocity.y * objectBounciness}, getFrameTime());
		//	}
		//	if (currentObject.current_position.y <= 0 + currentObject.objectRadius) {
		//		currentObject.current_position.y = 0 + currentObject.objectRadius;
		//		currentObject.SetVelocity(Vec2{ currentObject.velocity.x, -currentObject.velocity.y * objectBounciness }, getFrameTime());
		//	}
		//	if (currentObject.current_position.x >= windowWidth - currentObject.objectRadius) {
		//		currentObject.current_position.x = windowWidth - currentObject.objectRadius;
		//		currentObject.SetVelocity(Vec2{ -currentObject.velocity.x * objectBounciness, currentObject.velocity.y }, getFrameTime());
		//	}
		//	if (currentObject.current_position.x <= 0 + currentObject.objectRadius) {
		//		currentObject.current_position.x = 0 + currentObject.objectRadius;
		//		currentObject.SetVelocity(Vec2{ -currentObject.velocity.x * objectBounciness, currentObject.velocity.y }, getFrameTime());
		//	}

		//	// DrawCircle(currentObject.current_position.x, currentObject.current_position.y, currentObject.objectRadius, currentObject.objectColor);
		//	// DrawCircle(spatialSplit_columnWidth * currentObject.cellPosition.x + spatialSplit_columnWidth / 2, spatialSplit_rowHeight * currentObject.cellPosition.y + spatialSplit_rowHeight / 2, currentObject.objectRadius, RED);
		//	// DrawRectangle(spatialSplit_columnWidth * currentObject.cellPosition.x, spatialSplit_rowHeight * currentObject.cellPosition.y, spatialSplit_columnWidth, spatialSplit_rowHeight, Color{ 255, 109, 194, 64 });
		//	// DrawRectangle(spatialSplit_columnWidth * 0, spatialSplit_rowHeight * 5, spatialSplit_columnWidth * spatialSplitColumns, spatialSplit_rowHeight, Color{ 120, 24, 24, 32 });
		//}


	}
	return 0;
}


//void CheckCollisions() {
//	const float response_coefficient = 0.5f;
//	const int objects_count = objectArray.size();
//	// Iterate on all objects
//	for (int i = 0; i < objects_count; i++) {
//		VerletObject& object_1 = objectArray[i];
//		// Iterate on object involved in new collision pairs
//		for (int k = i + 1; k < objects_count; k++) {
//			VerletObject& object_2 = objectArray[k];
//			const Vec2 v = object_1.current_position - object_2.current_position;
//			const float dist2 = v.x * v.x + v.y * v.y;
//			const float min_dist = object_1.objectRadius + object_2.objectRadius;
//			// Check overlapping
//			if (dist2 < min_dist * min_dist) {
//				const float dist = sqrt(dist2);
//				const Vec2 n = v / dist;
//				const float mass_ratio_1 = object_1.objectRadius / (object_1.objectRadius + object_2.objectRadius);
//				const float mass_ratio_2 = object_2.objectRadius / (object_1.objectRadius + object_2.objectRadius);
//				const float delta = 0.5f * response_coef * (dist - min_dist);
//				// Update positions
//				object_1.current_position = object_1.current_position - (n * (mass_ratio_2 * delta));
//				object_2.current_position = object_2.current_position + (n * (mass_ratio_1 * delta));
//			}
//		}
//	}
//}





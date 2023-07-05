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
	// stepDeltaTimeTarget = 1.0f / rate;
	stepDeltaTime = 1.0f / rate;
}

int main(void) {
	InitWindow(windowWidth, windowHeight, "FOE [Verlet Physics Engine]");
	SetTargetFPS(windowFPS);
	setUpdateRate(windowFPS);

	while (!WindowShouldClose()) {

		SpatialSplitUpdate();

		spawnTime += GetFrameTime();

		// stepDeltaTime = stepDeltaTimeTarget * (windowFPS / (1 / GetFrameTime()));

		BeginDrawing();
		ClearBackground(Color{ 24, 24, 24, 255 });

		if (IsKeyDown(KEY_SPACE)) {
			if (SpatialSplitGetObjects() < objectAmount && spawnTime >= spawnDelay) {
				spawnTime = 0;

				for (int i = 0; i < objectGroupAmount; i++) {
					// VerletObject object({ static_cast<float>(startPosition.x), static_cast<float>(startPosition.y) + static_cast<float>(i) * (maxRadius * 2) }, static_cast<float>(GetRandomValue(minRadius, maxRadius)), 100, getRainbow(overall_time));
					VerletObject object({ static_cast<float>(startPosition.x), static_cast<float>(startPosition.y) + static_cast<float>(i) * (maxRadius * 2) }, static_cast<float>(GetRandomValue(minRadius, maxRadius)), 100, getRandColor(BLUE, SKYBLUE));
					const float angle = spawnAngleMax * static_cast<float>(sin(static_cast<double>(static_cast<float>(GetTime()) * spawnAngleSpeed))) + PI * spawnAngleOffset;
					object.SetVelocity(Vec2{ static_cast <float> (cos(static_cast<double>(angle))), static_cast <float> (sin(static_cast<double>(angle))) } * objectSpeed, getFrameTime());
					SpatialSplitAddObject(object);
				}
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			const Vec2 mousePos = { static_cast<float>(ClampInt(windowWidth, 0, GetMouseX())), static_cast<float>(ClampInt(windowHeight, 0, GetMouseY())) };
			const IntVec2 mouseCell = CalculateCellPosition(mousePos, spatialSplitColumns, spatialSplitRows, spatialSplit_columnWidth, spatialSplit_rowHeight);
			auto& referenceCell = spatialSplit[mouseCell.x][mouseCell.y];

				for (int i = 0; i < static_cast<int>(referenceCell.size()); i++) {

					const VerletObject& fetchedObject = referenceCell[i];

					if ((mousePos.x < (fetchedObject.current_position.x + fetchedObject.objectRadius) && mousePos.x >(fetchedObject.current_position.x - fetchedObject.objectRadius)) && (mousePos.y < (fetchedObject.current_position.y + fetchedObject.objectRadius) && mousePos.y >(fetchedObject.current_position.y - fetchedObject.objectRadius))) {
						referenceCell.erase(referenceCell.begin() + i);
					}
				}
			
				DrawCircleLines(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y), 16.0f, RED);
				string cellPosDisplay = "X: " + to_string(mouseCell.x) + ", Y: " + to_string(mouseCell.y);
				DrawText(cellPosDisplay.c_str(), 64, 128, 20, LIGHTGRAY);
				
		}
		
		for (int i{ subSteps }; i--;) {
			SpatialSplitCollisions();
		}

		for (auto& column : spatialSplit) {
			for (auto& cell : column) {
				for (const auto& object : cell) {
					DrawCircle(static_cast<int>(object.current_position.x), static_cast<int>(object.current_position.y), object.objectRadius, object.objectColor);
				}
			}
		}

		DrawText(to_string(SpatialSplitGetObjects()).c_str(), 64, 64, 24, WHITE);

		frameRate = (frameRate + static_cast<int>(std::ceil(1 / GetFrameTime()))) / 2;
		DrawText(to_string(frameRate).c_str(), windowWidth - 128, 64, 24, WHITE);
		DrawText(to_string(stepDeltaTime).c_str(), windowWidth - 128, 128, 24, WHITE);
		DrawText(to_string(GetFrameTime()).c_str(), windowWidth - 128, 160, 24, WHITE);

		string frameComparison = to_string(static_cast<int>(1 / GetFrameTime())) + " - " + to_string(static_cast<int>(1 / stepDeltaTime));
		DrawText(frameComparison.c_str(), windowWidth - 128, 192, 24, WHITE);
		DrawText(to_string(windowFPS / (1 / GetFrameTime())).c_str(), windowWidth - 128, 224, 24, WHITE);
		DrawText(to_string(stepDeltaTime * (windowFPS / (1 / GetFrameTime()))).c_str(), windowWidth - 128, 256, 24, WHITE);
		EndDrawing();
	}
	return 0;
}






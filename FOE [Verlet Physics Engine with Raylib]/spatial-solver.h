#pragma once

#include "vector2.h"
#include "verlet-object.h"
#include <cmath>
#include <vector>
#include "main.h"

using namespace foe;

// Spatial Split Variables
constexpr int spatialSplitColumns = 64;
constexpr int spatialSplitRows = 36;


std::vector<VerletObject> spatialSplit[spatialSplitColumns][spatialSplitRows];
constexpr int spatialSplit_columnWidth = windowWidth / spatialSplitColumns;
constexpr int spatialSplit_rowHeight = windowHeight / spatialSplitRows;

inline float getFrameTime() {
	return stepDeltaTime / static_cast<float>(subSteps);
}

inline void SpatialSplitAddObject(VerletObject& object) {
	object.cellPosition = CalculateCellPosition(object.current_position, spatialSplitColumns, spatialSplitRows, spatialSplit_columnWidth, spatialSplit_rowHeight);
	spatialSplit[object.cellPosition.x][object.cellPosition.y].push_back(object);
}

inline void SpatialSplitCollisions() {
	// Column
	for (int columnKey = 1; columnKey < spatialSplitColumns -1; columnKey++) {

		auto& column = spatialSplit[columnKey];

		// Cell
		for (int cellKey = 1; cellKey < spatialSplitRows -1; cellKey++) {

			auto& cell = column[cellKey];

			for (int distanceX = -1; distanceX <= 1; distanceX++) {
				for (int distanceY = -1; distanceY <= 1; distanceY++) {

					auto& otherCell = spatialSplit[columnKey + distanceX][cellKey + distanceY];

					for (int i = 0; i < static_cast<int>(cell.size()); i++) {

						auto& object = cell[i];

						for (int j = 0; j < static_cast<int>(otherCell.size()); j++) {

							auto& otherObject = otherCell[j];

							// If it is the same object, then skip it and continue the loop
							if ((distanceX == 0) && (distanceY == 0) && (i == j)) continue;


							const Vec2 diff = object.current_position - otherObject.current_position;
							const float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
							const float min_dist = object.objectRadius + otherObject.objectRadius;

							// const float dist = std::sqrt(((object.current_position.x - otherObject.current_position.x) * (object.current_position.x - otherObject.current_position.x)) + ((object.current_position.y - otherObject.current_position.y) * (object.current_position.y - otherObject.current_position.y)));
							
							// Check overlapping
							if (dist < min_dist) {
								constexpr float response_coefficient = 0.75f;

								// const float dist = std::sqrt(squareDist);

								/*DrawText(to_string(dist).c_str(), 64, 192, 20, WHITE);
								DrawText(to_string(distance).c_str(), 64, 160, 20, WHITE);*/

								const Vec2 value = diff / dist;
								const float mass_ratio_1 = object.objectRadius / (object.objectRadius + otherObject.objectRadius);
								const float mass_ratio_2 = otherObject.objectRadius / (object.objectRadius + otherObject.objectRadius);
								const float delta = 0.5f * response_coefficient * (dist - min_dist);

								// Update positions
								object.current_position = object.current_position - (value * (mass_ratio_2 * delta));
								otherObject.current_position = otherObject.current_position + (value * (mass_ratio_1 * delta));
							}
						}
					}
				}
			}
		}
	}
}

inline void SpatialSplitUpdate() {

	for (auto& column : spatialSplit) {
		for (auto& cell : column) {
			for (VerletObject& object : cell) {
				object.Update(getFrameTime());
				object.Accelerate(gravity);
				if (object.current_position.y >= windowHeight - object.objectRadius - spatialSplit_rowHeight) {
					object.current_position.y = windowHeight - object.objectRadius - spatialSplit_rowHeight;
					object.SetVelocity(Vec2{ object.velocity.x, -object.velocity.y * objectBounciness }, getFrameTime());
				}
				if (object.current_position.y <= 0 + object.objectRadius + spatialSplit_rowHeight) {
					object.current_position.y = 0 + object.objectRadius + spatialSplit_rowHeight;
					object.SetVelocity(Vec2{ object.velocity.x, -object.velocity.y * objectBounciness }, getFrameTime());
				}
				if (object.current_position.x >= windowWidth - object.objectRadius - spatialSplit_columnWidth) {
					object.current_position.x = windowWidth - object.objectRadius - spatialSplit_columnWidth;
					object.SetVelocity(Vec2{ -object.velocity.x * objectBounciness, object.velocity.y }, getFrameTime());
				}
				if (object.current_position.x <= 0 + object.objectRadius + spatialSplit_columnWidth) {
					object.current_position.x = 0 + object.objectRadius + spatialSplit_columnWidth;
					object.SetVelocity(Vec2{ -object.velocity.x * objectBounciness, object.velocity.y }, getFrameTime());
				}
			}
		}
	}

	std::vector<VerletObject> objectBufferArray;

	for (int i = 0; i < spatialSplitColumns; i++) {
		for (int j = 0; j < spatialSplitRows; j++) {

			std::vector<VerletObject>& cellVector = spatialSplit[i][j];

			for (int k = 0; k < static_cast<int>(cellVector.size()); k++) {

				VerletObject& object = cellVector[k];
				object.cellPosition = CalculateCellPosition(object.current_position, spatialSplitColumns, spatialSplitRows, spatialSplit_columnWidth, spatialSplit_rowHeight);


				if (object.cellPosition == IntVec2{ i, j }) {
					continue;
				}
				else {
					objectBufferArray.push_back(object);
					cellVector.erase(cellVector.begin() + k);
				}
			}
		}
	}

	for (VerletObject& objectBuffer : objectBufferArray) {
		spatialSplit[objectBuffer.cellPosition.x][objectBuffer.cellPosition.y].push_back(objectBuffer);
	}
}

inline int SpatialSplitGetObjects() {
	int size = 0;
	for (const auto& column : spatialSplit) {
		for (const auto& cell : column) {
			size += static_cast<int>(cell.size());
		}
	}
	return size;
}

#include "HeightMap.h"
#include "Game.h"
#include "CSVHelper.h"
#include <fstream>

HeightMap::HeightMap(Game* game) {
	mGame = game;
	std::ifstream fileReader("Assets/HeightMap/HeightMap.csv");

	if (!fileReader.is_open()) {
		SDL_Log("File Did Not Open");
	}

	std::string line;
	//constructs csv vector
	getline(fileReader, line);
	while (!line.empty()) {
		std::vector<std::string> holder;
		std::vector<int> temp;

		holder = CSVHelper::Split(line);
		for (unsigned int j = 0; j < holder.size(); j++) {
			temp.push_back(std::stoi(holder[j]));
		}
		intsCSV.push_back(temp);
		getline(fileReader, line);
	}
}

bool HeightMap::IsCellOnTrack(int row, int column) {
	//check bounds
	if (row < 0 || row > intsCSV.size()) {
		return false;
	}
	if (column < 0 || column > intsCSV[row].size()) {
		return false;
	}
	//check for -1
	if (intsCSV[row][column] == -1) {
		return false;
	}
	return true;
}

float HeightMap::GetHeightFromCell(int row, int column) {
	return -40.0f + float(intsCSV[row][column]) * 5.0f;
}

Vector3 HeightMap::CellToWorld(int row, int column) {
	if (!IsCellOnTrack(row, column)) {
		return Vector3::Zero;
	}
	float x = gridTop - cellSize * row;
	float y = gridLeft + cellSize * column;
	float z = GetHeightFromCell(row, column);
	return Vector3(x, y, z);
}

Vector2 HeightMap::WorldToCell(float x, float y) {
	int row = (abs(x - gridTop) + cellSize / 2.0f) / cellSize;
	int col = (abs(y - gridLeft) + cellSize / 2.0f) / cellSize;
	return Vector2(row, col);
}

bool HeightMap::isOnTrack(float x, float y) {
	Vector2 test = WorldToCell(x, y);
	return IsCellOnTrack(test.x, test.y);
}

float HeightMap::GetHeight(float x, float y) {
	if (!isOnTrack(x, y)) {
		return -1000.0f;
	}
	Vector2 test = WorldToCell(x, y);
	return GetHeightFromCell(test.x, test.y);
}
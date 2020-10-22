#pragma once
#include <vector>
#include "Math.h"

class Game;

class HeightMap {
public:
	HeightMap(Game* mGame);
	Game* mGame;
	std::vector< std::vector<int> > intsCSV;
	Vector3 CellToWorld(int row, int column);
	Vector2 WorldToCell(float x, float y);
	bool isOnTrack(float x, float y);
	float GetHeight(float x, float y);
private:
	bool IsCellOnTrack(int row, int column);
	float GetHeightFromCell(int row, int column);
	float cellSize = 40.55f;
	float gridTop = 1280.0f;
	float gridLeft = -1641.0f;
};
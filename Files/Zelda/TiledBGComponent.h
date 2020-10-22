#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>

class Actor;

class TiledBGComponent : public SpriteComponent 
{
public:
	TiledBGComponent(Actor* owner, int drawOrder = 50);
	~TiledBGComponent();
	void Draw(SDL_Renderer* renderer);
	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);

private:
	int tileWidth;
	int tileHeight;
	std::vector< std::vector<int> > intsCSV;
};
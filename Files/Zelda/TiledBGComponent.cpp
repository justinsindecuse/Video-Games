#include "TiledBGComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder) : SpriteComponent(owner, drawOrder) {

}

TiledBGComponent::~TiledBGComponent() {

}

void TiledBGComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture)
	{

		int perRow = mTexWidth / tileWidth;
		for (unsigned int i = 0; i < intsCSV.size(); i++) {
			for (unsigned int j = 0; j < intsCSV[i].size(); j++) {
				int tileNum = intsCSV[i][j];
				SDL_Rect srcRect;
				SDL_Rect destRect;
				srcRect.w = tileWidth;
				srcRect.h = tileHeight;
				srcRect.x = (tileNum % perRow) * tileWidth;
				srcRect.y = (tileNum / perRow) * tileHeight;
				destRect.w = tileWidth;
				destRect.h = tileHeight;
				destRect.x = (j * tileWidth) - mOwner->mGame->cameraPos.x;
				destRect.y = (i * tileHeight) - mOwner->mGame->cameraPos.y;
				SDL_RenderCopyEx(renderer, mTexture, &srcRect, &destRect, 0.0, nullptr, SDL_FLIP_NONE);
			}
		}
		
	}
}

void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileW, int tileH) {
	tileWidth = tileW;
	tileHeight = tileH;
	std::ifstream fileReader(fileName);

	if (!fileReader.is_open()) {
		SDL_Log("File Did Not Open");
	}

	std::string line;
	std::vector<std::string> holder;
	//constructs csv vector
	getline(fileReader, line);
	while(!line.empty()) {
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
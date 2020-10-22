#include "LaserMine.h"
#include "Game.h"
#include "Renderer.h"

LaserMine::LaserMine(Game* game, Actor* parent) : Actor(game, parent) {
	mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
	lc = new LaserComponent(this);
}
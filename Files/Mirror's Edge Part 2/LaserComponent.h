#pragma once
#include "MeshComponent.h"
#include "SegmentCast.h"
#include <vector>

class LaserComponent : public MeshComponent {
public:
	LaserComponent(class Actor* owner, bool usesAlpha = true);
	void Draw(class Shader* shader);
	void Update(float deltaTime);
	Matrix4 Helper(LineSegment* line);
	std::vector<LineSegment*> lines;
private:

};
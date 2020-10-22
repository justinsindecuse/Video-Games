#include "LaserComponent.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Actor.h"
#include "Game.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"
#include "SegmentCast.h"
#include "Player.h"
#include <iostream>

LaserComponent::LaserComponent(class Actor* owner, bool usesAlpha) : MeshComponent(owner, usesAlpha) {
	SetMesh(mOwner->mGame->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Draw(class Shader* shader) {
	if (mMesh)
	{
		// Set the world transform
		for (unsigned int i = 0; i < lines.size(); i++) {
			shader->SetMatrixUniform("uWorldTransform",
				Helper(lines[i]));
			// Set the active texture
			Texture* t = mMesh->GetTexture(mTextureIndex);
			if (t)
			{
				t->SetActive();
			}
			// Set the mesh's vertex array as active
			VertexArray* va = mMesh->GetVertexArray();
			va->SetActive();
			// Draw
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}

	}
}

void LaserComponent::Update(float deltaTime) {
	bool reflected = false;
	lines.clear();

	//creates initial laser
	LineSegment* line = new LineSegment();
	line->mStart = mOwner->GetWorldPosition();
	line->mEnd = mOwner->GetWorldPosition() + (mOwner->GetWorldForward() * 500.0f);
	CastInfo info;
	if (SegmentCast(mOwner->mGame->mPlayer, *line, info)) {
		line->mEnd = info.mPoint;
	}
	else {
		if (SegmentCast(mOwner->mGame->objects, *line, info, mOwner->mParent)) {
			line->mEnd = info.mPoint;
			if (info.mActor->getMirror()) {
				reflected = true;
			}
			else {
				reflected = false;
			}
		}
	}
	lines.push_back(line);
	Vector3 newForward = line->mEnd - line->mStart;
	newForward.Normalize();

	//loop to test for reflections
	while (reflected) {
		//Creates new line segment
		newForward = Vector3::Reflect(newForward, info.mNormal);
		LineSegment* line1 = new LineSegment();
		line1->mStart = info.mPoint;
		line1->mEnd = info.mPoint + (newForward * 500.0f);
		newForward = line1->mEnd - line1->mStart;
		newForward.Normalize();

		//tests for collisions
		Actor* currentBlock = info.mActor;
		if (SegmentCast(mOwner->mGame->mPlayer, *line1, info)) {
			line1->mEnd = info.mPoint;
			reflected = false;
		}
		else {
			if (SegmentCast(mOwner->mGame->objects, *line1, info, currentBlock)) {
				line1->mEnd = info.mPoint;
				//test for mirrors
				if (info.mActor->getMirror()) {
					reflected = true;
				}
				else {
					reflected = false;
				}
			}
			else {
				reflected = false;
			}
		}
		lines.push_back(line1);
	}
}

Matrix4 LaserComponent::Helper(LineSegment* line1) {
	//Quaternion stuff
	Vector3 old = Vector3::UnitX;
	Vector3 newLine = line1->mEnd - line1->mStart;
	newLine.Normalize();
	Matrix4 rotate;
	//test case to see if lineSegment is UnitX
	//Why is the LineSegment for these lasers the also the UnitX??????????? am i missing something?
	if ((newLine.x == 1.0f && newLine.y == 0.0f) && newLine.z == 0.0f) {
		rotate = Matrix4::CreateFromQuaternion(Quaternion::Identity);
	}
	else {
		float quatAngle = acos(Vector3::Dot(old, newLine));
		Vector3 axis = Vector3::Cross(old, newLine);
		axis.Normalize();
		Quaternion* rotation = new Quaternion(axis, quatAngle);
		rotate = Matrix4::CreateFromQuaternion(*rotation);
	}

	//Other stuff
	Matrix4 scale = Matrix4::CreateScale(Vector3(line1->Length(), 1.0f, 1.0f));
	Matrix4 translate = Matrix4::CreateTranslation((line1->mEnd + line1->mStart) * (1.0f/2.0f));
	Matrix4 mWorldTransform = scale * rotate * translate;
	return mWorldTransform;
}
//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <iostream>

bool Game::initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	stop = false;
	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	thickness = 10;
	paddlePos.y = 384;
	paddlePos.x = 10;
	paddleHeight = 100;
	paddleWidth = 7;
	ballPos.x = 507;
	ballPos.y = 379;
	ballVelocity.x = 90;
	ballVelocity.y = 90;
	ballDimension = 10;
	ballMove = 3;
	limit = 0.033f;
	limiter = 1000;
	screenx = 1024;
	screeny = 768;
	prevTime = float(SDL_GetTicks());
	change = 1;
	return true;
}

void Game::RunLoop() {
	while (!stop) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			//case statement
		case SDL_QUIT:
			stop = true;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		stop = true;
	}
	if (state[SDL_SCANCODE_UP]) {
		arrow = "up";
	}
	if (state[SDL_SCANCODE_DOWN]) {
		arrow = "down";
	}
}

void Game::UpdateGame() {
	float currTime = float(SDL_GetTicks());
	//caps fps at 60
	while (currTime <= prevTime + 16) {
		currTime = float(SDL_GetTicks());
	}
	deltaTime = (currTime - prevTime)/limiter;
	prevTime = currTime;
	//Maxes out Delta Time
	if (deltaTime > limit) {
		deltaTime = limit;
	}
	if (arrow == "up") {
		if (paddlePos.y - ballMove <= thickness) {
			paddlePos.y = thickness;
		}
		else {
			paddlePos.y -= ballMove;
		}
	}
	if (arrow == "down") {
		if (paddlePos.y + ballMove >= screeny - thickness - paddleHeight) {
			paddlePos.y = screeny - thickness - paddleHeight;
		}
		else {
			paddlePos.y += ballMove;
		}
	}
	//Paddle Collision
	if (ballPos.x + (ballVelocity.x * deltaTime) <= paddlePos.x + paddleWidth && 
		ballPos.y + (ballVelocity.y * deltaTime) >= paddlePos.y && 
		ballPos.y + (ballVelocity.y * deltaTime) <= paddlePos.y + paddleHeight) {
		ballPos.x = paddlePos.x + paddleWidth + change;
		ballVelocity.x *= -change;
	}
	else {
		ballPos.x += int(ballVelocity.x * deltaTime);
	}
	//Right Wall Collision
	if (ballPos.x + (ballVelocity.x * deltaTime) >= screenx - thickness - ballDimension) {
		ballPos.x = screenx - change - thickness - ballDimension;
		ballVelocity.x *= -change;
	}
	else {
		ballPos.x += int(ballVelocity.x * deltaTime);
	}
	//Ceiling Collision
	if (ballPos.y + (ballVelocity.y * deltaTime) <= thickness) {
		ballPos.y = thickness + change;
		ballVelocity.y *= -change;
	}
	else {
		ballPos.y += int(ballVelocity.y * deltaTime);
	}
	//Floor Collision
	if (ballPos.y + (ballVelocity.y * deltaTime) >= screeny - thickness - ballDimension) {
		ballPos.y = screeny - change - thickness - ballDimension;
		ballVelocity.y *= -change;
	}
	else {
		ballPos.y += int(ballVelocity.y * deltaTime);
	}
	//Check if Player Loses
	if (ballPos.x <= 0) {
		stop = true;
	}
}

void Game::GenerateOutput() {
	//fill
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	//Drawing rectangle
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//Top
	DrawRect(0, 0, screenx, thickness);
	//Right
	DrawRect(screenx-thickness, 0, thickness, screeny);
	//Bottom
	DrawRect(0, screeny-thickness, screenx, thickness);

	//drawing paddle
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	DrawRect(paddlePos.x, paddlePos.y, paddleWidth, paddleHeight);

	//Drawing the ball
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	DrawRect(ballPos.x, ballPos.y, ballDimension, ballDimension);

	//Presenting
	SDL_RenderPresent(renderer);

	arrow = "";
}

void Game::DrawRect(int a, int b, int c, int d) {
	SDL_Rect rect;
	rect.x = a;
	rect.y = b;
	rect.w = c;
	rect.h = d;
	SDL_RenderFillRect(renderer, &rect);
}
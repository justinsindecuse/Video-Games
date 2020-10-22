#include "SDL2/SDL.h"
#include <string>

class Game {
public:
	SDL_Point ballPos;
	SDL_Point ballVelocity;
	bool initialize();
	void Shutdown();
	void RunLoop();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Point paddlePos;
	std::string arrow;
	float deltaTime;
	float prevTime;
	int ballDimension;
	int paddleWidth;
	int paddleHeight;
	int thickness;
	float limit;
	bool stop;
	int limiter;
	int ballMove;
	int screenx;
	int screeny;
	int change;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void DrawRect(int a, int b, int c, int d);
};
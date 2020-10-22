//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

int main(int argc, char** argv)
{
	Game game1;
	if (game1.initialize()) {
		game1.RunLoop();
	}
	game1.Shutdown();
	return 0;
}

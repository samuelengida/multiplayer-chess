// Support Code written by Michael D. Shah
// Last Updated: 6/10/21
// Please do not redistribute without asking permission.

// Functionality that we created
#include "Game.hpp"

#include <iostream>

int main(int argc, char** argv){

	// if(argc < 2) {
	// 	std::cout << "Please specify a .obj model" << std::endl;
	// 	return 0;
	// }
	
	OBJReader myReader1 = OBJReader("./../common/objects/Queen/Queen.obj");

	// Create an instance of an object for a Game
	Game myGame(1280,720, myReader1);
	// Run our program forever
	myGame.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}

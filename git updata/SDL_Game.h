#pragma once

struct Game_struct {
	int test; 
};

struct Game {
	bool up; 
};

Game print_up(Game_struct* input) {
	cout << "boutton up\n";
	return Game{ true };
}
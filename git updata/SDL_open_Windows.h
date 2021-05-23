#pragma once

#include <iostream>
#include <string>
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include <SDL_image.h>

struct Super_point{
	int x, y;

	//operator def of > < >= <= ==  
	bool operator==(const Super_point &a) {
		return ((this->x == a.x) && (this->y == a.y) ? true : false); 
	}
	bool operator>=(const Super_point &a) {
		return ((this->x >= a.x) && (this->y >= a.y) ? true : false);
	}
	bool operator<=(const Super_point &a) {
		return ((this->x <= a.x) && (this->y <= a.y) ? true : false);
	}

	bool operator > (const Super_point &a) {
		return ((this->x > a.x) && (this->y > a.y) ? true : false);
	}
	bool operator < (const Super_point &a) {
		return ((this->x < a.x) && (this->y < a.y) ? true : false);
	}

	// operator def 
	Super_point operator+ (const Super_point & a) {
		return (Super_point{ this->x + a.x , this->y + a.y }); 
	}
	Super_point operator- (const Super_point& a) {
		return (Super_point{ this->x - a.x , this->y - a.y });
	}
	Super_point& operator+= (const Super_point& a) {
		this->x += a.x; 
		this->y += a.y; 
		return *this; 
	}
	Super_point& operator-= (const Super_point& a) {
		this->x -= a.x;
		this->y -= a.y;
		return *this;
	}
};

struct Window_data {
	int WIDTH, HEIGHT;
	SDL_Window* window; // = NULL; // The window we'll be rendering to
	SDL_Renderer* renderer; // = NULL; // The window renderer
	SDL_Event window_event;
	Window_data(int WIDTH_in, int HEIGHT_in) {
		WIDTH = WIDTH_in;
		HEIGHT = HEIGHT_in;
		window = NULL; // The window we'll be rendering to
		renderer = NULL; // The window renderer
		window_event = { NULL };
	}
	
};


inline int open_window(Window_data &Win_data , std::string project_name)  { // buile sdl
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		// Error Handling
		std::cout << "SDL_Init failed: " << SDL_GetError() << "\n"; 
		return 1;
	}

	// Create window
	// SDL_WINDOWPOS_UNDEFINED: Used to indicate that you don't care what the window position is.
	Win_data.window =SDL_CreateWindow(project_name.c_str(), 50, 50, Win_data.WIDTH, Win_data.HEIGHT, SDL_WINDOW_SHOWN);
	if (Win_data.window == NULL) {
		std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << "\n"; 
		SDL_Quit();
		return 2;
	}
	// Initialize PNG loading	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL_image failed: " << IMG_GetError() << "\n";
		return 3;
	}
	// Create renderer
	Win_data.renderer = SDL_CreateRenderer(Win_data.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Win_data.renderer == NULL) {
		SDL_DestroyWindow(Win_data.window);
		std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n"; 
		SDL_Quit();
		return 5;
	}

	return 0;
}

inline void close_window(Window_data& Win_data)  {// close sdl 
	// Destroy renderer	
	// Destroy window	
	// Quit Image subsystem
	// Quit SDL subsystems
	SDL_DestroyRenderer(Win_data.renderer);
	SDL_DestroyWindow(Win_data.window);
	IMG_Quit();
	// Shutdown and cleanup the truetype font API.	
	SDL_Quit();
}

inline void clear_screan(Window_data &win_in){
	SDL_SetRenderDrawColor(win_in.renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(win_in.renderer);
	
}

// SDL_RenderPresent(win_in.renderer);
inline void Updata_screen(Window_data &win_in) {
	SDL_RenderPresent(win_in.renderer); 
}

/*inline int Set_screen_color() {
	SDL_SetRenderDrawColor()
}*/
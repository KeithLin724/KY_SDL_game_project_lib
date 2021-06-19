/*KYLiN Game project */
#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <tgmath.h>
#include <string>
#include "SDL_open_Windows.h"
#include "mouse_contro.h"
#include "ImageData.h"
#include "Image_move_with_mouse.h"
#include "SDL_Game.h"

int main(int argc, char* args[]) {

	//setup
	Window_data win_data(1200, 700); 
	MouseState MS = NONE;
	Mouse mouse1(win_data, MS); 
	std::string project_name = "KY Game project"; 
	bool quit = false; 
	//SDL_Event e;
	

	std::string backgrand = "../Images/backgrand.png"; 
	std::string captain = "../Images/captain.png";
	std::string spiderman = "../Images/spman.png"; 

	ImageData img_backgrand, img_captain, img_spman; 
	ImgData_control Image_control = { NULL , NULL , NULL }; 
	ImgData_control Image_control_spman = { NULL , NULL , NULL };

	if (open_window(win_data , project_name)) { // if no open a window 
		std::cout << "Failed to initialize SDL!\n"; 
		return -1; 
	}

	Super_point mouse_Bef_Point = { 0 , 0 }; 
	LoadImgTexture(win_data, img_backgrand, backgrand, 1, 1, 1, true);
	LoadImgTexture(win_data, img_captain, captain, 1, 1, 1, false, Super_point{ 10 , 10 });
	LoadImgTexture(win_data, img_spman, spiderman, 1, 1, 1, false, Super_point{ 0 , 170 + 10 });

	while (!quit) {// quit window 

		mouse1.mouseState = NONE; 
		while (SDL_PollEvent(&win_data.window_event)) {
			if (win_data.window_event.type == SDL_QUIT) {
				quit = true;
			}
			mouse_Bef_Point = mouse1.point_mouse; 
			Mouse_Event(win_data, mouse1);
		}
		ImgData_control_function(Image_control, win_data, mouse1, img_captain, mouse_Bef_Point, nullptr , print_up);

		if (Game_thing_out(Image_control).up) {
			cout << Game_thing_out(Image_control).up << "\n";
		}

		ImgData_control_function(Image_control_spman, win_data, mouse1, img_spman, mouse_Bef_Point);

		// clear screan 
		clear_screan(win_data); 
		
		// draw 

		ImgRender(win_data, img_backgrand, Super_point{ 0 , 0 }, Super_point{ NULL , NULL }, NULL, SDL_FLIP_NONE, 255, matrix_img{ 1,1 }, false, true); 
		// draw captain 
		ImageData_control_draw(Image_control, matrix_img{ 0.1 , 0.1 }); 

		ImageData_control_draw(Image_control_spman, matrix_img{ 0.3 , 0.3 });
		//ImgRender(win_data, img_captain, Super_point{ 0 , 0 }, Super_point{ NULL , NULL }, NULL, SDL_FLIP_NONE, 255, matrix_img{ 0.1 , 0.1 }, true);
		// updata screan 
		Updata_screen(win_data); 
		
	}

	DestroyImage(img_backgrand); 
	DestroyImage(img_captain); 

	close_window(win_data);

	return 0;
}
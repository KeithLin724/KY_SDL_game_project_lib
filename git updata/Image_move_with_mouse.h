#pragma once
#include "SDL_open_Windows.h"
#include "mouse_contro.h"
#include "ImageData.h"
#include "SDL_Game.h"
#include <vector>
#include <functional>

struct ImgData_control {
	Window_data* win_data; 
	Mouse *mouse_control; 
	ImageData *img; 
	Game* Game_out;
};

struct multi_ImgData_control {
	Window_data* win_data;
	Mouse* mouse_control;
	ImageData* img;
	std::vector <ImageData*> multi_img_arr; 
};

inline void ImgData_control_function(ImgData_control& Image_control, Window_data& win_in, Mouse& mouse_in, ImageData& img_in, Super_point& bef_point, Game_struct *Game_in = nullptr, std::function <Game(Game_struct *)> fun = nullptr) {
	Game game_out = {};
	if (mouse_in.mouseState == IN_LB_PR_HOVER) {
		if ((mouse_in.point_mouse.x >= img_in.img_p.x) && (mouse_in.point_mouse.x <= img_in.img_p.x + img_in.pw + 10)
			&& (mouse_in.point_mouse.y >= img_in.img_p.y) && (mouse_in.point_mouse.y <= img_in.img_p.y + img_in.ph + 10)) {
			std::cout << "in " <<img_in.texture <<" range \n";
			std::cout << bef_point.x << " " << bef_point.y << "\n";
			if ((bef_point.x != mouse_in.point_mouse.x) || (bef_point.y != mouse_in.point_mouse.y)) {
				std::cout << mouse_in.point_mouse.x - bef_point.x << " " << mouse_in.point_mouse.y - bef_point.y << "\n"; 
				img_in.img_p += (mouse_in.point_mouse - bef_point);
				std::cout << "change img point \n";
			}
		}
		else {
			std::cout << "out of the " << img_in.texture << "\n"; 
		}

	}
	else if (win_in.window_event.type == SDL_MOUSEBUTTONUP) {
		if (fun != nullptr) {
			game_out = fun(Game_in);
			if (game_out.up) {
				Image_control = { &win_in, &mouse_in , &img_in , &game_out };
			}
		}
	}
	Image_control = {&win_in, &mouse_in , &img_in , &game_out }; 
}

inline Game Game_thing_out(ImgData_control& Image_control) {
	return *(Image_control.Game_out);
}

inline int ImageData_control_draw(ImgData_control& Image_control, matrix_img scale = { 1,1 }, SDL_RendererFlip flip = SDL_FLIP_NONE , int alpha = 255 , double angle = NULL) {
	int wc = Image_control.img->frame % Image_control.img->wn;
	int hc = Image_control.img->frame / Image_control.img->wn;

	SDL_Rect src = {
		Image_control.img->width / Image_control.img->wn * wc  ,
		Image_control.img->height / Image_control.img->hn * hc , 
		Image_control.img->width / Image_control.img->wn , 
		Image_control.img->height / Image_control.img->hn
	}; 
	
	SDL_Rect dst = {
		Image_control.img->img_p.x , 
		Image_control.img->img_p.y , 
		src.w* scale._x , 
		src.h* scale._y
	}; 
	
	Image_control.img->pw = dst.w;
	Image_control.img->ph = dst.h;

	// Set and enable standard alpha blending mode for a texture
	// https://wiki.libsdl.org/SDL_SetTextureBlendMode
	// https://wiki.libsdl.org/SDL_BlendMode
	if (SDL_SetTextureBlendMode(Image_control.img->texture, SDL_BLENDMODE_BLEND) == -1) { // err
		std::cout << "SDL_SetTextureBlendMode failed: " << SDL_GetError() << "\n";
		return -1;
	}

	// Modulate texture alpha
	// https://wiki.libsdl.org/SDL_SetTextureAlphaMod
	if (SDL_SetTextureAlphaMod(Image_control.img->texture, alpha) == -1) {
		std::cout << "SDL_SetTextureAlphaMod failed: " << SDL_GetError() << "\n";
		return -1;
	}

	// SDL_Point
	// A structure that defines a two dimensional point.
	// https://wiki.libsdl.org/SDL_Point
	SDL_Point center = { NULL, NULL };

	// Copy a portion of the texture to the current rendering target, 
	// optionally rotating it by angle around the given center and also flipping it top-bottom and/or left-right.
	// if center is NULL, rotation will be done around dstrect.w / 2, dstrect.h / 2
	// https://wiki.libsdl.org/SDL_RenderCopyEx
	SDL_RenderCopyEx(Image_control.win_data->renderer, Image_control.img->texture, &src, &dst, angle, &center, flip);
	return 1;
}




inline void multi_ImgData_control_function(multi_ImgData_control& multi_Image_control_in, Window_data& win_in, Mouse& mouse_in, ImageData& img_in,std::vector<ImageData>& multi_img_arr, Super_point& bef_point) {
	std::vector<ImageData* > img_control_multi; 
	if (mouse_in.mouseState == IN_LB_PR_HOVER) {
		if ((mouse_in.point_mouse.x >= img_in.img_p.x) && (mouse_in.point_mouse.x <= img_in.img_p.x + img_in.pw + 10)
			&& (mouse_in.point_mouse.y >= img_in.img_p.y) && (mouse_in.point_mouse.y <= img_in.img_p.y + img_in.ph + 10)) {
			std::cout << "in captain range \n";
			std::cout << bef_point.x << " " << bef_point.y << "\n";

			if ((bef_point.x != mouse_in.point_mouse.x) || (bef_point.y != mouse_in.point_mouse.y)) {
				std::cout << mouse_in.point_mouse.x - bef_point.x << " " << mouse_in.point_mouse.y - bef_point.y << "\n";
				auto diff_point = (mouse_in.point_mouse - bef_point);
				img_in.img_p += diff_point; 
				for (auto &elme : multi_img_arr) {
					elme.img_p += diff_point; 
					img_control_multi.push_back(&elme); 
				}

				std::cout << "change img point \n";
			}
		}
		else {
			std::cout << "out of the captain \n";
		}

	}
	else if (win_in.window_event.type == SDL_MOUSEBUTTONUP) {
		// design



	}


	multi_Image_control_in = { &win_in , &mouse_in ,&img_in , img_control_multi };
	//Image_control = { &win_in, &mouse_in , &img_in };
}

inline int multi_ImageData_control_draw(multi_ImgData_control& multi_Image_control_in, matrix_img main_scale = { 1,1 }, matrix_img multi_scale = {1,1}, SDL_RendererFlip flip = SDL_FLIP_NONE, int alpha = 255, double angle = NULL) {
	int wc = multi_Image_control_in.img->frame % multi_Image_control_in.img->wn;
	int hc = multi_Image_control_in.img->frame / multi_Image_control_in.img->wn;

	SDL_Rect src = {
		multi_Image_control_in.img->width / multi_Image_control_in.img->wn * wc , 
		multi_Image_control_in.img->height / multi_Image_control_in.img->hn * hc,
		multi_Image_control_in.img->width / multi_Image_control_in.img->wn , 
		multi_Image_control_in.img->height / multi_Image_control_in.img->hn
	};
	
	SDL_Rect dst = {
		multi_Image_control_in.img->img_p.x , 
		multi_Image_control_in.img->img_p.y , 
		src.w* main_scale._x , 
		src.h* main_scale._y
	}; 

	multi_Image_control_in.img->pw = dst.w;
	multi_Image_control_in.img->ph = dst.h;

	// Set and enable standard alpha blending mode for a texture
	// https://wiki.libsdl.org/SDL_SetTextureBlendMode
	// https://wiki.libsdl.org/SDL_BlendMode
	if (SDL_SetTextureBlendMode(multi_Image_control_in.img->texture, SDL_BLENDMODE_BLEND) == -1) { // err
		std::cout << "SDL_SetTextureBlendMode failed: " << SDL_GetError() << "\n";
		return -1;
	}

	// Modulate texture alpha
	// https://wiki.libsdl.org/SDL_SetTextureAlphaMod
	if (SDL_SetTextureAlphaMod(multi_Image_control_in.img->texture, alpha) == -1) {
		std::cout << "SDL_SetTextureAlphaMod failed: " << SDL_GetError() << "\n";
		return -1;
	}

	// SDL_Point
	// A structure that defines a two dimensional point.
	// https://wiki.libsdl.org/SDL_Point
	SDL_Point center = { NULL, NULL };

	// Copy a portion of the texture to the current rendering target, 
	// optionally rotating it by angle around the given center and also flipping it top-bottom and/or left-right.
	// if center is NULL, rotation will be done around dstrect.w / 2, dstrect.h / 2
	// https://wiki.libsdl.org/SDL_RenderCopyEx
	SDL_RenderCopyEx(multi_Image_control_in.win_data->renderer, multi_Image_control_in.img->texture, &src, &dst, angle, &center, flip);

	for (auto & elme : multi_Image_control_in.multi_img_arr) {
		wc = elme->frame % elme->wn;
		hc = elme->frame / elme->wn;

		//SDL_Rect src, dst;
		src = { 
			elme->width / elme->wn * wc  ,
			elme->height / elme->hn * hc , 
			elme->width / elme->wn , 
			elme->height / elme->hn
		}; 
		/*src.x = elme->width / elme->wn * wc;
		src.y = elme->height / elme->hn * hc;
		src.w = elme->width / elme->wn;
		src.h = elme->height / elme->hn;*/

		dst = {
			elme->img_p.x , 
			elme->img_p.y , 
			src.w* (int)multi_scale._x , 
			src.h* (int)multi_scale._y
		}; 
		/*dst.x = elme->img_p.x;
		dst.y = elme->img_p.y;

		dst.w = src.w * multi_scale._x;
		dst.h = src.h * multi_scale._y;*/

		elme->pw = dst.w;
		elme->ph = dst.h;

		if (SDL_SetTextureBlendMode(elme->texture, SDL_BLENDMODE_BLEND) == -1) { // err
			std::cout << "SDL_SetTextureBlendMode failed(mutli): " << SDL_GetError() << "\n";
			return -1;
		}

		if (SDL_SetTextureAlphaMod(elme->texture, alpha) == -1) {
			std::cout << "SDL_SetTextureAlphaMod failed(mutli): " << SDL_GetError() << "\n";
			return -1;
		}

		SDL_Point center = { NULL, NULL };

		SDL_RenderCopyEx(multi_Image_control_in.win_data->renderer, elme->texture, &src, &dst, angle, &center, flip);
	}

	return 1;
}
#pragma once
#include "SDL_open_Windows.h"
#include "mouse_contro.h"
#include "ImageData.h"

struct ImgData_control {
	Window_data* win_data; 
	Mouse *mouse_control; 
	ImageData *img; 
};

inline void ImgData_control_function(ImgData_control& Image_control , Window_data &win_in , Mouse& mouse_in, ImageData& img_in, Super_point& bef_point) {
	if (mouse_in.mouseState == IN_LB_PR_HOVER) {
		if ((mouse_in.point_mouse.x >= img_in.img_p.x) && (mouse_in.point_mouse.x <= img_in.img_p.x + img_in.pw + 10)
			&& (mouse_in.point_mouse.y >= img_in.img_p.y) && (mouse_in.point_mouse.y <= img_in.img_p.y + img_in.ph + 10)) {
			std::cout << "in captain range \n";
			std::cout << bef_point.x << " " << bef_point.y << "\n";
			if ((bef_point.x != mouse_in.point_mouse.x) || (bef_point.y != mouse_in.point_mouse.y)) {
				std::cout << mouse_in.point_mouse.x - bef_point.x << " " << mouse_in.point_mouse.y - bef_point.y;
				img_in.img_p += (mouse_in.point_mouse - bef_point);
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
	Image_control = {&win_in, &mouse_in , &img_in }; 
}

inline int ImageData_control_draw(ImgData_control& Image_control, matrix_img scale = { 1,1 }, SDL_RendererFlip flip = SDL_FLIP_NONE , int alpha = 255 , double angle = NULL) {
	int wc = Image_control.img->frame % Image_control.img->wn;
	int hc = Image_control.img->frame / Image_control.img->wn;

	SDL_Rect src, dst;
	src.x = Image_control.img->width / Image_control.img->wn * wc;
	src.y = Image_control.img->height / Image_control.img->hn * hc;
	src.w = Image_control.img->width / Image_control.img->wn;
	src.h = Image_control.img->height / Image_control.img->hn;

	dst.x = Image_control.img->img_p.x;
	dst.y = Image_control.img->img_p.y;

	dst.w = src.w * scale._x;
	dst.h = src.h * scale._y;

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

#pragma once

#include <iostream>
#include <SDL.h> // Using SDL
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h> 
#include <string>
#include "SDL_open_Windows.h"

struct ImageData {
	SDL_Texture* texture;
	int width, height;
	int num;
	int wn, hn;
	int pw, ph;
	int frame; 
	Super_point img_p; 
};

struct matrix_img {
	double _x, _y; 
};

// ImageData loadTexture(char *path, bool ckEnable, Uint8 r, Uint8 g, Uint8 b)
inline void LoadImgTexture(Window_data& win_in, ImageData& IMG_in, std::string path, int num, int hn, int wn, bool ckEnable, Super_point point = Super_point{ 0 , 0 }, int frame_of_single = 0, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255) {
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		std::cout << "IMG_Load failed: " << IMG_GetError() << "\n"; 
		IMG_in = { NULL }; 
	}
	else {
		//ImageData img;

		// Set the color key (transparent pixel) in a surface.		
		SDL_SetColorKey(loadedSurface, ckEnable, SDL_MapRGB(loadedSurface->format, r, g, b));
		
		// Create texture from surface pixels
		IMG_in.texture = SDL_CreateTextureFromSurface(win_in.renderer, loadedSurface);
		if (IMG_in.texture == NULL) {
			std::cout << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << "\n"; 
			//printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get image dimensions and information
		IMG_in.width = loadedSurface->w;
		IMG_in.height = loadedSurface->h;
		IMG_in.num = num;
		IMG_in.wn = wn;
		IMG_in.hn = hn;
		IMG_in.frame = frame_of_single; 
		IMG_in.img_p = point; 

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

		//return newTexture;
		return; 
	}
}


inline int ImgRender(Window_data& win_in, ImageData& img, Super_point pos , Super_point c, double angle, SDL_RendererFlip flip, int alpha, matrix_img scale = {1 , 1}, bool move_mode = false , bool backgrand = false) {
	int wc = img.frame % img.wn;
	int hc = img.frame / img.wn;

	SDL_Rect src, dst;
	src.x = img.width / img.wn * wc;
	src.y = img.height / img.hn * hc;
	src.w = img.width / img.wn;
	src.h = img.height / img.hn;

	if (backgrand) {
		scale._x = (double)win_in.WIDTH / (double)src.w; // 1000 is W of window 
		scale._y = (double)win_in.HEIGHT / (double)src.h;
	}

	if (move_mode) 
		pos = img.img_p; 
	
	dst.x = pos.x;
	dst.y = pos.y;

	dst.w = src.w * scale._x;
	dst.h = src.h * scale._y;

	img.pw = dst.w;
	img.ph = dst.h;

	// Set and enable standard alpha blending mode for a texture
	// https://wiki.libsdl.org/SDL_SetTextureBlendMode
	// https://wiki.libsdl.org/SDL_BlendMode
	if (SDL_SetTextureBlendMode(img.texture, SDL_BLENDMODE_BLEND) == -1) { // err
		std::cout << "SDL_SetTextureBlendMode failed: " << SDL_GetError() << "\n"; 
		return -1;
	}

	// Modulate texture alpha
	// https://wiki.libsdl.org/SDL_SetTextureAlphaMod
	if (SDL_SetTextureAlphaMod(img.texture, alpha) == -1) {
		std::cout << "SDL_SetTextureAlphaMod failed: " << SDL_GetError() << "\n"; 
		return -1;
	}

	//SDL_RenderCopy(renderer, img.texture, &src, &dst);

	// SDL_Point
	// A structure that defines a two dimensional point.
	// https://wiki.libsdl.org/SDL_Point
	SDL_Point center = { c.x, c.y };

	// Copy a portion of the texture to the current rendering target, 
	// optionally rotating it by angle around the given center and also flipping it top-bottom and/or left-right.
	// if center is NULL, rotation will be done around dstrect.w / 2, dstrect.h / 2
	// https://wiki.libsdl.org/SDL_RenderCopyEx
	SDL_RenderCopyEx(win_in.renderer, img.texture, &src, &dst, angle, &center, flip);
	return 1;
}

inline void DestroyImage(ImageData &Img_in) {
	SDL_DestroyTexture(Img_in.texture);
}
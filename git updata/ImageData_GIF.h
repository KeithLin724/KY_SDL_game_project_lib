#pragma once

#include "ImageData.h"
#include "SDL_open_Windows.h"
#include "ImageData.h"
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

struct ImageData_GIF {
	std::vector<ImageData> img_gif; 
	int frame;
	Super_point img_gif_p;
};

inline void Load_Img_GIF_Texture(Window_data& win_in, ImageData_GIF& IMG_GIF_in, std::string file_path, int num, int hn, int wn, bool ckEnable, Super_point point = Super_point{ 0 , 0 }, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255) {
	ImageData part_in; 
	IMG_GIF_in.img_gif_p = point; 
	for (const auto& elme : fs::directory_iterator(file_path)) { // absolu
		// for one 
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(elme.path().u8string().c_str());// path.c_str()
		if (loadedSurface == NULL) {
			std::cout << "IMG_Load failed: " << IMG_GetError() << "\n";
			part_in = { NULL };
		}
		else {
			//ImageData img;

			// Set the color key (transparent pixel) in a surface.		
			SDL_SetColorKey(loadedSurface, ckEnable, SDL_MapRGB(loadedSurface->format, r, g, b));

			// Create texture from surface pixels
			part_in.texture = SDL_CreateTextureFromSurface(win_in.renderer, loadedSurface);
			if (part_in.texture == NULL) {
				std::cout << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << "\n";
				//printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
			}

			//Get image dimensions and information
			part_in.width = loadedSurface->w;
			part_in.height = loadedSurface->h;
			part_in.num = num;
			part_in.wn = wn;
			part_in.hn = hn;
			part_in.frame = 0;
			part_in.img_p = IMG_GIF_in.img_gif_p;

			// Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);
			IMG_GIF_in.img_gif.push_back(part_in); 
		}
	}
	IMG_GIF_in.frame = IMG_GIF_in.img_gif.size(); 
}

inline int Img_GIF_Render(Window_data& win_in, ImageData_GIF& IMG_GIF_in, Super_point pos, Super_point c, double angle, SDL_RendererFlip flip, int alpha, matrix_img scale = { 1 , 1 }, bool move_mode = false, bool backgrand = false) {
	
	for (auto &elme : IMG_GIF_in.img_gif) {
		int wc = elme.frame % elme.wn;
		int hc = elme.frame / elme.wn;

		SDL_Rect src, dst;
		src.x = elme.width / elme.wn * wc;
		src.y = elme.height / elme.hn * hc;
		src.w = elme.width / elme.wn;
		src.h = elme.height / elme.hn;

		if (backgrand) {
			scale._x = (double)win_in.WIDTH / (double)src.w; // 1000 is W of window 
			scale._y = (double)win_in.HEIGHT / (double)src.h;
		}

		if (move_mode)
			pos = IMG_GIF_in.img_gif_p; //elme.img_p;

		dst.x = pos.x;
		dst.y = pos.y;

		dst.w = src.w * scale._x;
		dst.h = src.h * scale._y;

		elme.pw = dst.w;
		elme.ph = dst.h;

		if (SDL_SetTextureBlendMode(elme.texture, SDL_BLENDMODE_BLEND) == -1) { // err
			std::cout << "SDL_SetTextureBlendMode failed: " << SDL_GetError() << "\n";
			return -1;
		}

		// Modulate texture alpha
		// https://wiki.libsdl.org/SDL_SetTextureAlphaMod
		if (SDL_SetTextureAlphaMod(elme.texture, alpha) == -1) {
			std::cout << "SDL_SetTextureAlphaMod failed: " << SDL_GetError() << "\n";
			return -1;
		}

		SDL_Point center = { c.x, c.y };

		SDL_RenderCopyEx(win_in.renderer, elme.texture, &src, &dst, angle, &center, flip);
	}
	return 1;
}

inline void DestroyImage(ImageData_GIF& IMG_GIF_in) {
	for (auto &elme : IMG_GIF_in.img_gif)
		SDL_DestroyTexture(elme.texture);
}
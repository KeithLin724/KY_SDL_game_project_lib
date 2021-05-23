#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h> // Using SDL
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include "SDL_open_Windows.h" 
using namespace std;

enum MouseState {
	NONE = 0,
	OUT = 1, // Mouse out
	IN_LB_SC = 2,  // Inside, Left Button, Single Click
	IN_RB_SC = 3,  // Inside, RIGHT Button, Single Click
	IN_LB_DC = 4,  // Inside, Left Button, Double Click
	IN_RB_DC = 5,  // Inside, RIGHT Button, Double Click
	IN_LB_PR = 6,  // Inside, Left Button, Press
	IN_RB_PR = 7,  // Inside, Left Button, Press
	IN_WU = 8,  // Inside, Wheel UP
	IN_WD = 9,  // Inside, Wheel DOWN
	HOVER = 10, // Mouse hover
	IN_LB_PR_HOVER = 11 // Inside, Left Button, Press, Hover
};

struct Mouse {
	// Button constants
	Super_point point_mouse; 
	//int X, Y; // about now of mouse xy 
	int BUTTON_X , BUTTON_Y;// button range of xy  
	int BUTTON_WIDTH, BUTTON_HEIGHT;

	MouseState mouseState;
	//void (*Mouse_Event)(SDL_Event* e, Mouse& mouse_data_in, int& x, int& y);
	Mouse(Window_data Win_in, MouseState mouseState_in) {
		BUTTON_X = Win_in.WIDTH / 4; // button range of x 
		BUTTON_Y = Win_in.HEIGHT / 4; // button range of y 
		BUTTON_WIDTH = Win_in.WIDTH / 2;
		BUTTON_HEIGHT = Win_in.HEIGHT / 2;
		mouseState = mouseState_in;
		//Mouse_Event  = MouseHandleEvent;
	 }
	
};

/*void set_image_mouse(ImageData Image_in, MouseState mouseState_in  , Mouse &mouse_in) {
	mouse_in.BUTTON_X = Image_in.px; // button range of x 
	mouse_in.BUTTON_Y = Image_in.py; // button range of y 
	mouse_in.BUTTON_WIDTH = Image_in.width;
	mouse_in.BUTTON_HEIGHT = Image_in.height;
	mouse_in.mouseState = mouseState_in;
	//Mouse_Event  = MouseHandleEvent;
}*/

inline void Mouse_Event(Window_data &win_in, Mouse& mouse_in)  {	
	// about mouse data move // int* x, int* y // If mouse event happened
	// https://wiki.libsdl.org/SDL_Event
	// https://wiki.libsdl.org/SDL_MouseMotionEvent
	// https://wiki.libsdl.org/SDL_MouseButtonEvent
	// https://wiki.libsdl.org/SDL_MouseWheelEvent
	if (win_in.window_event.type == SDL_MOUSEMOTION || win_in.window_event.type == SDL_MOUSEBUTTONDOWN || win_in.window_event.type == SDL_MOUSEBUTTONUP || win_in.window_event.type == SDL_MOUSEWHEEL) {
		// Get mouse position
		// https://wiki.libsdl.org/SDL_GetMouseState		
		SDL_GetMouseState(&mouse_in.point_mouse.x, &mouse_in.point_mouse.y);
		//printf("(%d, %d)\n", mouse_in.X, mouse_in.Y);

		// Mouse is at left/right/above/below of the button. Mouse is outside the button
		if (mouse_in.point_mouse.x < mouse_in.BUTTON_X || mouse_in.point_mouse.x >mouse_in.BUTTON_X + mouse_in.BUTTON_WIDTH || mouse_in.point_mouse.y < mouse_in.BUTTON_Y || mouse_in.point_mouse.y>mouse_in.BUTTON_Y + mouse_in.BUTTON_HEIGHT) {
			mouse_in.mouseState = OUT;
		}
		// Mouse is inside button
		{
			static int lasttime = SDL_GetTicks();
			static int curtime = SDL_GetTicks();
			int timediv;

			lasttime = curtime;
			curtime = SDL_GetTicks();
			timediv = curtime - lasttime;

			switch (win_in.window_event.type ){
			case SDL_MOUSEBUTTONDOWN:
				/*
				if (e->button.state == SDL_PRESSED)
				{
					*mouseState = IN_LB_PR;
				}
				*/
				break;

			case SDL_MOUSEBUTTONUP:
				//*mouseState = NONE;

				
				/*if (e->button.button == SDL_BUTTON_LEFT && timediv > 800)
				{
					mouse_in.mouseState = IN_LB_PR;
				}
				else if (e->button.button == SDL_BUTTON_RIGHT && timediv > 800)
				{
					mouse_in.mouseState = IN_RB_PR;
				}
				else */
				if (win_in.window_event.button.button == SDL_BUTTON_LEFT && win_in.window_event.button.clicks == 2 && timediv < 150) {
					mouse_in.mouseState = IN_LB_DC;
				}
				else if (win_in.window_event.button.button == SDL_BUTTON_RIGHT && win_in.window_event.button.clicks == 2 && timediv < 150) {
					mouse_in.mouseState = IN_RB_DC;
				}
				else if (win_in.window_event.button.button == SDL_BUTTON_LEFT && win_in.window_event.button.clicks == 1 && timediv < 800 && timediv > 100) {
					mouse_in.mouseState = IN_LB_SC;
				}
				else if (win_in.window_event.button.button == SDL_BUTTON_RIGHT && win_in.window_event.button.clicks == 1 && timediv < 800 && timediv > 100) {
					mouse_in.mouseState = IN_RB_SC;
				}

				break;

			case SDL_MOUSEWHEEL:
				if (win_in.window_event.wheel.y > 0) {// scroll up
					// Put code for handling "scroll up" here!
					mouse_in.point_mouse.y = win_in.window_event.wheel.y;
					mouse_in.mouseState = IN_WU;
				}
				else if (win_in.window_event.wheel.y < 0) { // scroll down
					// Put code for handling "scroll down" here!
					mouse_in.point_mouse.y = win_in.window_event.wheel.y;
					mouse_in.mouseState = IN_WD;
				}
				break;

			case SDL_MOUSEMOTION:
				mouse_in.mouseState = HOVER;
				if (win_in.window_event.button.button == SDL_BUTTON_LEFT) {
					mouse_in.mouseState = IN_LB_PR_HOVER;
				}
				break;
			}
		}
	}
}







/*inline void Mouse_Event(SDL_Event* e, Mouse& mouse_data_in, int* x, int* y) // about mouse data move // SDL_Event* e, MouseState& mouseState, int& x, int& y
{
	// If mouse event happened
	// https://wiki.libsdl.org/SDL_Event
	// https://wiki.libsdl.org/SDL_MouseMotionEvent
	// https://wiki.libsdl.org/SDL_MouseButtonEvent
	// https://wiki.libsdl.org/SDL_MouseWheelEvent
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEWHEEL)
	{
		// Get mouse position
		// https://wiki.libsdl.org/SDL_GetMouseState
		SDL_GetMouseState(x, y);
		//printf("(%d, %d)\n", x, y);

		// Mouse is at left/right/above/below of the button. Mouse is outside the button
		if (x < mouse_data_in.BUTTON_X || x > mouse_data_in.BUTTON_X + mouse_data_in.BUTTON_WIDTH ||
			y < mouse_data_in.BUTTON_Y || y > mouse_data_in.BUTTON_Y + mouse_data_in.BUTTON_HEIGHT)
		{
			mouse_data_in.mouseState = OUT;
		}
		else // Mouse is inside button
		{
			static int lasttime = SDL_GetTicks();
			static int curtime = SDL_GetTicks();
			int timediv;

			lasttime = curtime;
			curtime = SDL_GetTicks();
			timediv = curtime - lasttime;

			switch (e->type)
			{
			case SDL_MOUSEBUTTONDOWN:

				break;

			case SDL_MOUSEBUTTONUP:

				if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 2 && timediv < 150)
				{
					mouse_data_in.mouseState = IN_LB_DC;
				}
				else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 2 && timediv < 150)
				{
					mouse_data_in.mouseState = IN_RB_DC;
				}
				else if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 1 && timediv < 800 && timediv > 100)
				{
					mouse_data_in.mouseState = IN_LB_SC;
				}
				else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 1 && timediv < 800 && timediv > 100)
				{
					mouse_data_in.mouseState = IN_RB_SC;
				}

				break;

			case SDL_MOUSEWHEEL:
				if (e->wheel.y > 0) // scroll up
				{
					// Put code for handling "scroll up" here!
					y = e->wheel.y;
					mouse_data_in.mouseState = IN_WU;
				}
				else if (e->wheel.y < 0) // scroll down
				{
					// Put code for handling "scroll down" here!
					y = e->wheel.y;
					mouse_data_in.mouseState = IN_WD;
				}
				break;

			case SDL_MOUSEMOTION:
				mouse_data_in.mouseState = HOVER;
				if (e->button.button == SDL_BUTTON_LEFT)
				{
					mouse_data_in.mouseState = IN_LB_PR_HOVER;
				}
				break;
			}
		}
	}
}*/

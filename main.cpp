#include<iostream>
#include<SDL.h> 

using namespace std;

int main(int argc, char* argv[]) {
	SDL_Window* window = SDL_CreateWindow("square", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_Rect square = { 450, 350, 50, 50 };

	bool running = true;
	SDL_Event event;

	int speed = 10;
	int r = 0, g = 0, b = 0;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP: square.y -= speed; break;
				case SDLK_DOWN: square.y += speed; break;
				case SDLK_LEFT: square.x -= speed; break;
				case SDLK_RIGHT: square.x += speed; break;
				case SDLK_LSHIFT: speed *= 2; break;
				case SDLK_r: r = 220, g = 100, b = 50; break;
				case SDLK_g: r = 50, g = 240, b = 160; break;
				case SDLK_b: r = 75, g = 50, b = 210; break;
				}
			}
			else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_LSHIFT: speed = 10;
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 40, 125, 70, 110);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, r, g, b,130);
		SDL_RenderFillRect(renderer, &square);
		SDL_RenderPresent(renderer);	
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit;
	return 0;
}
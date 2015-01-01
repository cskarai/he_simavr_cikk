#include "hw_simulator_ui.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window*   screen = NULL;
SDL_Texture*  background = NULL;
SDL_Renderer* renderer = NULL;

int visible_leds = 0;     // a látható LED-ek
int redraw = 0;           // kell-e újra rajzolás
int currently_pushed = 0; // az éppen lenyomott gomb

void    hw_simulator_ui_set_led_state(int led, int state)
{
	if( state )
		visible_leds |= (1 << led );
	else
		visible_leds &= ~(1 << led );
	
	redraw = 1;
}

void hw_simulator_ui_init()
{
	if( SDL_Init(SDL_INIT_VIDEO) != 0) // SDL inicializálás
	{
		fprintf(stderr, "\nNem sikerült az SDL-t elindítani: %s\n", SDL_GetError() );
		exit(-1);
	}
	
	screen = SDL_CreateWindow("Szimulátor",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		169, 219, SDL_WINDOW_OPENGL);
	if( screen == NULL )
	{
		fprintf(stderr, "\nNem sikerült ablakot létrehozni: %s\n", SDL_GetError() );
		exit(-1);
	}

	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Surface* image = SDL_LoadBMP("panel.bmp"); // háttérkép betöltése
	background = SDL_CreateTextureFromSurface(renderer, image);
	
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_FreeSurface(image); 
}

void hw_simulator_ui_quit()
{
	SDL_DestroyTexture(background);
	background = NULL;
	SDL_DestroyRenderer(renderer);  
	renderer = NULL;
	SDL_DestroyWindow(screen);
	screen = NULL;
	SDL_Quit(); // SDL lezárása
}

int  hw_simulator_ui_get_button(int x, int y)
{
	if( ( y < 182 ) || ( y > 212 ) )
		return 0;
	if( ( x >= 9 ) && ( x < 39 ) )
		return 1;
	if( ( x >= 9 + 60 ) && ( x < 39 + 60 ) )
		return 2;
	if( ( x >= 9 + 120 ) && ( x < 39 + 120 ) )
		return 3;
	return 0;
}

void hw_simulator_ui_draw_screen()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	int n;
	for( n = 0; n < 9; n++ )
	{
		if( visible_leds & (1 << n) )
		{
			SDL_Rect rect = {14 + 14*n, 22, 11, 41};
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	
	if( currently_pushed )
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 96);
		SDL_Rect rct = {9 + 60*(currently_pushed - 1), 181, 31, 32};
		SDL_RenderFillRect(renderer, &rct);
	}
	
	redraw = 0; // újra lett rajzolva
	SDL_RenderPresent(renderer);
}

int  hw_simulator_ui_poll()
{
	SDL_Event event;

	/* SDL esemény lekérése */
	if (SDL_PollEvent(&event)) {
		uint8_t last_pushed = currently_pushed;
		switch (event.type) {
			/* ablak bezárása */
			case SDL_QUIT:
				hw_simulator_ui_quit();
				return QUIT;

			/* billentyű */
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
					case SDLK_q:
						hw_simulator_ui_quit();
						return QUIT;
					default:
						break;
				}
			case SDL_MOUSEMOTION:
				{
					if( event.motion.state & SDL_BUTTON_LMASK )
					{
						int btn = hw_simulator_ui_get_button(event.button.x, event.button.y);
						if( btn == 0 )
							currently_pushed = 0;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if( event.button.button == SDL_BUTTON_LEFT)
					currently_pushed = hw_simulator_ui_get_button(event.button.x, event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				if( event.button.button == SDL_BUTTON_LEFT)
					currently_pushed = 0;
				break;
			default:
				break;
		}
		hw_simulator_ui_draw_screen();
		
		if( last_pushed != currently_pushed )
		{
			if( currently_pushed == 0 )
			{
				if( last_pushed == 1 )
					return BUTTON_1_RELEASED;
				if( last_pushed == 2 )
					return BUTTON_2_RELEASED;
				if( last_pushed == 3 )
					return BUTTON_3_RELEASED;
			}
			if( last_pushed == 0 )
			{
				if( currently_pushed == 1 )
					return BUTTON_1_PRESSED;
				if( currently_pushed == 2 )
					return BUTTON_2_PRESSED;
				if( currently_pushed == 3 )
					return BUTTON_3_PRESSED;
			}
		}
	}
	if( redraw )
		hw_simulator_ui_draw_screen();

	return NO_OPERATION;
}

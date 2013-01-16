#ifndef _COOLSTUFF_H_
#define _COOLSTUFF_H_

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>


using std::endl;

/*
  ----------------CONSTANTS AND GLOBAL VARIABLES--------------------
  - Mostly things for image manipulation involving SDL_Surface variables, and various other SDL objects
  - Also frame rate and resolution shenanigans
*/

//GLOBAL VARIABLES

SDL_Surface *screen = NULL;
SDL_Surface *black = NULL;
SDL_Surface *white = NULL;
SDL_Surface *txt = NULL;
SDL_Surface *tile = NULL;

SDL_Event event;

SDL_Rect *text = NULL;

TTF_Font* font = NULL;
TTF_Font* pieces_font = NULL;

SDL_Color textC = {0,0,0}, pieceC1 = {200, 200, 200}, pieceC2 = {56, 56, 56}, pieceC3 = {255,205,0};
SDL_Color wh = {255, 255, 255};

bool quit = false;

//CONSTANTS

const int SCRN_WIDTH = 800;
const int SCRN_HEIGHT = 800;
const int SCRN_BPP = 32;

const int FPS = 30;

//DATA STRUCTURES

struct coord {
    int x, y;
    bool isInBox(int x0, int y0, int w, int h) {
    	if ((x > x0 && x < (x0 + w)) && (y > y0) && (y < (y0 + h))) {
    		return true;
    	}
    	else return false;
    }
    coord() {
    	x = y = 0;
    }
    coord(int _x, int _y) {
    	x = _x;
    	y = _y;
    }
    struct coord operator=(const struct coord &right) {
    	if (this == &right) {
    		return *this;
    	}
    	this->x = right.x;
    	this->y = right.y;
    	return *this;
    }
    bool operator==(const struct coord* &right) {
		if (this->x == right->x && this->y == right->y) {
			return true;
		}
		else return false;
	}

	void showCoord() {
		std::cout << "(" << x << "," << y << ")";
	}
};


struct coord click;
struct coord *invalid = new coord(-1, -1);

/*
  ---------------FUNCTION PROTOTYPES---------------------------------
  - Generic function prototypes for all-purpose graphical functions -
  -
*/

//INITIALIZATION AND SUCH

bool init()
{
  	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
      	return false;
    }
  	screen = SDL_SetVideoMode(SCRN_WIDTH, SCRN_HEIGHT, SCRN_BPP,
			    SDL_SWSURFACE);
  	SDL_WM_SetCaption("Level Editor", "Level Editor");
  	if (TTF_Init() == -1) {
    	return false;
  	}
  	if (screen == NULL)
    {
      	return false;
    }
  	return true;
} //Simply initialization stuff for the SDL objects

SDL_Surface* imageLoad(std::string filename, bool trans = true);

bool load_files()
{
  	black = imageLoad("black_tile.png");
  	white = imageLoad("white_tile.png", false);
  	tile = imageLoad("border_tile.png");
  	font = TTF_OpenFont("throwhandsup.ttf", 64);
  	pieces_font = TTF_OpenFont("Inconsolata.ttf", 48);
  	if (font == NULL || black == NULL || white == NULL || tile == NULL || pieces_font == NULL)
    {
      	return false;
    }
  	return true;
} //Loads files like fonts and whatever else it needs at startup

//a more modular version of the load_files() function
//jay kay

//but here's a "load font" one for modularization
TTF_Font* openFont(std::string fontfile, int size=24) {
	font = TTF_OpenFont(fontfile.c_str(), size);
	return font;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest, SDL_Rect* clip = NULL);

void startup()
{
	font = openFont("throwhandsup.ttf", 64);
	pieces_font = openFont("Inconsolata.ttf", 64);
    if (!init() || !load_files() || font == NULL || pieces_font == NULL) {
        exit(0);
    }
  	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF,
					0xFF, 0xFF));
  	//apply_surface(0,0,bg, screen);
} //The very beginning stuff.  Right now it sends that bitch a smiley face

//GENERIC STUFF

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

//GRAPHICAL STUFF

void get_out();

struct coord getClick(bool canQuit = true) {
	struct coord result;
	SDL_WaitEvent(&event);
	if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN) {
		SDL_GetMouseState(&result.x, &result.y);
	}
	else if (canQuit && event.type == SDL_QUIT) {
		get_out();
		exit(0);
	}
	else if (event.type != SDL_MOUSEMOTION) {
		result.x = click.x;
		result.y = click.y;
	}
	return result;
}

void boxArea(int x, int y, int w, int h) {
	for (int xn = 0; xn <= w; xn+=5) {
		for (int yn = 0;yn <= h; yn+=5) {
			if (xn % w == 0 || yn % h == 0) {
				apply_surface(x + xn, y + yn, tile, screen);
			}
		}
	}
}

SDL_Surface* imageLoad(std::string filename, bool trans)
{
  	SDL_Surface *spriteX = NULL;
 	SDL_Surface *final = NULL;
  	spriteX = IMG_Load(filename.c_str());
  	if (spriteX != NULL)
  	{
      	final = SDL_DisplayFormat(spriteX);
      	SDL_FreeSurface(spriteX);
		if (final != NULL)
		{
		  	Uint32 colorkey = SDL_MapRGB(final->format, 0xFF,
						   0xFF, 0xFF);
			if (trans == true) {
				SDL_SetColorKey(final, SDL_RLEACCEL | SDL_SRCCOLORKEY,
				  colorkey);
			}
		}
        else {
            std::cerr << "SDL_FreeSurface goofed on " << filename << std::endl;
		}
    }
    else {
        std::cerr << "oops at " << filename;
    }
  	return final;
} //Loads the image at filename, if trans, will load like a sprite map

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface*
		   destination, SDL_Rect* clip)
{
  SDL_Rect Offset;
  Offset.x = x;
  Offset.y = y;

  SDL_BlitSurface(source, clip, destination, &Offset);
} //Puts source at x,y of destination, if clip is set it'll do this all in the bounds of clip

void clearBox(int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	Uint32 colorkey = SDL_MapRGB(tile->format, 0xFF,
						   0xFF, 0xFF);
	SDL_FillRect(screen, &rect, colorkey);
}

void refresh(SDL_Surface* screen, int x = 0, int y = 0, int w = 0, int h = 0){
  SDL_UpdateRect(screen, x, y, w, h);
}
//refreshes the screen because I hate all the SDL_Flip == -1 shit

void makeText(std::string message, TTF_Font* hereFont= font, SDL_Color color = textC, int x = SCRN_WIDTH/2, int y = SCRN_HEIGHT/2)
{
  const char* pancake = message.c_str();
  txt = TTF_RenderText_Solid( hereFont, pancake, color);
  if (txt == NULL) {
    std::cerr << "Could not render:" << message;
  }
  apply_surface(x, y, txt, screen);
  SDL_FreeSurface(txt);
} //Puts message at x,y on the screen, neat stuff


void introAnimation() {
	int n = 0;
	SDL_Color rawr = {n,n,n};
	while (n < 256) {
		refresh(screen);
		makeText("HELLO THERE", font, rawr);
		n++;
		rawr.r = rawr.g = rawr.b = n;
		SDL_Delay(5);
	}
}

void clearScreen() {
	Uint32 asdf = SDL_MapRGB(screen->format, 0xFF,
						   0xFF, 0xFF);
	if (SDL_FillRect(screen, NULL, asdf) == -1) {
		std::cerr<< "Could not clear the screen!" << endl;
	}
}

//SHUTDOWN STUFF

void clean_up()
{
  SDL_FreeSurface(screen);
  //SDL_FreeSurface(txt);
  TTF_CloseFont(font);
} //Frees all the SDL objects, nuff said
void get_out()
{
  clean_up();
  TTF_Quit();
  SDL_Quit();
} //shuts down SDL and exits the program

/*
  ----------------CLASSES--------------------------------------------
  -
*/


#endif

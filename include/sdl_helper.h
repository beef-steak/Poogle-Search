#ifndef SDL_HELPER_H
#define SDL_HELPER_H

#include <Ltexture.h>

struct Point2dx{
	double x;
	double y;
};

struct MotionX{
	Point2dx pos;
	Point2dx v;
	Point2dx a;
	Point2dx hit;
	double rotation;
};

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

bool loadMedia(const char* location, SDL_Texture*& gTexture, SDL_Renderer*& gRenderer);
bool loadMedia(const char* location,SDL_Renderer*& gRenderer,TTF_Font*& gFont,LTexture* gTextTexture,int psize);
SDL_Surface* loadSurface( const char* path, SDL_PixelFormat *gPx);
SDL_Texture* loadTexture( const char* path, SDL_Renderer*& gRenderer );


bool init(const char* wName,SDL_Window*& gWindow, SDL_Renderer*& gRenderer,SDL_Surface*& gScreenSurface)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( wName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}else{
                    gScreenSurface = SDL_GetWindowSurface( gWindow );
                }

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(const char* location,SDL_Renderer*& gRenderer,TTF_Font*& gFont,LTexture* gTextTexture,int psize)
{
	//Loading success flag
	bool success = true;

	//Open the font
	//16_true_type_fonts/lazy.ttf
    //"../asset/fonts/lazy.ttf"
	gFont = TTF_OpenFont( location, psize );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if( !gTextTexture->loadFromRenderedText( "The quick brown fox jumps over the lazy dog", gRenderer, gFont  ,textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

bool loadMedia(const char* location, SDL_Texture*& gTexture, SDL_Renderer*& gRenderer)
{
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture( location,gRenderer );
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void close(SDL_Window*& gWindow,SDL_Renderer*& gRenderer)
{
	//Free loaded images
	//gTextTexture.free();

	//Free global font
	//TTF_CloseFont( gFont );
	//gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface( const char* path, SDL_PixelFormat *gPx)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gPx, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

SDL_Texture* loadTexture( const char* path, SDL_Renderer*& gRenderer )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

#endif
#include <string>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void reportError(std::string msg);
void close();
SDL_Surface* loadSurface(std::string path);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gPNGSurface = NULL;

int main(int argc, char* argv[])
{

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Apply the PNG image
				SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );
			
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}


bool init()
{
  bool success = true;

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    reportError("Could not initialize basic video");
    success = false;
  } else {
    gWindow = SDL_CreateWindow("extensions demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      reportError("Could not create window.");
      success = false;
    } else {
      // get dat PNG loadin'
      int imgFlags = IMG_INIT_PNG;
      if(!(IMG_Init(imgFlags) & imgFlags)) {
        reportError("Could not load PNG extension");
        success = false;
      } else {
        gScreenSurface = SDL_GetWindowSurface(gWindow);
      }
    }
  }

  return success;
}

/**
 * Cause we ain' playin'
 */
void reportError(std::string msg)
{
  printf("%s SDL Error: %s\n", msg.c_str(), SDL_GetError());
}

SDL_Surface* loadSurface(std::string path)
{
  SDL_Surface* optimizedSurface = NULL;

  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL) {
    printf("Could not load PNG: %s\n", IMG_GetError());
  } else {
    optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
    SDL_FreeSurface(loadedSurface);
  }

  return optimizedSurface;
}

// TODO: Could put this in a little class to wrap SDL
void close() 
{
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();
}

bool loadMedia()
{
  gPNGSurface = loadSurface("./06_extension_libraries_and_loading_other_image_formats/loaded.png");
  return true;
}
